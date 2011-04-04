/*
 *  mafAlgorithm.h
 *  serverXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFALGORITHM_H
#define MAFALGORITHM_H

#include <mafOperation.h>

/**
 Class name: mafAlgorithm
 This class provides a test operation executed through the xmlrpc call.
 */
class mafAlgorithm : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor
    mafAlgorithm(const QString code_location = "");

signals:
    /// Trigger the start execution of the operation.
    void startExecution();

public slots:
    /// set Parameters of the operation
    /* virtual */ void setParameters(QList<QVariant> parameters);

    /// Execute the resource algorithm.
    /*virtual*/ void execute();

private:
    QString m_FileNameInput;
    QString m_FileNameOutput;
    int m_IterationParameter;
};

#endif // MAFALGORITHM_H
