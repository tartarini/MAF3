#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QPointF>
#include <QSize>
#include <QGraphicsWidget>

#include "mafNodeConnectorGraphicWidget.h"
#include "mafNodeConnectionGraphicWidget.h"

using namespace mafGUI;

mafNodeConnectorGraphicWidget::~mafNodeConnectorGraphicWidget() {
	this->deleteConnections();
    if (scene() != NULL) {
		this->scene()->removeItem(this);
	}
}

mafNodeConnectorGraphicWidget::mafNodeConnectorGraphicWidget(mafNodeGraphicWidget *parent1, QGraphicsScene *scene1, /*QLabel*/QWidget* widget, mafNodeConnectorGraphicWidget::ConnectorType conType1, const ConnectorAlignment connectorAlignment, bool singleConnection, bool disableWidgetOnConnection, int radius, bool selfConnections)
        : QGraphicsItem(parent1), //was scene1 as second arg
		//: QGraphicsItem(NULL, scene1),
        mConnectorType(conType1), 
		parent(parent1), 
		mWidget(widget), 
		mConnectorAlignment(connectorAlignment), 
		mSingleConnection(singleConnection), 
		mDisableWidgetOnConnection(disableWidgetOnConnection), 
		mSelfConnections(selfConnections), 
		mRadius(radius) {
	//orig
    //setFlag(ItemIsMovable);
    setCacheMode(DeviceCoordinateCache);

    setZValue(2);

    updatePosition();


    if (conType1 == In) {
		darkColor =  QColor(Qt::darkYellow);
    } else if (conType1 == Out) {
		darkColor =  QColor(Qt::darkRed).light(80);
    } else {
		darkColor =  QColor(Qt::darkGreen);
	}
	highlight = false;
}

int mafNodeConnectorGraphicWidget::type() const {
    return Type;
}

bool mafNodeConnectorGraphicWidget::singleConnection() const {
    return mSingleConnection;
}

void mafNodeConnectorGraphicWidget::setSingleConnection(bool singleConnection) {
        mSingleConnection = singleConnection;
}

bool mafNodeConnectorGraphicWidget::selfConnections() const {
    return mSelfConnections;
}

void mafNodeConnectorGraphicWidget::setSelfConnections(bool selfConnections) {
        mSelfConnections = selfConnections;
}

mafNodeConnectorGraphicWidget::ConnectorType mafNodeConnectorGraphicWidget::connectorType() const {
    return mConnectorType;
}

void mafNodeConnectorGraphicWidget::setConnectorType(ConnectorType c) {
        mConnectorType = c;
}

mafNodeConnectorGraphicWidget::ConnectorAlignment mafNodeConnectorGraphicWidget::connectorAlignment() const {
        return mConnectorAlignment;
}
void mafNodeConnectorGraphicWidget::setConnectorAlignment(ConnectorAlignment alignment) {
        mConnectorAlignment = alignment;
}

void mafNodeConnectorGraphicWidget::updatePosition() {
	this->prepareGeometryChange();

    if (parent == NULL || mWidget == NULL) {
		return;
	}

	QPointF pPos = parent->pos();
	QSize widgetSize = mWidget->size();

	QPointF newPos;
        if (connectorAlignment() == mafNodeConnectorGraphicWidget::Left) {
		newPos.setX(-mRadius);
        newPos.setY(parent->subWidgetRect(mWidget).y() + parent->subWidgetRect(mWidget).height()/2.0);
    } else if (connectorAlignment() == mafNodeConnectorGraphicWidget::Right) {

        newPos.setX(parent->rect().width() + mRadius);
        newPos.setY(parent->subWidgetRect(mWidget).y()+ parent->subWidgetRect(mWidget).height()/2.0);
    } else if (connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom) {
        newPos.setX(parent->subWidgetRect(mWidget).x() + parent->subWidgetRect(mWidget).width()/2.0);

        newPos.setY(parent->rect().height() + mRadius);
    } else if (connectorAlignment() == mafNodeConnectorGraphicWidget::Top) {
        newPos.setX(parent->subWidgetRect(mWidget).x() + parent->subWidgetRect(mWidget).width()/2.0);
		newPos.setY(-mRadius);
	}


	this->setPos(newPos);

        foreach (mafNodeConnectionGraphicWidget *c, this->arrows) {
        c->updatePosition();
    }
	update();
}


