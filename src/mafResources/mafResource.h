/*
 *  mafResource.h
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFRESOURCE_H
#define MAFRESOURCE_H

// Includes list
#include "mafResourcesDefinitions.h"
#include "mafMementoResource.h"

namespace mafResources {

// Class forwarding list

/**
Class name: mafResource
This class provides basic API to set input and get output from a mafResource.
*/
class MAFRESOURCESSHARED_EXPORT mafResource : public mafCore::mafObject {
    Q_OBJECT
    Q_PROPERTY(bool dataLoaded READ dataLoaded WRITE setDataLoaded)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public:
    /// Object constructor.
    mafResource(const QString code_location = "");

    /// Used in MAF3 design by contract to check object's validity.
    /** isObjectValid methods play an important role in checking the consistency
    of objects in the debug. isObjectValid is defined as a pure virtual function
    in Object class, thus it needs to be overridden in all inheriting classes.
    The inheriting class should perform defensive checks to make
    sure that it is in a consistent state/
    Also note that this method is only available in the debug build.*/
    /*virtual*/ bool isObjectValid() const;

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Set input of the resource.
    /** If the resource is already present inside the input list, setInput will return
    its index inside the list and will not add the object. If idx is inside the range
    of the input list, resource will replace the one contained at idx position.
    If idx is out of the range of the input list, resource will be appended as last
    item of the list */
    int setInput(mafResource *resource, const int idx = 0);

    /// Append a new resource to the inpiut list.
    int addInput(mafResource *resource);

    /// Remove the input at index 'idx'
    void removeInput(const int idx);

    /// Remove the the given input object from the list
    void removeInput(mafResource *resource);

    /// Remove all the inputs from the resource.
    void removeAllInputs();

    /// Get output of the resource.
    mafResource *output();

    /// Get a specific input of the resource.
    mafResource *input(const int idx = 0);

    /// Get the input list of the resource.
    mafResourceList *inputList();

    /// Return the instance of the resource's status. The caller has to delete the allocated memory he asked.
    /*virtual*/ mafCore::mafMemento *createMemento() const;

    /// Allows setting a previous saved object's state.
    /**
     This is used to implement a sort of undo mechanism for the object's state, but can be used also by the
    serialization mechanism to serialize data into the selected storage type.
    The 'deep_memento' flag is used to avoid the copy of the object unique hash in normal operation like
    undo or copy/paste operations. The complete object save is instead needed for serialization pourposes.*/
    /*virtual*/ void setMemento(mafCore::mafMemento *memento, bool deep_memento = false);

    /// Return true if data has been loaded.
    bool dataLoaded();

    /// Initialize the resource.
    virtual bool initialize();

signals:
    /// Start the execution of the resource.
    void startExecution();

    /// Notify that the resource execution is terminated.
    void executionEnded();

    /// Terminate the execution.
    void terminateExecution();

public slots:
    /// Set value of m_DataLoaded.
    void setDataLoaded(bool dataLoaded);

    /// Slot called when an input is destroyed outside. It has to be removed automatically from the input list.
    void inputDestroyed();

    /// Execute the resource algorithm.
    /**
        Resource emit the executionEnded signal at the end of the execute code so that the observer connected to it
        can finalize the execution mechanism. The subclassing mafResource and overriding the execute slot, at the end of
        the execution code the custom resource MUST call mafResource::execute() or emit the executionEnded signal.
    */
    virtual void execute();

    /// Terminate the execution.
    virtual void terminate(bool result);

protected:
    /// Object destructor.
    /* virtual */ ~mafResource();

    bool m_DataLoaded; ///< Indicates if data has been loaded.
    mafResource *m_Output; ///< Output of the resource.

private:
    mafResourceList *m_InputList; ///< Resource input List

};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafResourceList *mafResource::inputList() {
    return m_InputList;
}

inline mafResource *mafResource::output() {
    return m_Output;
}

inline bool mafResource::dataLoaded() {
    return m_DataLoaded;
}

} //mafResources

#endif // MAFRESOURCE_H
