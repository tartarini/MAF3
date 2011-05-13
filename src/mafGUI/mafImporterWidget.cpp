#include "mafImporterWidget.h"
#include "ui_mafImporterWidget.h"

#include <QFileDialog>

mafImporterWidget::mafImporterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mafImporterWidget)
{
    ui->setupUi(this);
}

mafImporterWidget::~mafImporterWidget()
{
    delete ui;
}

void mafImporterWidget::setOperation(mafCore::mafObjectBase *op) {
    m_Operation = op;
}



void mafImporterWidget::setOperationGUI(QWidget *gui) {
    if(gui != NULL) {
        // Then connect automatically the signals of the GUI widgets to the Operation's slots.
        m_Operation->connectObjectSlotsByName((QObject *)gui);
        // and initialize the widgets value with those contained into the operation's properties.
        m_Operation->initializeUI((QObject *)gui);
        // do the same thing for all the the operations'children
        QObjectList children = m_Operation->children();
        foreach(QObject *child, children) {
            mafCore::mafObjectBase *obj = qobject_cast<mafCore::mafObjectBase *>(child);
            if(obj) {
                obj->connectObjectSlotsByName((QObject *)gui);
                obj->initializeUI((QObject *)gui);
            }
        }
    }
}


void mafImporterWidget::on_browseFileButton_clicked() {
    QString w = m_Operation->property("wildcard").toString();
    if(w.isEmpty()) {
        w = mafTr("All Files (*)");
    }
        
    //open dialog for selecting the name of the session
    QFileDialog::Options options;
//    if (!native->isChecked())
//        options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QStringList files = QFileDialog::getOpenFileNames(
                                                      NULL, 
                                                      mafTr("Import Data"),
                                                      ".",
                                                      w,
                                                      &selectedFilter,
                                                      options);
    m_Operation->setProperty("filename", files[0]);
    ui->filePathLineEdit->setText(files[0]);
}
