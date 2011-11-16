/*
 *  mafPipe.h
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPE_H
#define MAFPIPE_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafVME;
class mafDataSet;

/**
 Class name: mafPipe
 This is the base class for the MAF3 pipes.
 */
class MAFRESOURCESSHARED_EXPORT mafPipe : public mafCore::mafObject {
    Q_OBJECT
    Q_PROPERTY(QVariantHash parametersHash READ parametersHash WRITE setParametersHash)

    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public:
     /// Object constructor.
    mafPipe(const QString code_location = "");

    /// Set the input VME to be used as input data for the pipeline./*
    void setInput(mafVME *vme);
    
    /// Return the input at the given index. Return NULL if the index passed is out of inputList range.
    mafVME *input(int idx = 0) const;
    
    /// Return the dataset for the given input index and at given timestamp.
    mafDataSet *dataSetForInput(int input_idx = 0, double t = -1);

    /// Remove the the given input object from the list
    void removeInput(mafVME *vme);

    /// Remove the the given at index 'idx'.
    void removeInput(const int idx);

    /// Get the input of the pipe.
    mafVMEList *inputList();

    /// Return the hash that contains parameters used to initialize the pipe algorithm.
    QVariantHash parametersHash() const;

    /// Assign the hash that contains parameters used to initialize the pipe algorithm.
    /** The hash contains key, value representing the name of the property (key) and the corresponding
    value of it (the value of the hash).*/
    void setParametersHash(const QVariantHash hash);

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    virtual void updatePipe(double t = -1) = 0;

protected:
    /// Object destructor.
    /* virtual */ ~mafPipe();

private:
    mafVMEList *m_InputList; ///< VME input List
    QVariantHash m_ParametersHash; ///< Hash containing the optional parameters used to initialize the pipe.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafVMEList *mafPipe::inputList() {
    return m_InputList;
}

inline QVariantHash mafPipe::parametersHash() const {
    return m_ParametersHash;
}

} //namespace mafResources

#endif // MAFPIPE_H
