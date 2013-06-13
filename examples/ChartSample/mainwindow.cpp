#include <QtGui>
#include <QLabel>

#include "mainwindow.h"
#include "stylesheeteditor.h"
#include "windowflagseditor.h"


const int InsertTextButton = 10;
const int InsertNodeButton = 11;
const int InsertNode2Button = 12;

using namespace mafGUI;

MainWindow::MainWindow()
{
    scene = new ExampleDiagramScene(itemMenu);
    createActions();
    createToolBox();
    createMenus();

    scene->setMenu(itemMenu);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(nodeItemInserted(mafNodeGraphicWidget *)),
            this, SLOT(itemInserted(mafNodeGraphicWidget *)));

    //connect(scene, SIGNAL(itemSelected(QGraphicsItem *)),
    //    this, SLOT(itemSelected(QGraphicsItem *)));
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);

    view = new QGraphicsView(scene);
    //view = new MyFasterGraphicView(scene);
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    // add the canvas
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("NodeEditorScene"));

    //this->currentWindowFlags = Qt::CustomizeWindowHint | /*Qt::WindowType::MSWindowsFixedSizeDialogHint | Qt::WindowType::WindowTitleHint*/ Qt::WindowType::Tool;
}

void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button) {
    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *mButton, buttons) {
    if (mButton != button)
        button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Blue Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("White Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("Gray Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

    scene->update();
    view->update();
}

void MainWindow::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
    if (buttonGroup->button(id) != button)
        button->setChecked(false);
    }
	/*
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
	}
	else if (id == InsertNode2Button) {
		//scene->setItemType(DiagramItem::DiagramType::Node);
        scene->setMode(DiagramScene::InsertNode2);
    } else {
        //scene->setItemType(DiagramItem::DiagramType(id));
        //scene->setMode(DiagramScene::InsertItem);
    }*/

    this->scene->mode = id;
}

void MainWindow::deleteItem()
{
	if (scene->selectedItems().count()) {
		foreach (QGraphicsItem *item, scene->selectedItems()) {
			/*//now in dtor: 
			if (item->type() == DiagramItem::Type) {
				qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
			}

			//dw isn't it better to delete stuff in destructors??
			if (item->type() == mafNodeGraphicWidget::Type) {
				qgraphicsitem_cast<mafNodeGraphicWidget *>(item)->removeConnections();
			}
			*/

			//scene->removeItem(item);

			//can we delete all item or better only ours, and why is it not deleted automatically on removal? what is the correct way?
			delete item;
		}
		scene->clearSelection();
		scene->clearFocus();
	}
	else if (this->scene->focusItem() && this->scene->focusItem()->type() == mafNodeGraphicWidget::Type) {
        mafNodeGraphicWidget* item = qgraphicsitem_cast<mafNodeGraphicWidget *>(this->scene->focusItem());
		//now in dtor: item->removeConnections();

		//scene->removeItem(item);
		delete item;
		scene->clearSelection();
		scene->clearFocus();
    }
	
}

void MainWindow::pointerGroupClicked(int)
{
    scene->setMode(mafDiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        /*if (item->zValue() >= zValue &&
            item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;*/
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        /*if (item->zValue() <= zValue &&
            item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;*/
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::itemInserted(mafNodeGraphicWidget *item) {
    pointerTypeGroup->button(int(mafDiagramScene::MoveItem))->setChecked(true);
    scene->setMode(mafDiagramScene::Mode(pointerTypeGroup->checkedId()));
    //buttonGroup->button(int(item->diagramType()))->setChecked(false);

    //caused crashes
    //buttonGroup->button(InsertNode2Button)->setChecked(false);
	
    //note: can add widget later!! wrong location here
    if (item->widget() != NULL) {
        //QPointF p = item->pos();
        item->setWindowFlags(currentWindowFlags);
        item->widget()->setStyleSheet(currentStylesheet);
        QPointF p2 = item->pos();
        item->setVisible(true);
        //item->setPos(p);
        item->widget()->adjustSize();
        item->widget()->updateGeometry();
    }
}

void MainWindow::sceneScaleChanged(const QString &scale) {
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}

void MainWindow::lineColorChanged() {
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
    ":/images/linecolor.png",
    qVariantValue<QColor>(lineAction->data())));
    lineButtonTriggered();
}

