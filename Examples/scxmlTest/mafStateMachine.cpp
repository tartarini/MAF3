#include "mafStateMachine.h"
#include <qscxml.h>

using namespace mafCore;

mafStateMachine::mafStateMachine(QScxml *state_machine, const mafString code_location) : mafObjectBase(code_location), m_StateMachine(state_machine), m_Model(new mafModel()) {
    setObjectName("stateMachineLogic");
    m_Model->setObjectName("model");
}

mafStateMachine::~mafStateMachine() {
    delete m_Model;
}

void mafStateMachine::setModel(mafModel *m) {
    m_Model = m;
}

void mafStateMachine::setCurrentState(mafString name) {
    mafMsgDebug() << "Current State: " << name;
}

void mafStateMachine::useModel() {
    mafMsgDebug() << "ip: " << m_Model->serviceIP();
    mafMsgDebug() << "port: " << m_Model->servicePort();
    mafMsgDebug() << "protocol: " << m_Model->serviceProtocol();
}
