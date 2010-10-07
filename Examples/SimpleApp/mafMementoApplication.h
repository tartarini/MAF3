#ifndef MAFMEMENTOAPPLICATION_H
#define MAFMEMENTOAPPLICATION_H

// Includes list
#include <mafMemento.h>

// Class forwarding list

/**
 Class name: mafMementoApplication
 This class allows to store the status of the application's settings.
 */
class mafMementoApplication : public mafCore::mafMemento {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMemento);

public:
    /// Memento constructor.
    mafMementoApplication(const mafString code_location = "");

    /// Memento constructor.
    mafMementoApplication(const QObject *obj, const mafString code_location = "");
};

#endif // MAFMEMENTOAPPLICATION_H
