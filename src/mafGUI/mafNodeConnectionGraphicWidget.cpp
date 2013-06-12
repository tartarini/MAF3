#include "mafNodeConnectionGraphicWidget.h"
#include "mafNodeConnectorGraphicWidget.h"
#include <math.h>

using namespace mafGUI;

const qreal Pi = 3.14;

mafNodeConnectionGraphicWidget::~mafNodeConnectionGraphicWidget(){
	if (this->mStartConnector != NULL) {
                //this->mStartConnector->removeConnection(this);
		this->mStartConnector = NULL;
	}
	if (this->mEndConnector != NULL) {
                //this->mEndConnector->removeConnection(this);
		this->mEndConnector = NULL;
	}

	if (scene() != NULL) {
		this->scene()->removeItem(this);
	}
}

mafNodeConnectionGraphicWidget::mafNodeConnectionGraphicWidget(mafNodeConnectorGraphicWidget *startConnector, mafNodeConnectorGraphicWidget *endConnector,
         QGraphicsItem *parent, QGraphicsScene *scene, bool bidirectional)
		 : QGraphicsPathItem(parent, scene)
{
    mStartConnector = startConnector;
    mEndConnector = endConnector;
    setFlag(QGraphicsItem::ItemIsSelectable, true);

	setBidirectional(bidirectional);

    mColor = Qt::black;
    setPen(QPen(mColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	arrowSize=10;

	setZValue(-1000.0);

	QPointF controlPoint1;
	QPointF controlPoint2;
	recreatePath(controlPoint1, controlPoint2);
}
//! [0]

int mafNodeConnectionGraphicWidget::type() const {
    return Type;
}


//! [1]
QRectF mafNodeConnectionGraphicWidget::boundingRect() const
{
	qreal extra = 3;
	return QGraphicsPathItem::boundingRect().normalized().adjusted(-extra, -extra, extra, extra);
	
}
//! [1]

//! [2]
QPainterPath mafNodeConnectionGraphicWidget::shape() const
{
	QPainterPath path = QGraphicsPathItem::shape();
    return path;
}

void mafNodeConnectionGraphicWidget::updatePosition() {
	prepareGeometryChange();
	update();
}

void mafNodeConnectionGraphicWidget::recreatePath(QPointF& controlPoint1, QPointF& controlPoint2)
{
	QPointF a(mapFromItem(mStartConnector, 0, 0));
	QPointF b(mapFromItem(mEndConnector, 0, 0));

	qreal dist = QLineF(a, b).length();
	qreal diffx = abs(a.x() - b.x());
	qreal diffy = abs(a.y() - b.y());

	QPointF left;
	QPointF right;
        mafNodeConnectorGraphicWidget* leftConn;
        mafNodeConnectorGraphicWidget* rightConn;
	if (a.x() < b.x()) {
		left = a;
		leftConn = mStartConnector;
		right = b;
		rightConn = mEndConnector;
	}
	else {
		left = b;
		leftConn = mEndConnector;
		right = a;
		rightConn = mStartConnector;
	}

	QPointF bottom;
	QPointF top;
        mafNodeConnectorGraphicWidget* bottomConn;
        mafNodeConnectorGraphicWidget* topConn;
	if (a.y() < b.y()) {
		top = a;
		topConn = mStartConnector;
		bottom = b;
		bottomConn = mEndConnector;	
	}
	else {
		top = b;
		topConn = mEndConnector;
		bottom = a;
		bottomConn = mStartConnector;
	}

	
	controlPoint1 = a;
	controlPoint2 = b;
	qreal moveX = 0.45 * diffx;
	qreal moveY = 0.45 * diffy;

	QSizeF combinedSize(30,30);
	if (leftConn->parentItem() != NULL) {
		combinedSize.setWidth(leftConn->parentItem()->boundingRect().width());
		combinedSize.setHeight(leftConn->parentItem()->boundingRect().height());
	}
	else {
		combinedSize.setWidth(5 * leftConn->mRadius);
		combinedSize.setWidth(5 * leftConn->mRadius);
	}
	if (rightConn->parentItem() != NULL) {
		combinedSize.setWidth(combinedSize.width() + rightConn->parentItem()->boundingRect().width());
		combinedSize.setHeight(combinedSize.height() + rightConn->parentItem()->boundingRect().height());
	}
	else {
		combinedSize.setWidth(combinedSize.width() + 5 * rightConn->mRadius);
		combinedSize.setHeight(combinedSize.height() + 5 * rightConn->mRadius);
	}
	
	

        if (leftConn->connectorAlignment() == rightConn->connectorAlignment() && leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Left && right.x() - left.x() < combinedSize.width()/2.0) {
		controlPoint1.setX(controlPoint1.x() - moveY/2.0 /*- combinedSize.width()/2.0*/);
		controlPoint2.setX(controlPoint2.x() - moveY/2.0 /*- combinedSize.width()/2.0*/);
	}
        else if (leftConn->connectorAlignment() == rightConn->connectorAlignment() && leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Right && right.x() - left.x() < combinedSize.width()/2.0) {
		controlPoint1.setX(controlPoint1.x() + moveY/2.0 /*+ combinedSize.width()/2.0*/);
		controlPoint2.setX(controlPoint2.x() + moveY/2.0 /*+ combinedSize.width()/2.0*/);
	}
        else if (leftConn->connectorAlignment() == rightConn->connectorAlignment() && leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Top && bottom.y() - top.y() < combinedSize.height()/2.0) {
		controlPoint1.setY(controlPoint1.y() - moveX/2.0 /*- combinedSize.height()/2.0*/);
		controlPoint2.setY(controlPoint2.y() - moveX/2.0 /*- combinedSize.height()/2.0*/);
	}
        else if (leftConn->connectorAlignment() == rightConn->connectorAlignment() && leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom && bottom.y() - top.y() < combinedSize.height()/2.0) {
		controlPoint1.setY(controlPoint1.y() + moveX/2.0 /*+ combinedSize.height()/2.0*/);
		controlPoint2.setY(controlPoint2.y() + moveX/2.0 /*+ combinedSize.height()/2.0*/);
	}
    else
        if (leftConn->connectorAlignment() != mafNodeConnectorGraphicWidget::Left && rightConn->connectorAlignment() != mafNodeConnectorGraphicWidget::Right
                        && topConn->connectorAlignment() != mafNodeConnectorGraphicWidget::Top && bottomConn->connectorAlignment() != mafNodeConnectorGraphicWidget::Bottom) {


		controlPoint1 = a;
		controlPoint2 = b;
        qreal moveX = 0.45 * diffx;
		qreal moveY = 0.45 * diffy;
		if (dist > 5 * (mStartConnector->mRadius + arrowSize)) {			
			if (moveX < 3 * (mStartConnector->mRadius + arrowSize)) {
				moveX = 3 * (mStartConnector->mRadius + arrowSize);
			}
			if (moveY < 3 * (mStartConnector->mRadius + arrowSize)) {
				moveY = 3 * (mStartConnector->mRadius + arrowSize);
			}
		}

                if (mStartConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Left) {
			controlPoint1.setX(controlPoint1.x() - moveX);
		}
                else if (mStartConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Right) {
			controlPoint1.setX(controlPoint1.x() + moveX);
		}
                else if (mStartConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom) {
			controlPoint1.setY(controlPoint1.y() + moveY);
		}
                else if (mStartConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Top) {
			controlPoint1.setY(controlPoint1.y() - moveY);
		}

                if (mEndConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Left) {
			controlPoint2.setX(controlPoint2.x() - moveX);
		}
                else if (mEndConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Right) {
			controlPoint2.setX(controlPoint2.x() + moveX);
		}
                else if (mEndConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom) {
			controlPoint2.setY(controlPoint2.y() + moveY);
		}
                else if (mEndConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Top) {
			controlPoint2.setY(controlPoint2.y() - moveY);
		}
	}
	else {
		controlPoint1 = a;
		controlPoint2 = b;
		qreal maxMove = 0.5 * dist;
		moveX = 0.5 * dist;
		moveY = 0.5 * dist;
		if (mStartConnector->parent != NULL) {
			maxMove = 1 * (mStartConnector->parent->rect().width() + mStartConnector->parent->rect().height());
		}
		else if (mEndConnector->parent != NULL) {
			maxMove = 1 * (mEndConnector->parent->rect().width() + mEndConnector->parent->rect().height());
		}
		if (moveX > maxMove) {
			moveX = maxMove + 0.1 * (moveX-maxMove);
		}
		if (moveY > maxMove) {
			moveY = maxMove + 0.1 * (moveY-maxMove);
		}
                if (mStartConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Left) {
			moveX *= -1;
			if ((mStartConnector == topConn) == (mStartConnector == rightConn)) {
				moveY *= -1;//relevantHeight;
			}
		}
                else if (mStartConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Right) {
			//moveX *= 1;
			if ((mStartConnector == topConn) == (mStartConnector == leftConn)) {
				moveY *= -1;//relevantHeight;
			}
		}
                else if (mStartConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom) {
			//moveY *= 1;
			if ((mStartConnector == leftConn) == (mStartConnector == topConn)) {
				moveX *= -1;
			}
		}
                else if (mStartConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Top) {
			moveY *= -1;
			if ((mStartConnector == leftConn) == (mStartConnector == bottomConn)) {
				moveX *= -1;
			}
		}

        if (mEndConnector == topConn && topConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Top && (bottomConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Left || bottomConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Right)) {
			moveY *= -1;
			//moveY = 0;
		}
                else if (mEndConnector == bottomConn && bottomConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom && (topConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Left || topConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Right)) {
			moveY *= -1;
			//moveY = 0;
		}
                else if (mEndConnector == leftConn && leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Left && (rightConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Top || rightConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom)) {
			moveX *= -1;
			//moveX = 0;
		}
                else if (mEndConnector == rightConn && rightConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Right && (leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Top || leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom)) {
			moveX *= -1;
			//moveX = 0;
		}

		controlPoint1.setX(controlPoint1.x() + moveX);
		controlPoint1.setY(controlPoint1.y() + moveY);


		moveX = 0.5 * dist;
		moveY = 0.5 * dist;
        if (mStartConnector->parent != NULL && mEndConnector->parent != NULL) {
			maxMove = 1 * (mEndConnector->parent->rect().width() + mEndConnector->parent->rect().height());
		}
		if (moveX > maxMove) {
			moveX = maxMove + 0.1 * (moveX-maxMove);
		}
		if (moveY > maxMove) {
			moveY = maxMove + 0.1 * (moveY-maxMove);
		}
                if (mEndConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Left) {
			moveX *= -1;
			if ((mEndConnector == topConn) == (mEndConnector == rightConn)) {
				moveY *= -1;//relevantHeight;
			}
		}
                else if (mEndConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Right) {
			//moveX *= 1;
			if ((mEndConnector == topConn) == (mEndConnector == leftConn)) {
				moveY *= -1;//relevantHeight;
			}
		}
                else if (mEndConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom) {
			//moveY *= 1;
			if ((mStartConnector == leftConn) == (mStartConnector == topConn)) {
				moveX *= -1;
			}
		}
                else if (mEndConnector->connectorAlignment() == mafNodeConnectorGraphicWidget::Top) {
			moveY *= -1;
			if ((mStartConnector == leftConn) == (mStartConnector == bottomConn)) {
				moveX *= -1;
			}
		}

        if (mStartConnector == topConn && topConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Top && (bottomConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Left || bottomConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Right)) {
			moveY *= -1;
			//moveY = 0;
		}
                else if (mStartConnector == bottomConn && bottomConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom && (topConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Left || topConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Right)) {
			moveY *= -1;
			//moveY = 0;
		}
                else if (mStartConnector == leftConn && leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Left && (rightConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Top || rightConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom)) {
			moveX *= -1;
			//moveX = 0;
		}
                else if (mStartConnector == rightConn && rightConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Right && (leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Top || leftConn->connectorAlignment() == mafNodeConnectorGraphicWidget::Bottom)) {
			moveX *= -1;
			//moveX = 0;
		}

		controlPoint2.setX(controlPoint2.x() + moveX);
		controlPoint2.setY(controlPoint2.y() + moveY);
	}


	QPainterPath p(a);
	p.cubicTo(controlPoint1, controlPoint2, b);
	this->setPath(p);

        update();
}

QPolygonF mafNodeConnectionGraphicWidget::createArrowPoly(QPainterPath& p, mafNodeConnectorGraphicWidget* conn) {
	float arrowStartPercentage;
	float arrowEndPercentage;
	
	if (conn == mEndConnector) {
		arrowStartPercentage = p.percentAtLength(p.length() - conn->mRadius - arrowSize);
		arrowEndPercentage = p.percentAtLength(p.length() - conn->mRadius);
	}
	else {
		arrowStartPercentage = p.percentAtLength(conn->mRadius + arrowSize);
		arrowEndPercentage = p.percentAtLength(conn->mRadius);
	}
	QPointF headStartP = p.pointAtPercent(arrowStartPercentage);
	QPointF headEndP = p.pointAtPercent(arrowEndPercentage);
	QLineF arrowMiddleLine(headStartP, headEndP);
	//QLineF normHead = arrowMiddleLine.normalVector();
	arrowMiddleLine.unitVector();
	QPointF normHead(arrowMiddleLine.dy(), -arrowMiddleLine.dx());
	QPointF arrowP1 = headStartP + normHead * 0.4;
	QPointF arrowP2 = headStartP - normHead * 0.4;

	QPolygonF arrowHeadEnd;
	arrowHeadEnd << headEndP << arrowP1 << arrowP2 /*<< headEndP*/;
	return arrowHeadEnd;
}


bool mafNodeConnectionGraphicWidget::setBidirectional(bool bidirectional) {
        mBidirectional =  bidirectional && mStartConnector->connectorType() == mafNodeConnectorGraphicWidget::InOut && mEndConnector->connectorType() == mafNodeConnectorGraphicWidget::InOut;
	return mBidirectional;
}
bool mafNodeConnectionGraphicWidget::bidirectional() {
	return mBidirectional;
}


void mafNodeConnectionGraphicWidget::debugPaint(QPainter *painter, QPointF& controlPoint1, QPointF& controlPoint2) {
	static int i = 0, j=0, k=0;

	QPen mPen = painter->pen();
	QColor ctrlPtCol(i=(i+19)%256 , j=(j+51)%256, k=(k+11)%256);
	mPen.setColor(ctrlPtCol);
	painter->setPen(mPen);

	//painter->drawPath(shape()); // to see item.
	painter->fillRect(boundingRect(), QColor(i=(i+19)%256 , j=(j+51)%256, k=(k+11)%256)); // to see item.

	//painter->setPen(Qt::NoPen);
    painter->setBrush(ctrlPtCol);

	//painter->drawPoint(controlPoint1);
	//painter->drawPoint(controlPoint2);
	painter->drawEllipse(controlPoint1.x()-2, controlPoint1.y()-2, 5, 5);
	painter->drawEllipse(controlPoint2.x()-2, controlPoint2.y()-2, 5, 5);
}


//! [4]
void mafNodeConnectionGraphicWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
	Q_UNUSED(option);
    Q_UNUSED(w);

	painter->setRenderHint(QPainter::Antialiasing);

	if (mStartConnector == NULL || mEndConnector == NULL || mStartConnector->collidesWithItem(mEndConnector))
        return;

	QPointF controlPoint1;
	QPointF controlPoint2;
	recreatePath(controlPoint1, controlPoint2);
	
    if (static_cast<mafDiagramScene*>(scene())->isDebugDraw()) {
		QPen origPen = painter->pen();
		QBrush origBrush = painter->brush();
		debugPaint(painter, controlPoint1, controlPoint2);
		painter->setPen(origPen);
	    painter->setBrush(origBrush);
	}

    QPen mPen = pen();
    mPen.setColor(mColor);
    painter->setPen(mPen);
	//line
    //painter->setBrush(mColor);
	painter->setBrush(Qt::NoBrush);

	if (isSelected())
		painter->setPen(QPen(mColor, 1, Qt::DashLine));


    QPainterPath p = this->path();
	painter->drawPath(p);

	//fill
	painter->setBrush(mColor);

    QPolygonF arrowHeadEnd = createArrowPoly(p, mEndConnector);
	if (bidirectional()) {
		QPolygonF arrowHeadStart = createArrowPoly(p, mStartConnector);
		p.addPolygon(arrowHeadStart);
		painter->drawPolygon(arrowHeadStart);
	}
    p.addPolygon(arrowHeadEnd);
	painter->drawPolygon(arrowHeadEnd);
	
}
