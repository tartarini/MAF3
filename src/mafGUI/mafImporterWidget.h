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
    
    /// Assign the current operation
    void setOperation(mafCore::mafObjectBase *op);
    
    /// Assign the operation's GUI
    void setOperationGUI(QWidget *gui);
    
    /// set the dialog type for operation (importer, exporter)
    void setDialogType(mafGUI::mafDialogType dialogType);
        
private:
    Ui::mafImporterWidget *ui;
    mafCore::mafObjectBase *m_Operation;   ///< Pointer to the current running operation.
    QWidget *m_OperationGUI; ///< GUI associated to m_Operation.
    mafGUI::mafDialogType m_DialogType;
};

#endif // MAFIMPORTERWIDGET_H
