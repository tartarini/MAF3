#ifndef MAFSTATEMACHINE_H
#define MAFSTATEMACHINE_H

// Includes list
#include "mafObjectBase.h"
#include "mafModel.h"

// Class forwarding list
class QScxml;

/**
 Class name: mafStateMachine
 This class provides a logic call for the state machine written into the scxml file.
 */
class mafStateMachine : public mafCore::mafObjectBase  {
    Q_OBJECT
    Q_PROPERTY(QString currentstate READ currentState WRITE setCurrentState)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafStateMachine(QScxml *state_machine, const QString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafStateMachine();

    /// Assign the model class to the state machine logic.
    void setModel(mafModel *m);

    /// Return the current model associated with the state machine logic.
    mafModel *model() {return m_Model;}

    /// Set the current executed state name.
    void setCurrentState(QString name);

    /// Return the current executed state name.
    QString currentState();

public slots:
    /// Slot that use the data written inside the model.
    void useModel();

private:
    QScxml *m_StateMachine; ///< Reference to the scripting state machine.
    QString m_CurrentState; ///< Reference to the current executed state.
    mafModel *m_Model; ///< Reference to the current model.
};

inline QString mafStateMachine::currentState() {
    return m_CurrentState;
}

#endif // MAFSTATEMACHINE_H
