/*
 *  mafNodeGraphicWidget.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 11/02/13.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafNodeGraphicWidget.h"

using namespace mafGUI;

void mafNodeGraphicWidget::hide() {
    this->widget()->close();
}

void mafNodeGraphicWidget::deleted() {
    this->widget()->close();
    //delete this;
}

void mafNodeGraphicWidget::deleted(int result) {
    this->widget()->close();
}

mafNodeGraphicWidget::~mafNodeGraphicWidget() {
    removeWigetFromConnectors();
    //deleteConnections();
    if (scene() != NULL) {
        this->scene()->removeItem(this);
    }
}

int mafNodeGraphicWidget::type() const {
    return Type;
}

QList<mafNodeConnectorGraphicWidget *> &mafNodeGraphicWidget::connectors() {
    return m_Connectors;
}

mafNodeGraphicWidget::mafNodeGraphicWidget(QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene, Qt::WindowFlags wFlags) :
QGraphicsProxyWidget(parent, wFlags), mMaxRadius(1) {
    setCacheMode(DeviceCoordinateCache);

    mContextMenu = contextMenu;
    isMoving = false;

    setZValue(1);
    //param
    mNoResize = true;
    if (mNoResize) {
        mControlResizeHandles = true;
    } else {
        mControlResizeHandles = false;
    }

/*
    QPainterPath path;

	 //dw
	 //QGroupBox *groupBox = new QGroupBox("GrBox");
	 //QLabel *numberLabel = new QLabel("Telephone number");
	 //QLineEdit *numberEdit = new QLineEdit;
	 //QFormLayout *layout = new QFormLayout;
	 //layout->addRow(numberLabel, numberEdit);
	 //groupBox->setLayout(layout);
	 QFormLayout *layout2 = new QFormLayout;
	 //layout2->addRow(groupBox);

	 QLabel* out0 = new QLabel("out0");
	 out0->setAlignment(Qt::AlignRight);
	 QLabel* in0 = new QLabel("in0");
	 layout2->addRow(in0, out0);
	 QLabel* in1 = new QLabel("in1");
	 layout2->addRow(in1);
	 QLabel* in2 = new QLabel("in2");
	 layout2->addRow(in2);
	 QLabel* inout0 = new QLabel("inOut0");
	 inout0->setAlignment(Qt::AlignRight);
	 layout2->addRow(inout0);

	 //QSpinBox ( int minValue, int maxValue, int step = 1, QWidget * parent = 0, const char * name = 0 )
	 QSpinBox* spinBox = new QSpinBox();
	 spinBox->setMinimum(0);
	 spinBox->setMaximum(9);
	 layout2->addRow(spinBox);

	 QDialog *dialog1 = new QDialog();
	 //QFrame *dialog1 = new QFrame();
	 //QGroupBox* dialog1 = new QGroupBox("A QGroupBox");
	 dialog1->setLayout(layout2);
	 //dialog1->setLayout(groupBox);
	 //this->setWidget(groupBox);
	 //dw will this fix deletion not called on close?
	 dialog1->setAttribute(Qt::WA_DeleteOnClose);
	 this->setWidget(dialog1);

         addConnector(new mafNodeConnectorGraphicWidget(this, scene, out0, mafNodeConnectorGraphicWidget::Out, true));
         addConnector(new mafNodeConnectorGraphicWidget(this, scene, in0, mafNodeConnectorGraphicWidget::In));
         addConnector(new mafNodeConnectorGraphicWidget(this, scene, in1, mafNodeConnectorGraphicWidget::In));
         addConnector(new mafNodeConnectorGraphicWidget(this, scene, in2, mafNodeConnectorGraphicWidget::In));
         addConnector(new mafNodeConnectorGraphicWidget(this, scene, inout0, mafNodeConnectorGraphicWidget::InOut, true));

         addConnector(new mafNodeConnectorGraphicWidget(this, scene, spinBox, mafNodeConnectorGraphicWidget::InOut));
	 
	 //QGraphicsScene scene;
	 //QGraphicsProxyWidget *proxy = scene->addWidget(groupBox);
	 //QGraphicsView view(&scene);
	 //view.show();
*/

/*    setPolygon(mPolygon);*/
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    if (scene != NULL){
        scene->addItem(this);
    }
	 
}

QRectF mafNodeGraphicWidget::boundingRect() const {
    QRectF rec(QGraphicsProxyWidget::boundingRect());
    qreal extra = 3;//2 * (mMaxRadius) ;
    rec = rec.normalized().adjusted(-extra, 0, extra, 0);

    return rec;
}

QPainterPath mafNodeGraphicWidget::shape() const {
    QPainterPath p;// = QGraphicsProxyWidget::shape();
    QRectF rec(QGraphicsProxyWidget::boundingRect());
    p.addRect(rec);

    return p;
}

void mafNodeGraphicWidget::setWidget(QWidget *widget) {
    QGraphicsProxyWidget::setWidget(widget);
    connect(widget, SIGNAL(destroyed()), this, SLOT(deleted()));
}

void mafNodeGraphicWidget::addConnector(mafNodeConnectorGraphicWidget* nc) {
    connectors().append(nc);
    if (nc->mRadius > mMaxRadius) {
        mMaxRadius = nc->mRadius;
    }

    updateConnectorsPos();
    prepareGeometryChange();
}

