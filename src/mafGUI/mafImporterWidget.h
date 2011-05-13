#ifndef MAFIMPORTERWIDGET_H
#define MAFIMPORTERWIDGET_H

#include "mafGUIDefinitions.h"
#include <QWidget>

namespace Ui {
    class mafImporterWidget;
}

class mafImporterWidget : public QWidget
{
    Q_OBJECT

public slots:
    void on_browseFileButton_clicked();

public:
    explicit mafImporterWidget(QWidget *parent = 0);
    ~mafImporterWidget();

private:
    Ui::mafImporterWidget *ui;
};

#endif // MAFIMPORTERWIDGET_H
