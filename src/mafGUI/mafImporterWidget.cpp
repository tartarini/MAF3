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

void mafImporterWidget::on_browseFileButton_clicked() {
     //open dialog for selecting the name of the session
    QFileDialog::Options options;
//    if (!native->isChecked())
//        options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QStringList files = QFileDialog::getOpenFileNames(
                                                      NULL, tr("QFileDialog::getOpenFileNames()"),
                                                      "",
                                                      mafTr("All Files (*);;Text Files (*.xmsf)"),
                                                      &selectedFilter,
                                                      options);
//    if (files.count()) {
//        openFilesPath = files[0];
//        openFileNamesLabel->setText(QString("[%1]").arg(files.join(", ")));
//    }
    qDebug() << files[0];
}
