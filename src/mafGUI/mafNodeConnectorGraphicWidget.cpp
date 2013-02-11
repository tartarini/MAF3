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
	//dw super class destructor should do this, right?
	if (scene() != NULL) {
		this->scene()->removeItem(this);
	}
}

mafNodeConnectorGraphicWidget::mafNodeConnectorGraphicWidget(mafNodeGraphicWidget *parent1, QGraphicsScene *scene1, /*QLabel*/QWidget* widget, mafNodeConnectorGraphicWidget::ConnectorType conType1, const ConnectorAlignment connectorAlignment, bool singleConnection, bool disableWidgetOnConnection, int radius, bool selfConnections)
		//dw666 orig:
		: QGraphicsItem(parent1, scene1),
		//: QGraphicsItem(NULL, scene1),
	mConnectorType(conType1), parent(parent1), mWidget(widget), mConnectorAlignment(connectorAlignment), mSingleConnection(singleConnection), mDisableWidgetOnConnection(disableWidgetOnConnection), mSelfConnections(selfConnections), mRadius(radius)
{
	//orig
    //setFlag(ItemIsMovable);
	//dw new4: is this a problem? check it
    setCacheMode(DeviceCoordinateCache);

    //dw new: setZValue(1);
	setZValue(2);

	//dw667: new try
	updatePosition();

	//setFlag(QGraphicsItem::ItemIsSelectable, true);

        if (conType1 == In) {
		darkColor =  QColor(Qt::darkYellow);
	}
        else if (conType1 == Out) {
		darkColor =  QColor(Qt::darkRed).light(80);
	}
	else {
		darkColor =  QColor(Qt::darkGreen);
	}
	highlight = false;
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

	//if we dont have parent, stay where we are
	if (parent == NULL || mWidget == NULL) {
		return;
	}

	QPointF pPos = parent->pos();
	QSize widgetSize = mWidget->size();
	//dw677:
	//QSizeF widgetSize = parent->size();
	//radius = labelSize.height()/2;

	QPointF newPos;
        if (connectorAlignment() == mafNodeConnectorGraphicWidget::Left) {
		newPos.setX(-mRadius);
		//newPos.setY(mWidget->pos().y() + widgetSize.height()/2.0);
		//dw677:
		//newPos.setY(parent->pos().y() + widgetSize.height()/2.0);
		//newPos.setY(mWidget->pos().y() + mWidget->rect().height()/2.0);
		//newPos.setY(mWidget->pos().y() + mWidget->rect().height()/2.0);
		//newPos.setY(mWidget->pos().y() + mWidget->height()/2.0);
		newPos.setY(parent->subWidgetRect(mWidget).y() + parent->subWidgetRect(mWidget).height()/2.0);
	}
        else if (connectorAlignment() == mafNodeConnectorGraphicWidget::Right) {
		//seems to be to big
		//newPos.setX(parent->boundingRect().width()+mRadius+1);
		//seems to be size of content??? too small
		//newPos.setX(parent->window()->size().width()+mRadius+1);
                //float rightx = dynamic_cast<mafNodeGraphicWidget*>(parent)->widget()->width()
		//	+ mRadius + 1;
                //float rightx = dynamic_cast<mafNodeGraphicWidget*>(parent)->widget()->geometry().size().width()
		//	+ mRadius + 1;
		/*
                float rightx = dynamic_cast<mafNodeGraphicWidget*>(parent)->widget()->width()
                        + dynamic_cast<mafNodeGraphicWidget*>(parent)->widget()->baseSize().width()
			+ mRadius + 1;
			*/

		//dw FIXME: ugly
		newPos.setX(parent->rect().width() + mRadius);
		//newPos.setY(mWidget->pos().y()+ widgetSize.height()/2.0);
		newPos.setY(parent->subWidgetRect(mWidget).y()+ parent->subWidgetRect(mWidget).height()/2.0);
	}
        else if (connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom) {
		//newPos.setX(mWidget->pos().x() + widgetSize.width()/2.0);
		newPos.setX(parent->subWidgetRect(mWidget).x() + parent->subWidgetRect(mWidget).width()/2.0);

		//newPos.setY(parent->widget()->rect().height() + mRadius);
		//newPos.setY(parent->boundingRect().height() + mRadius);
		newPos.setY(parent->rect().height() + mRadius);
	}
        else if (connectorAlignment() == mafNodeConnectorGraphicWidget::Top) {
		//newPos.setX(mWidget->pos().x() + widgetSize.width()/2.0);
		newPos.setX(parent->subWidgetRect(mWidget).x() + parent->subWidgetRect(mWidget).width()/2.0);
		newPos.setY(-mRadius);
	}


	this->setPos(newPos);
	//dw seems to always paint -> is already called from update? or will cause updatePos in next frame again?
	//update();

        foreach (mafNodeConnectionGraphicWidget *c, this->arrows) {
		//dw problem: label already deleted but connector tries to enable it?
        //c->paint(painter, option, w);
		c->updatePosition();
    }
	update();
}

