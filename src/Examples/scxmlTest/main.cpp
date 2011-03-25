#include <QtCore/QCoreApplication>
#include <qscxml.h>
#include "mafStateMachine.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QScxml *state_machine = QScxml::load("./testWorkflow.scxml");
    QObject::connect (state_machine, SIGNAL(finished()), &a, SLOT(quit()));

    mafStateMachine sm(state_machine);
    state_machine->registerObject(sm.model(), "");
    state_machine->registerObject(&sm, "");

    state_machine->start();
    return a.exec();
}