void mafNodeGraphicWidget::deleteConnections() {
    foreach (mafNodeConnectorGraphicWidget *c, connectors()) {
        c->deleteConnections();
    }
}

void mafNodeGraphicWidget::removeWigetFromConnectors() {
    foreach (mafNodeConnectorGraphicWidget *c, connectors()) {
       c->removeWidget();
    }
}

void mafNodeGraphicWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    scene()->clearSelection();
    setSelected(true);
    if (mContextMenu != NULL) {
        mContextMenu->exec(event->screenPos());
    }
}

QVariant mafNodeGraphicWidget::itemChange(GraphicsItemChange change,
                     const QVariant &value) {
    if (change == QGraphicsItem::ItemPositionChange) {
        updateConnectorsPos();
    }
    if (change == QGraphicsItem::ItemVisibleHasChanged && !value.value<bool>()) {
        deleted();
        return value;
    }

   if (mControlResizeHandles && change == QGraphicsItem::ItemCursorChange) {
	   QCursor cur = qVariantValue<QCursor>(value);
           if (cur.shape() == Qt::SizeVerCursor || cur.shape() == Qt::SizeHorCursor || cur.shape() == Qt::SizeBDiagCursor || cur.shape() == Qt::SizeFDiagCursor) {
		   if (mNoResize) {
			   return Qt::ArrowCursor;
		   }
		   else {
                       foreach (mafNodeConnectorGraphicWidget *con, connectors()) {
					if (con->isUnderMouse()) {
						return Qt::ArrowCursor;
					}
				}
		   }
	   }
   }

   if (change == QGraphicsItem::ItemSelectedChange || change == QGraphicsItem::ItemTransformChange || change == QGraphicsItem::ItemScaleChange
	   || change == QGraphicsItem::ItemSendsGeometryChanges || change == QGraphicsItem::ItemMatrixChange) {
	   /*
                foreach (mafNodeConnectorGraphicWidget *con, connectors) {
			con->updatePositionGeometry();
		}
		*/
	   updateConnectorsPos();
   }
   if (change == QGraphicsItem::ItemPositionHasChanged || change == QGraphicsItem::ItemSelectedChange) {
	   /*
                foreach (mafNodeConnectorGraphicWidget *con, connectors) {
			con->updatePositionGeometry();
		}
		*/
	   updateConnectorsPos();
   }

    return QGraphicsProxyWidget::itemChange(change, value);
    //return value;
}

void mafNodeGraphicWidget::resizeEvent ( QGraphicsSceneResizeEvent * event ) {
		QGraphicsProxyWidget::resizeEvent(event);
		updateConnectorsPos();
}

void mafNodeGraphicWidget::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )  {
	event->ignore();
	return;
}

void mafNodeGraphicWidget::updateConnectorsPos() {
    foreach (mafNodeConnectorGraphicWidget *con, connectors()) {
		con->updatePosition();
    }
}


void mafNodeGraphicWidget::debugPaint(QPainter *painter) {
	static int i = 0, j=0, k=0;
	painter->fillRect(boundingRect(), /*Qt::green*/ QColor(i=(i+19)%256 , j=(j+51)%256, k=(k+11)%256)); // to see item.
}

void mafNodeGraphicWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w) {
    if (static_cast<mafDiagramScene*>(scene())->isDebugDraw()) {
        debugPaint(painter);
    }
    //updateConnectorsPos();

    QGraphicsProxyWidget::paint(painter, option, w);
    //QGraphicsItem::paint(painter, option, w);
    /*
    foreach (mafNodeConnectorGraphicWidget *c, connectors) {
        c->paint(painter, option, w);
        c->update();
    }
    */
}

const char* mafNodeGraphicWidget::shouldMoveOnClickTypes[] = {"QDialog", "QFrame", "QGroupBox"};

bool mafNodeGraphicWidget::shouldMoveNode(QGraphicsSceneMouseEvent *mouseEvent) {
	QPointF pos = mouseEvent->pos();
	QPointer<QWidget> hitWidget = widget()->childAt(pos.toPoint());
	if (hitWidget == NULL) {
		return true;
	}
	//foreach(QString t, shouldNotMoveTypes) {
	//	if (alienWidget->inherits(t.toStdString().c_str())) {
	const size_t len = sizeof(shouldMoveOnClickTypes) / sizeof(shouldMoveOnClickTypes[0]);
	for (size_t i = 0; i < len; ++i) {
		if (hitWidget->inherits(shouldMoveOnClickTypes[i])) {
			return true;
		}
	}
	return false;
}

void mafNodeGraphicWidget::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (shouldMoveNode(mouseEvent)) {
		QGraphicsItem::mousePressEvent(mouseEvent);
		isMoving = true;
		// what if we have to remove that?
		scene()->clearSelection();
		scene()->clearFocus();
		setSelected(true);
	}
	else {
		QGraphicsProxyWidget::mousePressEvent(mouseEvent);
	}
}

void mafNodeGraphicWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (isMoving) {
		QGraphicsItem::mouseMoveEvent(mouseEvent);
	}
	else {
		QGraphicsProxyWidget::mouseMoveEvent(mouseEvent);
	}
}

void mafNodeGraphicWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (isMoving) {
		isMoving = false;
	}

        QGraphicsItem::mouseReleaseEvent(mouseEvent);
	QGraphicsProxyWidget::mouseReleaseEvent(mouseEvent);
	scene()->clearSelection();
	setSelected(true);
}