void MainWindow::lineButtonTriggered() {
    scene->setLineColor(qVariantValue<QColor>(lineAction->data()));
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About NodeEditor Scene"),
                       tr("The <b>NodeEditor Scene</b> example shows "
                          "use of the graphics framework."));
}

void MainWindow::createToolBox() {
    buttonGroup = new QButtonGroup;
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
/*    layout->addWidget(createCellWidget(tr("Conditional"),
                               DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Process"),
                      DiagramItem::Step),0, 1);
    layout->addWidget(createCellWidget(tr("Input/Output"),
                      DiagramItem::Io), 1, 0);
					  */
    //layout->addWidget(createCellWidget(tr("Node"),
              //DiagramItem::Node), 2, 0);

    //mafNodeGraphicWidget item(mafNodeGraphicWidget::Node, itemMenu);
    /*
    QToolButton *nodeButton = new QToolButton;
    nodeButton->setCheckable(true);
    buttonGroup->addButton(nodeButton, InsertNodeButton);
    nodeButton->setIcon(QIcon(QPixmap(":/images/createnode1.png")
                        .scaled(30, 30)));
    nodeButton->setIconSize(QSize(50, 50));
    QGridLayout *nodeLayout = new QGridLayout;
    nodeLayout->addWidget(nodeButton, 0, 0, Qt::AlignHCenter);
    nodeLayout->addWidget(new QLabel(tr("Node")), 1, 0, Qt::AlignCenter);
    QWidget *nodeWidget = new QWidget;
    nodeWidget->setLayout(nodeLayout);
    layout->addWidget(nodeWidget, 2, 0);
    */

    QToolButton *node1Button = new QToolButton;
    node1Button->setCheckable(true);
    buttonGroup->addButton(node1Button, 1);
    node1Button->setText(tr("dialog, single in, only one connection"));

    QToolButton *node2Button = new QToolButton;
    node2Button->setCheckable(true);
    buttonGroup->addButton(node2Button, 2);
    node2Button->setText(tr("groupBox, single in, multi connection"));

    QToolButton *node5Button = new QToolButton;
    node5Button->setCheckable(true);
    buttonGroup->addButton(node5Button, 5);
    node5Button->setText(tr("dialog, single in, only one connection"));

    QToolButton *node6Button = new QToolButton;
    node6Button->setCheckable(true);
    buttonGroup->addButton(node6Button, 6);
    node6Button->setText(tr("groupBox, 2x inout, multi connection"));

    QToolButton *node7Button = new QToolButton;
    node7Button->setCheckable(true);
    buttonGroup->addButton(node7Button, 7);
    node7Button->setText(tr("frame with title, 1 x out, multi connection"));

    QToolButton *node8Button = new QToolButton;
    node8Button->setCheckable(true);
    buttonGroup->addButton(node8Button, 8);
    node8Button->setText(tr("only groupBox"));

    layout->addWidget(node1Button);
    layout->addWidget(node2Button);
    layout->addWidget(node5Button);
    layout->addWidget(node6Button);
    layout->addWidget(node7Button);
    layout->addWidget(node8Button);

/*
    QGridLayout *node2Layout = new QGridLayout;
    node2Layout->addWidget(node2Button, 0, 0, Qt::AlignHCenter);
    //node2Layout->addWidget(new QLabel(tr("Node2")), 1, 0, Qt::AlignCenter);
    QWidget *node2Widget = new QWidget;
    node2Widget->setLayout(node2Layout);
    layout->addWidget(node2Widget, 2, 1);
*/

/*
    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")
                        .scaled(30, 30)));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 1, 1);
*/
    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup;
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton *)),
            this, SLOT(backgroundButtonGroupClicked(QAbstractButton *)));

    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
                ":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
                ":/images/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                    ":/images/background3.png"), 0, 2);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                ":/images/background4.png"), 0, 3);

    //backgroundLayout->setRowStretch(2, 10);
    //backgroundLayout->setColumnStretch(2, 10);

    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);

    //toolBox = new QFrame;
    toolBox = new QScrollArea;
    QFrame* frame = new QFrame;
    toolBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //QScrollArea *scroll = new QScrollArea;
    QVBoxLayout* tbLayout = new QVBoxLayout;
	
    //toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));
    frame->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	
    QGroupBox* gBox = new QGroupBox(tr("Basic Flowchart Shapes"));
    QVBoxLayout* boxLayout = new QVBoxLayout;

    boxLayout->addWidget(itemWidget);
    gBox->setLayout(boxLayout);
    tbLayout->addWidget(gBox/*, tr("Basic Flowchart Shapes")*/);

    gBox = new QGroupBox(tr("Backgrounds"));
    boxLayout = new QVBoxLayout;
    boxLayout->addWidget(backgroundWidget);
    gBox->setLayout(boxLayout);
    tbLayout->addWidget(gBox/*, tr("Backgrounds")*/);

    StyleSheetEditor* styleSheetEditor = new StyleSheetEditor;
    /*QMenu* qssMenu = menuBar()->addMenu(tr("&Style"));
    QAction* styleAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Edit &Style"), this);
    styleAction->setShortcut(tr("Ctrl+Y"));
    styleAction->setStatusTip(tr("Edit Style"));
	qssMenu->addAction(styleAction);*/
    //connect(styleAction, SIGNAL(triggered()),
    //        styleSheetEditor, SLOT(show()));
    connect(styleSheetEditor, SIGNAL(newStyle(const QString&)),
                        this, SLOT(setNodeGraphicWidgetsStyle(const QString&)));
    connect(styleSheetEditor, SIGNAL(newStylesheet(const QString&)),
                        this, SLOT(setNodeGraphicWidgetsStylesheet(const QString&)));
    //tbLayout->addWidget(styleSheetEditor/*, tr("Style/Stylesheet Editor")*/);

    gBox = new QGroupBox(tr("Style/Stylesheet Editor"));
    boxLayout = new QVBoxLayout;
    boxLayout->addWidget(styleSheetEditor);
    gBox->setLayout(boxLayout);
    tbLayout->addWidget(gBox);

    ///////////////////////// windowflags editor
    WindowFlagsEditor* windowFlagsEditor = new WindowFlagsEditor;
    /*QMenu* flagsMenu = menuBar()->addMenu(tr("&WindowFlags"));
    QAction* flagsAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Edit &Style"), this);
    flagsAction->setShortcut(tr("Ctrl+F"));
    flagsAction->setStatusTip(tr("Edit WindowFlags"));
    flagsMenu->addAction(flagsAction);
    connect(flagsAction, SIGNAL(triggered()),
            windowFlagsEditor, SLOT(show()));*/
    connect(windowFlagsEditor, SIGNAL(newWindowFlags(Qt::WindowFlags)),
			this, SLOT(setNoteItemsWindowFlags(Qt::WindowFlags)));
    //tbLayout->addWidget(windowFlagsEditor/*, tr("WindowFlags Editor")*/);

    gBox = new QGroupBox(tr("WindowFlags Editor"));
    boxLayout = new QVBoxLayout;
    boxLayout->addWidget(windowFlagsEditor);
    gBox->setLayout(boxLayout);
    tbLayout->addWidget(gBox);

    //frame->setMinimumWidth(styleSheetEditor->sizeHint().width() + 40);
    //toolBox->setMinimumWidth(frame->sizeHint().width());
    toolBox->setMinimumWidth(styleSheetEditor->sizeHint().width() + 40);

    //toolBox->setLayout(tbLayout);
    frame->setLayout(tbLayout);
    tbLayout->setMargin(0);
    toolBox->setWidget(frame);
}

void MainWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()),
            this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"),
                                 tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+B"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()),
        this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/images/delete.png"),
                               tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()),
        this, SLOT(deleteItem()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    exitAction->setStatusTip(tr("Quit NodeEditor example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));

    fileNewAction = new QAction(tr("&New"), this);
    fileNewAction->setShortcut(tr("Ctrl+N"));
    connect(fileNewAction, SIGNAL(triggered()),
            this, SLOT(fileNew()));

    fileSaveAction = new QAction(tr("&Save"), this);
    fileSaveAction->setShortcut(tr("Ctrl+S"));
    connect(fileSaveAction, SIGNAL(triggered()),
            this, SLOT(fileSave()));

    fileOpenAction = new QAction(tr("&Open"), this);
    fileOpenAction->setShortcut(tr("Ctrl+O"));
    connect(fileOpenAction, SIGNAL(triggered()),
            this, SLOT(fileOpen()));
}

void MainWindow::fileNew() {
	scene->clear();
}

void MainWindow::fileSave() {
	QString fileName = QFileDialog::getSaveFileName(this,
     tr("Save Node Diagram"), "./", tr("Diagram Files (*.diagram)"));

	QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

    QTextStream out(&f);

	foreach( QGraphicsItem* i, scene->items()) {
                //if (i->inherits("mafNodeAdvancedGraphicWidget")) {
		if (i->type() == mafNodeGraphicWidget::Type) {
                        mafNodeAdvancedGraphicWidget* n = static_cast<mafNodeAdvancedGraphicWidget*>(i);
			n->serialize(out);
		}
	}
}

void MainWindow::fileOpen() {
	QString fileName = QFileDialog::getOpenFileName(this,
     tr("Open Node Diagram"), "./", tr("Diagram Files (*.diagram)"));

	QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	//clear old scene
    scene->clear();
	/*//this gives problems with connections, which are deleted by nodes!
    QList<QGraphicsItem *> list = scene->items();
    QList<QGraphicsItem *>::Iterator it = list.begin();
    for ( ; it != list.end(); ++it )
    {
		QGraphicsItem* gi = (*it);
		if ( (*it) && (*it)->parentItem() == 0 && (*it)->type() == mafNodeGraphicWidget::Type)
            {
					//mafNodeGraphicWidget* node = static_cast<mafNodeGraphicWidget*>(*it)
                    scene->removeItem(*it);
                    delete *it;
            }
    }
	*/


    QTextStream in(&f);
        QMap<int, mafNodeAdvancedGraphicWidget*> map ;

	while (!in.atEnd()) {
		QString l = in.readLine();
		int type = l.split("=")[1].toInt();
                mafNodeAdvancedGraphicWidget* n = scene->createNode(type);
		n->deserialize(in, map);
	}
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileNewAction);
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    /*
    StyleSheetEditor* styleSheetEditor = new StyleSheetEditor(this);
    QMenu* qssMenu = menuBar()->addMenu(tr("&Style"));
    QAction* styleAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Edit &Style"), this);
    styleAction->setShortcut(tr("Ctrl+Y"));
    styleAction->setStatusTip(tr("Edit Style"));
	qssMenu->addAction(styleAction);
    connect(styleAction, SIGNAL(triggered()),
            styleSheetEditor, SLOT(show()));
    connect(styleSheetEditor, SIGNAL(newStyle(const QString&)),
                        this, SLOT(setNodeGraphicWidgetsStyle(const QString&)));
    connect(styleSheetEditor, SIGNAL(newStylesheet(const QString&)),
                        this, SLOT(setNodeGraphicWidgetsStylesheet(const QString&)));

    WindowFlagsEditor* windowFlagsEditor = new WindowFlagsEditor(this);
    QMenu* flagsMenu = menuBar()->addMenu(tr("&WindowFlags"));
    QAction* flagsAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Edit &Style"), this);
    flagsAction->setShortcut(tr("Ctrl+F"));
    flagsAction->setStatusTip(tr("Edit WindowFlags"));
    flagsMenu->addAction(flagsAction);
    connect(flagsAction, SIGNAL(triggered()),
            windowFlagsEditor, SLOT(show()));
    connect(windowFlagsEditor, SIGNAL(newWindowFlags(Qt::WindowFlags)),
			this, SLOT(setNoteItemsWindowFlags(Qt::WindowFlags)));
    */

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}

void  MainWindow::setNodeGraphicWidgetsStyle(const QString &styleName) {
	/*
foreach( QGraphicsItem* i, scene->items()) {
        if (i->type() == mafNodeGraphicWidget::Type) {
                mafNodeAdvancedGraphicWidget* n = static_cast<mafNodeAdvancedGraphicWidget*>(i);
                n->widget()->setStyle(styleName);
        }
    }
    */
    qApp->setStyle(styleName);
}

void MainWindow::setNodeGraphicWidgetsStylesheet(const QString &stylesheetName)  {
	currentStylesheet = stylesheetName;
	foreach( QGraphicsItem* i, scene->items()) {
                //if (i->inherits("mafNodeAdvancedGraphicWidget")) {
		if (i->type() == mafNodeGraphicWidget::Type) {
                        mafNodeAdvancedGraphicWidget* n = static_cast<mafNodeAdvancedGraphicWidget*>(i);
			n->widget()->setStyleSheet(stylesheetName);
			n->update();
		}
	}
}

void MainWindow::setNoteItemsWindowFlags(Qt::WindowFlags flags) {
	currentWindowFlags = flags;
	foreach( QGraphicsItem* i, scene->items()) {
		if (i->type() == mafNodeGraphicWidget::Type) {
                        mafNodeAdvancedGraphicWidget* n = static_cast<mafNodeAdvancedGraphicWidget*>(i);
			/*
			If the widget had type Qt::Widget or Qt::SubWindow and becomes a window (Qt::Window, Qt::Dialog, etc.),
			it is put at position (0, 0) on the desktop. If the widget is a window and becomes a Qt::Widget or Qt::SubWindow,
			it is put at position (0, 0) relative to its parent widget.
			*/
			n->setWindowFlags(flags);
			n->setVisible(true);
			n->update();
			n->widget()->adjustSize();
		}
	}
}

void MainWindow::createToolbars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    /*
    fontCombo = new QFontComboBox();
    fontSizeCombo = new QComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)),
            this, SLOT(currentFontChanged(const QFont &)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(fontSizeChanged(const QString &)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()),
                                                 Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(
    ":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()),
                         Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
    ":/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));
*/

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()),
                                 Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
        ":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

    /*
    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
    */

    colorToolBar = addToolBar(tr("Color"));
    //colorToolBar->addWidget(fontColorToolButton);
    //colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup;
    pointerTypeGroup->addButton(pointerButton, int(mafDiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton,
                                int(mafDiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(sceneScaleChanged(const QString &)));

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);


    debugDrawCheckbox = new QCheckBox("DebugDraw");
    connect(debugDrawCheckbox, SIGNAL(clicked(bool)),
                static_cast<mafDiagramScene*>(scene), SLOT(setDebugDraw(bool)));

    /*connect(debugDrawCheckbox, SIGNAL(stateChanged(int state)),
		static_cast<DiagramScene*>(scene), SLOT(setDebugDraw(bool enabled)));*/

	QToolBar* debugToolbar = addToolBar(tr("Debug Draw"));
	debugToolbar->addWidget(debugDrawCheckbox);
}

QWidget *MainWindow::createBackgroundCellWidget(const QString &text,
                        const QString &image) {
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

/*
QWidget *MainWindow::createCellWidget(const QString &text,
                      DiagramItem::DiagramType type)
{

    DiagramItem item(type, itemMenu);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
*/

QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor) {
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu;
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()),
                this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}

QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile,
                        QColor color) {
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}

QIcon MainWindow::createColorIcon(QColor color) {
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
