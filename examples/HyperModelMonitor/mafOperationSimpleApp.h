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

public:
    /// Object constructor.
    mafOperationSimpleApp(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    void setCheckScalar(bool s);

    bool checkScalar() const;

public slots:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();

    void on_textTest_textEdited(QString text);

    void on_checkScalar_clicked(bool state);

protected:
    /// Terminate the operation.
    /*virtual*/ void terminated();
private:
    bool m_CheckScalar;
};

inline bool mafOperationSimpleApp::checkScalar() const {
    return m_CheckScalar;
}

#endif // MAFOPERATIONSIMPLEAPP_H
