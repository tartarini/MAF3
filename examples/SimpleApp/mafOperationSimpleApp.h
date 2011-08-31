/*
 *  mafOperationSimpleApp.h
 *  SimpleApp
 *
 *  Created by Paolo Quadrani on 14/12/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOPERATIONSIMPLEAPP_H
#define MAFOPERATIONSIMPLEAPP_H

#include <mafOperation.h>

/**
  Class Name: mafOperationSimpleApp
  This class represent a test operation that will be plugged from the vertical application.
  */
class mafOperationSimpleApp : public mafResources::mafOperation {
    Q_OBJECT
    Q_PROPERTY(bool checkScalar READ checkScalar WRITE setCheckScalar)
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);

public slots:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();

    /// slot for editing text.
    void on_textTest_textEdited(QString text);

    /// slot for turn on/off scalar.
    void on_checkScalar_clicked(bool state);
    
public:
    /// Object constructor.
    mafOperationSimpleApp(const QString code_location = "");
    
    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
    
    /// set scalar flag.
    void setCheckScalar(bool s);
    
    /// return scalar flag.
    bool checkScalar() const;
    
protected:
    /// Terminate the operation's execution.
    /*virtual*/ void terminated();
    
private:
    bool m_CheckScalar; ///<  scalar flag.
};

/////////////////////////////////
/// inline functions
/////////////////////////////////

inline bool mafOperationSimpleApp::checkScalar() const {
    return m_CheckScalar;
}

#endif // MAFOPERATIONSIMPLEAPP_H
