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
    int dataSize = 0;
    double dataTime = 0;
    QString dataHash;
    QString codecType;

    m_DataTextWrite.setDevice(m_Device);
    m_DataTextWrite.setFieldAlignment(QTextStream::AlignLeft);

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    const QMetaObject* meta = memento->metaObject();
    m_DataTextWrite << QString("MementoType") << endl;
    QString cn = meta->className();
    m_DataTextWrite << cn << endl;
    QString ot = memento->objectClassType();
    m_DataTextWrite << ot << endl;

    if (cn == "mafResources::mafMementoVME") {
        m_DataTextWrite << QString("dataSetCollection") << endl;
        //Add number of dataSet Contained by dataSetCollection
    }

    foreach(item, *propList) {
        //If next value is DataSetItem, open DataSet element
        if (item.m_Name == "mafDataSetTime") {
            m_DataTextWrite << QString("dataSet") << endl;
        }
        m_DataTextWrite << item.m_Name << ' ';
        m_DataTextWrite << (int)item.m_Multiplicity << endl;

        if (item.m_Name == "dataSize" ) {
            dataSize = item.m_Value.toInt();
            m_DataTextWrite << (int)dataSize << endl;;
        } else if (item.m_Name == "mafDataSetTime" ) {
            dataTime = item.m_Value.toDouble();
            m_DataTextWrite << (double)dataTime << endl;;
        } else if (item.m_Name == "dataHash" ) {
            dataHash = item.m_Value.toString();
            m_DataTextWrite << dataHash << endl;
        } else if (item.m_Name == "codecType" ) {
            codecType = item.m_Value.toString();
            m_DataTextWrite << codecType << endl;;
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
            m_DataTextWrite << fileName << endl;
        } else {
            marshall(item.m_Value);
            //m_DataTextWrite << endl;
        }

    }

    QObject *obj;
    foreach(obj, memento->children()) {
        this->encode((mafMemento *)obj);
    }
}

mafMemento *mafCodecRawASCII::decode() {
    REQUIRE(m_Device != NULL);
    int dataSize = 0;
    double dataTime = 0;
    QString dataHash;
    QString codecType;

    QString mementoTagSeparator;
    QString mementoType;
    QString objType;
    if(m_Level == 0) {
        m_DataTextRead.setDevice(m_Device);
        m_DataTextRead >> mementoTagSeparator;
    }

    m_DataTextRead >> mementoType;
    m_DataTextRead >> objType;

    mafMemento *memento = (mafMemento *)mafNEWFromString(mementoType);
    memento->setObjectClassType(objType);

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    while(!m_DataTextRead.atEnd()) {
        m_DataTextRead >> mementoTagSeparator;
        if(mementoTagSeparator != "MementoType") {
            item.m_Name = mementoTagSeparator;
            if (mementoTagSeparator == "dataSetCollection" || mementoTagSeparator == "dataSet" || mementoTagSeparator.isEmpty()) {
                continue;
            } else {
                m_DataTextRead >> item.m_Multiplicity;
            }

            if (item.m_Name == "dataValue") {
                //check if eChild is a file Name
                QString value;
                m_DataTextRead >> value;
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
                m_DataTextRead >> dataSize;
                item.m_Value = dataSize;
            } else if (item.m_Name == "mafDataSetTime" ) {
                m_DataTextRead >> dataTime;
                item.m_Value = dataTime;
            } else if (item.m_Name == "dataHash" ) {
                m_DataTextRead >> dataHash;
                item.m_Value = dataHash;
            } else if (item.m_Name == "codecType" ) {
                m_DataTextRead >> codecType;
                item.m_Value = codecType;
            } else {
                QString codecType;
                m_DataTextRead >> codecType;
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


