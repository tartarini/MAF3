/*
 *  mafCodecRawASCII.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCodecRawASCII.h"
#include <mafEventBusManager.h>
#include "QDir"

using namespace mafCore;
using namespace mafSerialization;
using namespace mafEventBus;

mafCodecRawASCII::mafCodecRawASCII(const QString code_location) : mafCodecRaw(code_location) {
    m_EncodingType = "RAW_ASCII";
}

mafCodecRawASCII::~mafCodecRawASCII() {
}

void mafCodecRawASCII::encode(mafMemento *memento) {
    REQUIRE(memento != NULL);
    REQUIRE(m_Device != NULL);
   
    m_DataTextWrite.setDevice(m_Device);
    m_DataTextWrite.setFieldAlignment(QTextStream::AlignLeft);

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    const QMetaObject* meta = memento->metaObject();
    m_DataTextWrite << QString("MementoType");
    m_DataTextWrite << ' ' << m_LevelEncode << endl;
    QString mementoType = meta->className();
    m_DataTextWrite << mementoType << endl;
    QString ot = memento->objectClassType();
    m_DataTextWrite << ot << endl;

    foreach(item, *propList) { //for should be inside each encodeItem
      m_DataTextWrite << item.m_Name << ' ';
      m_DataTextWrite << (int)item.m_Multiplicity << endl;

      if (mementoType == "mafResources::mafMementoDataSet") {
        // use mafMementoDataSet to encode dataSet items.
        memento->encodeItem(NULL, NULL, &m_DataTextWrite, item);
      } else {
          marshall(item.m_Value); //If will be removed: each memento will have its "encodeItem", and marshall will be moved in a separated class
      }
    }

    QObject *obj;
    ++m_LevelEncode;
    foreach(obj, memento->children()) {
        this->encode((mafMemento *)obj);
    }
    --m_LevelEncode;
}

mafMemento *mafCodecRawASCII::decode() {
    REQUIRE(m_Device != NULL);

    QString mementoTagSeparator;
    QString mementoType;
    QString objType;
    if(m_LevelDecode == -1) {  
      //-1 for initializing the file
        m_DataTextRead.setDevice(m_Device);
        m_DataTextRead >> mementoTagSeparator;
    }

    m_DataTextRead >> m_LevelDecode; //read memento level
    m_DataTextRead >> mementoType;
    m_DataTextRead >> objType;

    mafMemento *memento = (mafMemento *)mafNEWFromString(mementoType);
    memento->setObjectClassType(objType);

    //Fill the map of memento and levelDecode.
    m_MementoMap[m_LevelDecode] = memento;

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    while(!m_DataTextRead.atEnd()) {
        int pos = m_DataTextRead.pos();
        m_DataTextRead >> mementoTagSeparator;
        if(mementoTagSeparator != "MementoType") {
            item.m_Name = mementoTagSeparator;
            if (mementoTagSeparator.isEmpty()) {
                continue;
            } else {
                m_DataTextRead >> item.m_Multiplicity;
            }
            if (mementoType == "mafResources::mafMementoDataSet") {
              m_DataTextRead.seek(pos);
              // use mafMementoDataSet to encode dataSet items.
              item.m_Value = memento->decodeItem(NULL, NULL, &m_DataTextRead);
            } else {
              QString typeName;
              m_DataTextRead >> typeName;
              item.m_Value = demarshall(typeName, item.m_Multiplicity);
            }
            propList->append(item);
        } else {
            int parentLevel = m_LevelDecode;
            mafMemento *mChild = decode();
            int parentRelation = m_LevelDecode - parentLevel;
            if (parentRelation > 0) {
              mChild->setParent(memento);
            } else {
              QMap<int, mafMemento*>::const_iterator i = m_MementoMap.find(m_LevelDecode-1);
              mafMemento *mementoParent = (mafMemento*)i.value();
              mChild->setParent(mementoParent);
            }
            m_LevelDecode = parentLevel;
        }
    }

    return memento;
}

void mafCodecRawASCII::marshall(const QVariant &value ){
    switch( value.type() ){
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ULongLong:
                m_DataTextWrite << QString("int") << ' ';
                m_DataTextWrite << (int)value.toInt() << endl;
                break;
        case QVariant::Double:
                m_DataTextWrite << QString("double") << ' ';
                m_DataTextWrite << (double)value.toDouble() << endl;
                break;
        case QVariant::Bool:
                m_DataTextWrite << QString("boolean") << ' ';
                m_DataTextWrite << (int)value.toBool() << endl;
                break;
        case QVariant::Date:
                m_DataTextWrite << QString("dateTime.iso8601") << ' ';
                m_DataTextWrite << (QString)value.toDate().toString( Qt::ISODate ) << endl;
                break;
        case QVariant::DateTime:
                m_DataTextWrite << QString("dateTime.iso8601") << ' ';
                m_DataTextWrite << (QString)value.toDateTime().toString( Qt::ISODate ) << endl;
                break;
        case QVariant::Time:
                m_DataTextWrite << QString("dateTime.iso8601") << ' ';
                m_DataTextWrite << (QString)value.toTime().toString( Qt::ISODate ) << endl;
                break;
        case QVariant::StringList:
        case QVariant::List: {
                m_DataTextWrite << QString("list") << ' ';
                foreach( QVariant item, value.toList() ) {
                        marshall( item );
                    }
                if(value.toList().count() == 0){
                   m_DataTextWrite << endl;
                }
                break;
        }
        case QVariant::Map: {
            QMap<QString, QVariant> map = value.toMap();
            QMap<QString, QVariant>::ConstIterator index = map.begin();
            m_DataTextWrite << QString("map") << ' ';
            while( index != map.end() ) {
                marshall(index.key());
                int multi = 0;
                if (index.value().type() == QVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == QVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == QVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                marshall( *index );
                ++index;
            }
            break;
        }
        case QVariant::Hash: {
            QHash<QString, QVariant> hash = value.toHash();
            QHash<QString, QVariant>::ConstIterator index = hash.begin();
            m_DataTextWrite << QString("hash") << ' ';
            while( index != hash.end() ) {
                marshall(index.key());
                int multi = 0;
                if (index.value().type() == QVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == QVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == QVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                marshall( *index );
                ++index;
            }
            break;
        }
        case QVariant::ByteArray: {
            m_DataTextWrite << QString("base64") << ' ';
            m_DataTextWrite << value.toByteArray().toBase64() << endl;
            break;
        }
        default: {
            if( value.canConvert(QVariant::String) ) {
                m_DataTextWrite << QString("string") << ' ';
                m_DataTextWrite << (QString)value.toString() << endl;
            }
            else {
               //self representation?
            }
            break;
        }
    }
}

QVariant mafCodecRawASCII::demarshall( QString typeName, int multiplicity ) {
    if ( typeName == "string" ) {
        QString value;
        value = m_DataTextRead.readLine().trimmed();
        return QVariant( value );
    }
    else if (typeName == "int") {
        int value = 0;
        m_DataTextRead >> value;
        QVariant val( value );
        return val;
    }
    else if( typeName == "double" ) {
        double value = 0;
        m_DataTextRead >> value;
        QVariant val( value );
        return val;
    }
    else if( typeName == "boolean" ) {
        int value;
        m_DataTextRead >> value;
        return QVariant( (bool)value );
    }
    else if( typeName == "datetime" || typeName == "dateTime.iso8601" ) {
        QString value;
        m_DataTextRead >> value;
        return QVariant( QDateTime::fromString( value, Qt::ISODate ) );
    }
    else if( typeName == "list" ) {
        QVariantList value;

        int i = 0;
        for (; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataTextRead >> type;
            value.append(QVariant(demarshall(type, multi)));
        }
        return QVariant( value );
    }
    else if( typeName == "map" )
    {
        QMap<QString,QVariant> stct;

        int i = 0;
        for (; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataTextRead >> type;
            QString nodeName = demarshall( type, multi ).toString();
            m_DataTextRead >> type;
            m_DataTextRead >> multi;
            stct[ nodeName ] = QVariant(demarshall( type, multi ));
        }
        return QVariant(stct);
    }
    else if( typeName == "hash" )
    {
        QHash<QString,QVariant> hash;

        int i = 0;
        for (; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataTextRead >> type;
            QString nodeName = demarshall( type, multi ).toString();
            m_DataTextRead >> type;
            m_DataTextRead >> multi;
            hash[ nodeName ] = QVariant(demarshall( type, multi ));
        }
        return QVariant(hash);
    }
    else if( typeName == "base64" ) {
        QVariant returnVariant;
        QByteArray dest;
        QByteArray src;
        m_DataTextRead >> src;
        dest = QByteArray::fromBase64( src );
        QDataStream ds(&dest, QIODevice::ReadOnly);
        ds.setVersion(QDataStream::Qt_4_6);
        ds >> returnVariant;
        if( returnVariant.isValid() ) {
            return returnVariant;
        }
        else {
            return QVariant( dest );
        }
    }
    qCritical() << QString( "Cannot handle type %1").arg(typeName);
    return QVariant();
}


