/*
 *  mafOperation.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOPERATION_H
#define MAFOPERATION_H

#include "mafResource.h"

namespace mafResources {

// Class forwarding list

/**
Class name: mafOperation
This class provides basic API for building elaboration algorithms for mafResources.
An operation takes as input one or more mafVMEs and generate as output a mafVME. The algorithm executed on the input data
is provided ad mafDataPipe. The mafOperation that manage the possibility to have the undo mechanism for the executed algorithm.
*/
class MAFRESOURCESSHARED_EXPORT mafOperation : public mafResource {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafResource);

public:
    /// Object constructor.
    mafOperation(const mafString code_location = "");

    /// Return true or false according to the unDo ability of the operation.
    bool canUnDo() const;

    /// Return the operation type (can be: mafOperationTypeImporter, mafOperationTypeExporter or mafOperationTypeOperation).
    mafOperationType operationType() const;

    /// Initialize the resource.
    /*virtual*/ bool initialize();

    /// Terminate the execution.
    /*virtual*/ bool terminate();

    /// check if the operation is running.
    bool isRunning() const;

signals:
    /// Trigger the set parameters of the operation.
    //void setParametersSignal(mafList<mafVariant> parameters);

public slots:
    /// Set parameters of operation.
    virtual void setParameters(mafList<mafVariant> parameters);

    /// Execute the resource algorithm.
    virtual bool execute();

    /// Allows to call the piece of algorithm that is needed to restore the previous state of the operation's execution.
    virtual void unDo();

    /// Allows to call the piece of algorithm that is needed to apply the operation again.
    virtual void reDo();

protected:
    bool m_IsRunning; ///< Flag that check if the operation is running, i.e. the execution is started
    bool m_CanUnDo; ///< Flag that store the unDo capability of the operation.
    mafOperationType m_OperationType; ///< Describe the operation type (mafOperationTypeImporter, mafOperationTypeExporter or mafOperationTypeOperation).
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////
inline bool mafOperation::isRunning() const{
    return m_IsRunning;
}

inline bool mafOperation::canUnDo() const {
    return m_CanUnDo;
}

inline mafOperationType mafOperation::operationType() const {
    return m_OperationType;
}

} // namespace mafResources

#endif // MAFOPERATION_H
