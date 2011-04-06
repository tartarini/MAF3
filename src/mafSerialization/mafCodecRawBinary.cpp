/*
 *  mafCodecRawBinary.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCodecRawBinary.h"
#include <mafEventBusManager.h>
#include "QDir"

using namespace mafCore;
using namespace mafSerialization;
using namespace mafEventBus;

mafCodecRawBinary::mafCodecRawBinary(const QString code_location) : mafCodecRaw(code_location) {
    m_EncodingType = "RAW";
}

mafCodecRawBinary::~mafCodecRawBinary() {
}

void mafCodecRawBinary::encode(mafMemento *memento) {
    REQUIRE(memento != NULL);
    REQUIRE(m_Device != NULL);
    int dataSize = 0;
    double dataTime = 0;
    QString dataHash;
    QString codecType;

    m_DataStreamWrite.setDevice(m_Device);
    m_DataStreamWrite.setVersion(QDataStream::Qt_4_6);

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    const QMetaObject* meta = memento->metaObject();
    m_DataStreamWrite << QString("MementoType");
    QString cn = meta->className();
    m_DataStreamWrite << cn;
    QString ot = memento->objectClassType();
    m_DataStreamWrite << ot;

    if (cn == "mafResources::mafMementoVME") {
        m_DataStreamWrite << QString("dataSetCollection");
        //Add number of dataSet Contained by dataSetCollection
    }

    foreach(item, *propList) {
        //If next value is DataSetItem, open DataSet element
        if (item.m_Name == "mafDataSetTime") {
            m_DataStreamWrite << QString("dataSet");
        }
        m_DataStreamWrite << item.m_Name;
        m_DataStreamWrite << (int)item.m_Multiplicity;

        if (item.m_Name == "dataSize" ) {
            dataSize = item.m_Value.toInt();
            m_DataStreamWrite << (int)dataSize;
        } else if (item.m_Name == "mafDataSetTime" ) {
            dataTime = item.m_Value.toDouble();
            m_DataStreamWrite << (double)dataTime;
        } else if (item.m_Name == "dataHash" ) {
            dataHash = item.m_Value.toString();
            m_DataStreamWrite << dataHash;
        } else if (item.m_Name == "codecType" ) {
            codecType = item.m_Value.toString();
            m_DataStreamWrite << codecType;
        } else if (item.m_Name == "dataValue") {
            //Generate file name and save external data
            QString path = ((QFile *)this->m_Device)->fileName().section('/', 0, -2);
            QString fileName;
            QTextStream(&fileName) << dataHash << "_" << dataTime << ".vtk";
            QString url;
            QTextStream(&url) << path << "/" << fileName;

            mafEventArgumentsList argList;
            argList.append(mafEventArgument(char*, (char*)item.m_Value.toByteArray().constData()));
            argList.append(mafEventArgument(QString, url));
            argList.append(mafEventArgument(int, dataSize));
            mafEventBusManager::instance()->notifyEvent("maf.local.serialization.saveExternalData", mafEventTypeLocal, &argList);
            m_DataStreamWrite << fileName;
        } else {
            marshall(item.m_Value);
        }

    }

    QObject *obj;
    foreach(obj, memento->children()) {
        this->encode((mafMemento *)obj);
    }
}

mafMemento *mafCodecRawBinary::decode() {
    REQUIRE(m_Device != NULL);
    int dataSize = 0;
    double dataTime = 0;
    QString dataHash;
    QString codecType;



    QString mementoTagSeparator;
    QString mementoType;
    QString objType;
    if(m_Level == 0) {
        m_DataStreamRead.setDevice(m_Device);
        m_DataStreamRead >> mementoTagSeparator;
    }

    m_DataStreamRead >> mementoType;
    m_DataStreamRead >> objType;

    mafMemento *memento = (mafMemento *)mafNEWFromString(mementoType);
    memento->setObjectClassType(objType);

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    while(!m_DataStreamRead.atEnd()) {
        m_DataStreamRead >> mementoTagSeparator;
        if(mementoTagSeparator != "MementoType") {
            item.m_Name = mementoTagSeparator;
            if (mementoTagSeparator == "dataSetCollection" || mementoTagSeparator == "dataSet") {
                continue;
            } else {
                m_DataStreamRead >> item.m_Multiplicity;
            }

            if (item.m_Name == "dataValue") {
                //check if eChild is a file Name
                QString value;
                m_DataStreamRead >> value;
                QString path = ((QFile *)this->m_Device)->fileName().section('/', 0, -2);
                QByteArray url;
                url.append(path);
                url.append("/");
                url.append(value);
                QUrl u = QUrl::fromEncoded(url);
                if (u.isValid()) {
                    //write external file url
                    item.m_Value = u.toString();
                } else {
                    item.m_Value = value;
                }
            } else  if (item.m_Name == "dataSize" ) {
                m_DataStreamRead >> dataSize;
                item.m_Value = dataSize;
            } else if (item.m_Name == "mafDataSetTime" ) {
                m_DataStreamRead >> dataTime;
                item.m_Value = dataTime;
            } else if (item.m_Name == "dataHash" ) {
                m_DataStreamRead >> dataHash;
                item.m_Value = dataHash;
            } else if (item.m_Name == "codecType" ) {
                m_DataStreamRead >> codecType;
                item.m_Value = codecType;
            } else {
                QString codecType;
                m_DataStreamRead >> codecType;
                item.m_Value = demarshall(codecType, item.m_Multiplicity);
            }

            propList->append(item);
        } else {
            // To be continued...
            ++m_Level;
            mafMemento *mChild = decode();
            mChild->setParent(memento);
        }
    }

    return memento;
}

void mafCodecRawBinary::marshall(const QVariant &value ){
    switch( value.type() ){
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ULongLong:
                m_DataStreamWrite << QString("int");
                m_DataStreamWrite << (int)value.toInt();
                break;
        case QVariant::Double:
                m_DataStreamWrite << QString("double");
                m_DataStreamWrite << (double)value.toDouble();
                break;
        case QVariant::Bool:
                m_DataStreamWrite << QString("boolean");
                m_DataStreamWrite << (bool)value.toBool();
                break;
        case QVariant::Date:
                m_DataStreamWrite << QString("dateTime.iso8601");
                m_DataStreamWrite << (QString)value.toDate().toString( Qt::ISODate );
                break;
        case QVariant::DateTime:
                m_DataStreamWrite << QString("dateTime.iso8601");
                m_DataStreamWrite << (QString)value.toDateTime().toString( Qt::ISODate );
                break;
        case QVariant::Time:
                m_DataStreamWrite << QString("dateTime.iso8601");
                m_DataStreamWrite << (QString)value.toTime().toString( Qt::ISODate );
                break;
        case QVariant::StringList:
        case QVariant::List: {
                m_DataStreamWrite << QString("list");
                foreach( QVariant item, value.toList() ) {
                        marshall( item );
                    }
                break;
        }
        case QVariant::Map: {
            QMap<QString, QVariant> map = value.toMap();
            QMap<QString, QVariant>::ConstIterator index = map.begin();
            m_DataStreamWrite << QString("map");
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
            m_DataStreamWrite << QString("hash");
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
                m_DataStreamWrite << multi;
                marshall( *index );
                ++index;
            }
            break;
        }
        case QVariant::ByteArray: {
            m_DataStreamWrite << QString("base64");
            m_DataStreamWrite << value.toByteArray().toBase64();
            break;
        }
        default: {
            if( value.canConvert(QVariant::String) ) {
                m_DataStreamWrite << QString("string");
                m_DataStreamWrite << (QString)value.toString();
            }
            else {
               //self representation?
            }
            break;
        }
    }
}

QVariant mafCodecRawBinary::demarshall( QString typeName, int multiplicity ) {
    if ( typeName == "string" ) {
        QString value;
        m_DataStreamRead >> value;
        return QVariant( value );
    }
    else if (typeName == "int") {
        int value = 0;
        m_DataStreamRead >> value;
        QVariant val( value );
        return val;
    }
    else if( typeName == "double" ) {
        double value = 0;
        m_DataStreamRead >> value;
        QVariant val( value );
        return val;
    }
    else if( typeName == "boolean" ) {
        bool value;
        m_DataStreamRead >> value;
        return QVariant( value );
    }
    else if( typeName == "datetime" || typeName == "dateTime.iso8601" ) {
        QString value;
        m_DataStreamRead >> value;
        return QVariant( QDateTime::fromString( value, Qt::ISODate ) );
    }
    else if( typeName == "list" ) {
        QVariantList value;

        int i = 0;
        for (; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataStreamRead >> type;
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
            m_DataStreamRead >> type;
            QString nodeName = demarshall( type, multi ).toString();
            m_DataStreamRead >> type;
            m_DataStreamRead >> multi;
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
            m_DataStreamRead >> type;
            QString nodeName = demarshall( type, multi ).toString();
            m_DataStreamRead >> multi;
            m_DataStreamRead >> type;
            hash[ nodeName ] = QVariant(demarshall( type, multi ));
        }
        return QVariant(hash);
    }
    else if( typeName == "base64" ) {
        QVariant returnVariant;
        QByteArray dest;
        QByteArray src;
        m_DataStreamRead >> src;
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


