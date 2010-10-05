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

mafCodecRawBinary::mafCodecRawBinary(const mafString code_location) : mafCodecRaw(code_location) {
    m_EncodingType = "RAW";
}

mafCodecRawBinary::~mafCodecRawBinary() {
}

void mafCodecRawBinary::encode(mafMemento *memento) {
    REQUIRE(memento != NULL);
    REQUIRE(m_Device != NULL);
    int dataSize = 0;
    double dataTime = 0;
    mafString dataHash;
    mafString codecType;

    m_DataStreamWrite.setDevice(m_Device);
    m_DataStreamWrite.setVersion(QDataStream::Qt_4_6);

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    const QMetaObject* meta = memento->metaObject();
    m_DataStreamWrite << mafString("MementoType");
    mafString cn = meta->className();
    m_DataStreamWrite << cn;
    mafString ot = memento->objectClassType();
    m_DataStreamWrite << ot;

    if (cn == "mafResources::mafMementoVME") {
        m_DataStreamWrite << mafString("dataSetCollection");
        //Add number of dataSet Contained by dataSetCollection
    }

    foreach(item, *propList) {
        //If next value is DataSetItem, open DataSet element
        if (item.m_Name == "mafDataSetTime") {
            m_DataStreamWrite << mafString("dataSet");
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
            mafString path = ((mafFile *)this->m_Device)->fileName().section('/', 0, -2);
            mafString fileName;
            mafTextStream(&fileName) << dataHash << "_" << dataTime << ".vtk";
            mafString url;
            mafTextStream(&url) << path << "/" << fileName;

            mafEventArgumentsList argList;
            argList.append(mafEventArgument(char*, (char*)item.m_Value.toByteArray().constData()));
            argList.append(mafEventArgument(mafString, url));
            argList.append(mafEventArgument(int, dataSize));
            mafEventBusManager::instance()->notifyEvent("SAVE_EXTERNALDATA", mafEventTypeLocal, &argList);
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
    mafString dataHash;
    mafString codecType;



    mafString mementoTagSeparator;
    mafString mementoType;
    mafString objType;
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
                mafString value;
                m_DataStreamRead >> value;
                mafString path = ((mafFile *)this->m_Device)->fileName().section('/', 0, -2);
                mafByteArray url;
                url.append(path);
                url.append("/");
                url.append(value);
                mafUrl u = QUrl::fromEncoded(url);
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
                mafString codecType;
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

void mafCodecRawBinary::marshall(const mafVariant &value ){
    switch( value.type() ){
        case mafVariant::Int:
        case mafVariant::UInt:
        case mafVariant::LongLong:
        case mafVariant::ULongLong:
                m_DataStreamWrite << mafString("int");
                m_DataStreamWrite << (int)value.toInt();
                break;
        case mafVariant::Double:
                m_DataStreamWrite << mafString("double");
                m_DataStreamWrite << (double)value.toDouble();
                break;
        case mafVariant::Bool:
                m_DataStreamWrite << mafString("boolean");
                m_DataStreamWrite << (bool)value.toBool();
                break;
        case mafVariant::Date:
                m_DataStreamWrite << mafString("dateTime.iso8601");
                m_DataStreamWrite << (mafString)value.toDate().toString( Qt::ISODate );
                break;
        case mafVariant::DateTime:
                m_DataStreamWrite << mafString("dateTime.iso8601");
                m_DataStreamWrite << (mafString)value.toDateTime().toString( Qt::ISODate );
                break;
        case mafVariant::Time:
                m_DataStreamWrite << mafString("dateTime.iso8601");
                m_DataStreamWrite << (mafString)value.toTime().toString( Qt::ISODate );
                break;
        case mafVariant::StringList:
        case mafVariant::List: {
                m_DataStreamWrite << mafString("list");
                foreach( mafVariant item, value.toList() ) {
                        marshall( item );
                    }
                break;
        }
        case mafVariant::Map: {
            mafMap<mafString, mafVariant> map = value.toMap();
            mafMap<mafString, mafVariant>::ConstIterator index = map.begin();
            m_DataStreamWrite << mafString("map");
            while( index != map.end() ) {
                marshall(index.key());
                int multi = 0;
                if (index.value().type() == mafVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == mafVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == mafVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                marshall( *index );
                ++index;
            }
            break;
        }
        case mafVariant::Hash: {
            mafHash<mafString, mafVariant> hash = value.toHash();
            mafHash<mafString, mafVariant>::ConstIterator index = hash.begin();
            m_DataStreamWrite << mafString("hash");
            while( index != hash.end() ) {
                marshall(index.key());
                int multi = 0;
                if (index.value().type() == mafVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == mafVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == mafVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                m_DataStreamWrite << multi;
                marshall( *index );
                ++index;
            }
            break;
        }
        case mafVariant::ByteArray: {
            m_DataStreamWrite << mafString("base64");
            m_DataStreamWrite << value.toByteArray().toBase64();
            break;
        }
        default: {
            if( value.canConvert(mafVariant::String) ) {
                m_DataStreamWrite << mafString("string");
                m_DataStreamWrite << (mafString)value.toString();
            }
            else {
               //self representation?
            }
            break;
        }
    }
}

mafVariant mafCodecRawBinary::demarshall( mafString typeName, int multiplicity ) {
    if ( typeName == "string" ) {
        mafString value;
        m_DataStreamRead >> value;
        return mafVariant( value );
    }
    else if (typeName == "int") {
        int value = 0;
        m_DataStreamRead >> value;
        mafVariant val( value );
        return val;
    }
    else if( typeName == "double" ) {
        double value = 0;
        m_DataStreamRead >> value;
        mafVariant val( value );
        return val;
    }
    else if( typeName == "boolean" ) {
        bool value;
        m_DataStreamRead >> value;
        return mafVariant( value );
    }
    else if( typeName == "datetime" || typeName == "dateTime.iso8601" ) {
        mafString value;
        m_DataStreamRead >> value;
        return mafVariant( mafDateTime::fromString( value, Qt::ISODate ) );
    }
    else if( typeName == "list" ) {
        mafList<mafVariant> value;

        int i = 0;
        for (; i < multiplicity; i++) {
            mafString type;
            int multi = 0;
            m_DataStreamRead >> type;
            value.append(mafVariant(demarshall(type, multi)));
        }
        return mafVariant( value );
    }
    else if( typeName == "map" )
    {
        mafMap<mafString,mafVariant> stct;

        int i = 0;
        for (; i < multiplicity; i++) {
            mafString type;
            int multi = 0;
            m_DataStreamRead >> type;
            mafString nodeName = demarshall( type, multi ).toString();
            m_DataStreamRead >> type;
            m_DataStreamRead >> multi;
            stct[ nodeName ] = mafVariant(demarshall( type, multi ));
        }
        return mafVariant(stct);
    }
    else if( typeName == "hash" )
    {
        mafHash<mafString,mafVariant> hash;

        int i = 0;
        for (; i < multiplicity; i++) {
            mafString type;
            int multi = 0;
            m_DataStreamRead >> type;
            mafString nodeName = demarshall( type, multi ).toString();
            m_DataStreamRead >> multi;
            m_DataStreamRead >> type;
            hash[ nodeName ] = mafVariant(demarshall( type, multi ));
        }
        return mafVariant(hash);
    }
    else if( typeName == "base64" ) {
        mafVariant returnVariant;
        mafByteArray dest;
        mafByteArray src;
        m_DataStreamRead >> src;
        dest = mafByteArray::fromBase64( src );
        mafDataStream ds(&dest, mafIODevice::ReadOnly);
        ds.setVersion(mafDataStream::Qt_4_6);
        ds >> returnVariant;
        if( returnVariant.isValid() ) {
            return returnVariant;
        }
        else {
            return mafVariant( dest );
        }
    }
    mafMsgCritical() << mafString( "Cannot handle type %1").arg(typeName);
    return mafVariant();
}


