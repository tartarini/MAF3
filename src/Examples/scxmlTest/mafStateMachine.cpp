#include "mafStateMachine.h"
#include <qscxml.h>

using namespace mafCore;

mafStateMachine::mafStateMachine(QScxml *state_machine, const QString code_location) : mafObjectBase(code_location), m_StateMachine(state_machine), m_Model(new mafModel()) {
    setObjectName("stateMachineLogic");
    m_Model->setObjectName("model");
}

mafStateMachine::~mafStateMachine() {
    delete m_Model;
}

void mafStateMachine::setModel(mafModel *m) {
    m_Model = m;
}

void mafStateMachine::setCurrentState(QString name) {
    qDebug() << "Current State: " << name;
}

void mafStateMachine::useModel() {
    qDebug() << "ip: " << m_Model->serviceIP();
    qDebug() << "port: " << m_Model->servicePort();
    qDebug() << "protocol: " << m_Model->serviceProtocol();
}
