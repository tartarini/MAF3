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

    Q_PROPERTY(QString inputFileName READ inputFileName WRITE setInputFileName)
    Q_PROPERTY(QString outputFileName READ outputFileName WRITE setOutputFileName)
    Q_PROPERTY(int iterations READ iterations WRITE setIterations)

public Q_SLOTS:    
    /// Execute the resource algorithm.
    /*virtual*/ void execute();
    
Q_SIGNALS:
    /// Trigger the start execution of the operation.
    void startExecution();

public:
    /// Object constructor
    mafAlgorithm(const QString code_location = "");

    /// return input file name.
    QString inputFileName() const;

    /// return output file name.
    QString outputFileName() const;
    
    /// return number of iterations.
    int iterations() const;
    
    /// set input file name.
    void setInputFileName(const QString &f);

    /// set output file name.
    void setOutputFileName(const QString &f);
    
    /// set number of iterations.
    void setIterations(const int i);
    
protected:
    /// Terminate the operation.
    /*virtual*/ void terminated();
    
private:
    QString m_FileNameInput;
    QString m_FileNameOutput;
    int m_IterationParameter;
};

#endif // MAFALGORITHM_H
