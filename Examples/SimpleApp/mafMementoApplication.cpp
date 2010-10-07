#include "mafMementoApplication.h"

using namespace mafCore;

mafMementoApplication::mafMementoApplication(const mafString code_location) : mafMemento(code_location) {
}

mafMementoApplication::mafMementoApplication(const QObject *obj, const mafString code_location) : mafMemento(code_location) {
    const QMetaObject* meta = obj->metaObject();
    setObjectClassType(meta->className());

    mafMementoPropertyList *list = mementoPropertyList();
}
