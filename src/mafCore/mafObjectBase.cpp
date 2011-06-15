/*
 *  mafObjectBase.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#include "mafObjectBase.h"
#include "mafIdProvider.h"
#include "mafObjectRegistry.h"
#include "mafVisitor.h"

using namespace mafCore;

mafObjectBase::mafObjectBase(const QString code_location) : QObject(), m_UIFilename(""), m_Modified(false), m_ReferenceCount(1) {
    mafIdProvider *provider = mafIdProvider::instance();
    m_ObjectId = provider->createNewId();

    mafObjectRegistry::instance()->addObject(this, code_location);

    m_ObjectHash = QUuid::createUuid();
    
    connect(this, SIGNAL(incrementReference()), this, SLOT(ref()), Qt::DirectConnection);
    connect(this, SIGNAL(decreaseReference()), this, SLOT(deleteObject()), Qt::DirectConnection);
}

mafObjectBase::~mafObjectBase() {
    // need to anticipate the 'destroyed()' signal from QObject
    // to allws mafObjectRegitry to remove it from the hash, otherwise
    // if the signal is sent from the parent QObject, this subclas has
    // been already destroyed and its m_ObjectId is not more valid.
    mafObjectRegistry::instance()->removeObject(m_ObjectId);
    m_ObjectId = -1;
}

void mafObjectBase::retain() {
    emit incrementReference();
}

void mafObjectBase::release() {
    emit decreaseReference();
}

void mafObjectBase::setModified(bool m) {
    m_Modified = m;
}

bool mafObjectBase::isEqual(const mafObjectBase *obj) const {
    const QMetaObject* my_meta = metaObject();

    const QMetaObject* obj_meta = obj->metaObject();
    int i = 0;
    int num = obj_meta->propertyCount();
    if(num != my_meta->propertyCount()) {
        return false;
    }
    for ( ; i < num; ++i) {
        const QMetaProperty obj_qmp = obj_meta->property(i);
        const QMetaProperty my_qmp = my_meta->property(i);
        QString obj_name = obj_qmp.name();
        if(obj_name == "objectHash") {
            continue;
        }
        QVariant obj_value = obj->property(obj_name.toAscii());
        QString my_name = my_qmp.name();
        QVariant my_value = property(my_name.toAscii());
        char *n = my_name.toAscii().data();
        if((my_name != obj_name) || (my_value != obj_value)) {
            return false;
        }
    }

    return true;
}

bool mafObjectBase::isObjectValid() const {
    return m_ObjectId != -1;
}

void mafObjectBase::acceptVisitor(mafVisitor *v) {
    // Design by contract condition.
    REQUIRE(v != NULL);

    v->visit(this);
}

void mafObjectBase::connectObjectSlotsByName(QObject *signal_object) {
    const QMetaObject *mo = this->metaObject();
    Q_ASSERT(mo);
    const QObjectList list = qFindChildren<QObject *>(signal_object, QString());
    for (int i = 0; i < mo->methodCount(); ++i) {
        QMetaMethod method_slot = mo->method(i);
        if (method_slot.methodType() != QMetaMethod::Slot)
            continue;
        const char *slot = mo->method(i).signature();
        if (slot[0] != 'o' || slot[1] != 'n' || slot[2] != '_')
            continue;
        bool foundIt = false;
        for(int j = 0; j < list.count(); ++j) {
            const QObject *co = list.at(j);
            QByteArray objName = co->objectName().toAscii();
            int len = objName.length();
            if (!len || qstrncmp(slot + 3, objName.data(), len) || slot[len+3] != '_')
                continue;
            int sigIndex = -1; //co->metaObject()->signalIndex(slot + len + 4);
            const QMetaObject *smo = co->metaObject();
            if (sigIndex < 0) { // search for compatible signals
                int slotlen = qstrlen(slot + len + 4) - 1;
                for (int k = 0; k < co->metaObject()->methodCount(); ++k) {
                    QMetaMethod method = smo->method(k);
                    if (method.methodType() != QMetaMethod::Signal)
                        continue;

                    if (!qstrncmp(method.signature(), slot + len + 4, slotlen)) {
                        const char *signal = method.signature();
                        QString event_sig = SIGNAL_SIGNATURE;
                        event_sig.append(signal);

                        QString observer_sig = CALLBACK_SIGNATURE;
                        observer_sig.append(slot);

                        if(connect(co, event_sig.toAscii(), this, observer_sig.toAscii())) {
                            qDebug() << mafTr("CONNECTED slot %1 with signal %2").arg(slot, signal);
                            foundIt = true;
                            break;
                        } else {
                            qWarning() << mafTr("Cannot connect slot %1 with signal %2").arg(slot, signal);
                        }
                    }
                }
            }
        }
        if (foundIt) {
            // we found our slot, now skip all overloads
            while (mo->method(i + 1).attributes() & QMetaMethod::Cloned)
                  ++i;
        } else if (!(mo->method(i).attributes() & QMetaMethod::Cloned)) {
            qWarning("QMetaObject::connectSlotsByName: No matching signal for %s", slot);
        }
    }
}


void mafObjectBase::initializeUI(QObject *selfUI) {
    QList<QObject *> widgetList = qFindChildren<QObject *>(selfUI, QString());
    int i = 0, size = widgetList.count();
    for(; i<size; ++i) {
        bool propertyIsAWidget = true;
        QObject *widget = widgetList.at(i);
        QString widgetName = widget->objectName();


        QVariant value = this->property(widgetName.toAscii());
        if(!value.isValid()) {
            //qWarning(mafTr("Property with name %1 doesn't exist").arg(widgetName).toAscii());
            //continue;
            propertyIsAWidget = false;
        }

         if (propertyIsAWidget) {
            const QMetaObject *metaobject = widget->metaObject();
            int count = metaobject->propertyCount();
            for (int i=0; i<count; ++i) {
                QMetaProperty metaproperty = metaobject->property(i);
                if(!metaproperty.isUser()) {
                    continue;
                }
                const char *name = metaproperty.name();
                widget->setProperty(name,value);
                break;
            }
        }

        //Set property of the widgets
        const QMetaObject *metaobject = this->metaObject();
        int count = metaobject->propertyCount();
        for (int i=0; i<count; ++i) {
            QMetaProperty metaproperty = metaobject->property(i);
            const char *name = metaproperty.name();
            QString propertyName(name);
            if (propertyName.contains("_")){
                int index = propertyName.indexOf("_");
                QString propWidgetName = propertyName.left(index);
                QString propName = propertyName.mid(index+1);
                if (propWidgetName.compare(widgetName) == 0){
                    widget->setProperty(propName.toAscii(), this->property(propertyName.toAscii()));
                }
            }
        }
    }
}

void mafObjectBase::ref() {
    ++m_ReferenceCount;
}

void mafObjectBase::deleteObject() {
    --m_ReferenceCount;
    //char *name = this->objectName().toAscii().data();
    if(m_ReferenceCount == 0) {
        this->setParent(NULL);
        delete this;
    }
}