/*
void mafNodeConnectorGraphicWidget::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> mafNodeConnectorGraphicWidget::edges() const
{
    return edgeList;
}

void mafNodeConnectorGraphicWidget::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }
    
    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        mafNodeConnectorGraphicWidget *node = qgraphicsitem_cast<mafNodeConnectorGraphicWidget *>(item);
        if (!node)
            continue;

        QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
        qreal dx = line.dx();
        qreal dy = line.dy();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * 10;
    foreach (Edge *edge, edgeList) {
        QPointF pos;
        if (edge->sourcemafNodeConnectorGraphicWidget() == this)
            pos = mapFromItem(edge->destmafNodeConnectorGraphicWidget(), 0, 0);
        else
            pos = mapFromItem(edge->sourcemafNodeConnectorGraphicWidget(), 0, 0);
        xvel += pos.x() / weight;
        yvel += pos.y() / weight;
    }
    
    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

bool mafNodeConnectorGraphicWidget::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}*/

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

	//dw new
	//return QGraphicsItem::boundingRect();
}

QPainterPath mafNodeConnectorGraphicWidget::shape() const
{
	
    QPainterPath path;
    path.addEllipse(-mRadius, -mRadius, 2*mRadius, 2*mRadius);
    return path;
	
	//dw new
	//return QGraphicsItem::shape();

	//dw new2
	/*
	QPainterPath path;
	path.addRect(boundingRect());
    return path;
	*/
}


void mafNodeConnectorGraphicWidget::debugPaint(QPainter *painter) {
	//dw debug
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
	//painter->drawEllipse(-mRadius*0.8, -mRadius*0.8, mRadius, mRadius);
	//painter->drawEllipse(0, 0, 2*mRadius, 2*mRadius);
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

	//dw new2, is abstract
	//QGraphicsItem::paint(painter, option, w);
	
	/*
	//dw hacky, good idea??
        mafNodeConnectionGraphicWidget* a;
	a->p
        foreach(mafNodeConnectionGraphicWidget* a, arrows) {
		a->paint(painter, option, NULL);
	}
*/
	
/*
	painter->drawEllipse(-7, -7, 20, 20);
    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
	*/
}

//QVariant mafNodeConnectorGraphicWidget::itemChange(GraphicsItemChange change, const QVariant &value)
//{
	/*
    switch (change) {
    case ItemPositionHasChanged:
//        foreach (Edge *edge, edgeList)
//            edge->adjust();
//        graph->itemMoved();
        break;
    default:
        break;
    };
	*/

//    return QGraphicsItem::itemChange(change, value);
//}

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

/*
//not called, does the event exist for this class???
void mafNodeConnectorGraphicWidget::mouseOverEvent(QGraphicsSceneMouseEvent *event)
{
	highlight = true;
    update();
    //QGraphicsItem::mouseOverEvent(event);
}
*/

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
	//TODO: how to handle deletion exactly, when to emit signals
	//arrow->startConnector()->arrows.removeOne(arrow);
	//arrow->endConnector()->arrows.removeOne(arrow);

	//deleting arrow will also remove it from this connector
	//dw new
	//arrow->setParentItem(NULL);
        //delete arrow;
	//arrows removed during connection destruction, but attached outside => bad idea
}

/* removes a connection, but does not delete it*/
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

/*
void mafNodeConnectorGraphicWidget::enterEvent ( QEvent * event )  {
	highlight = true;
	//QGraphicsItem::enterEvent(event);
}
*/

//dw new3
void mafNodeConnectorGraphicWidget::update(const QRectF & rect) {
	
	//dw new3: why do ourself
        foreach (mafNodeConnectionGraphicWidget *c, this->arrows) {
		//dw problem: label already deleted but connector tries to enable it?
        //c->paint(painter, option, w);
		c->update(rect);
    }
	
	QGraphicsItem::update(rect);
}