void mafNodeConnectorGraphicWidget::setHighlight(bool highlight) {
	prepareGeometryChange();
	this->highlight = highlight;
	this->updatePosition();
	this->update();
}

QRectF mafNodeConnectorGraphicWidget::boundingRect() const
{
    qreal adjust = 1;
    return QRectF(-mRadius - adjust, -mRadius - adjust,
                  2*(mRadius + adjust), 2*(mRadius + adjust));
}

QPainterPath mafNodeConnectorGraphicWidget::shape() const
{
	
    QPainterPath path;
    path.addEllipse(-mRadius, -mRadius, 2*mRadius, 2*mRadius);
    return path;
}

void mafNodeConnectorGraphicWidget::debugPaint(QPainter *painter) {
	static int i = 0, j=0, k=0;
	painter->fillRect(boundingRect(), /*Qt::green*/ QColor(i=(i+19)%256 , j=(j+51)%256, k=(k+11)%256)); // to see item.
}


void mafNodeConnectorGraphicWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* w)
{
    Q_UNUSED(option);
    Q_UNUSED(w);

    painter->setRenderHint(QPainter::Antialiasing);

    if (static_cast<mafDiagramScene*>(scene())->isDebugDraw()) {
        debugPaint(painter);
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    QRadialGradient gradient(-mRadius/2, -mRadius/2, mRadius);
	if (/*option->state & QStyle::State_Sunken*/ highlight) {
        gradient.setCenter(mRadius/2, mRadius/2);
        gradient.setFocalPoint(mRadius/2, mRadius/2);
        //gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        //gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
		gradient.setColorAt(1, darkColor.light(240));
		gradient.setColorAt(0, darkColor.light(120));
    } else {
        //gradient.setColorAt(0, Qt::yellow);
        //gradient.setColorAt(1, Qt::darkYellow);
		gradient.setColorAt(0, darkColor.light(240));
        gradient.setColorAt(1, darkColor);
    }
	painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    //painter->drawEllipse(-mRadius, -mRadius, mRadius, mRadius);
	painter->drawEllipse(-mRadius, -mRadius, 2*mRadius, 2*mRadius);
}

void mafNodeConnectorGraphicWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//highlight = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void mafNodeConnectorGraphicWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	setHighlight(false);
	//highlight = false;
    //update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void mafNodeConnectorGraphicWidget::addConnection(mafNodeConnectionGraphicWidget *arrow)
{
	if (mSingleConnection && arrows.count() > 0) {
		//delete arrows.first();
		//arrows.clear();
		deleteConnections();
	}

	if (mDisableWidgetOnConnection) {
		mWidget->setEnabled(false);
	}

    arrows.append(arrow);
}

void mafNodeConnectorGraphicWidget::deleteConnection(mafNodeConnectionGraphicWidget *arrow)
{
    //TODO: handle deletion exactly, when to emit signals
	//arrow->startConnector()->arrows.removeOne(arrow);
	//arrow->endConnector()->arrows.removeOne(arrow);

	//deleting arrow will also remove it from this connector
	//dw new
	//arrow->setParentItem(NULL);
        //delete arrow;
	//arrows removed during connection destruction, but attached outside => bad idea
}

void mafNodeConnectorGraphicWidget::removeConnection(mafNodeConnectionGraphicWidget *connection) {
	arrows.removeOne(connection);
	if (mDisableWidgetOnConnection && mWidget != NULL && arrows.count() == 0) {
		mWidget->setEnabled(true);
	}
}

void mafNodeConnectorGraphicWidget::deleteConnections()
{
        foreach(mafNodeConnectionGraphicWidget* arrow, arrows) {
		deleteConnection(arrow);
	}
}

void mafNodeConnectorGraphicWidget::removeWidget() {
	mWidget = NULL;
	parent = NULL;
}

void mafNodeConnectorGraphicWidget::update(const QRectF & rect) {
    foreach (mafNodeConnectionGraphicWidget *c, this->arrows) {
		//dw problem: label already deleted but connector tries to enable it?
        //c->paint(painter, option, w);
		c->update(rect);
    }
	
	QGraphicsItem::update(rect);
}
