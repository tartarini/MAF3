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

mafCodecRawASCII::mafCodecRawASCII(const mafString code_location) : mafCodecRaw(code_location) {
    m_EncodingType = "RAW_ASCII";
}

mafCodecRawASCII::~mafCodecRawASCII() {
}

void mafCodecRawASCII::encode(mafMemento *memento) {
    REQUIRE(memento != NULL);
    REQUIRE(m_Device != NULL);
    int dataSize = 0;
    double dataTime = 0;
    mafString dataHash;
    mafString codecType;

    m_DataTextWrite.setDevice(m_Device);
    m_DataTextWrite.setFieldAlignment(QTextStream::AlignLeft);

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    const QMetaObject* meta = memento->metaObject();
    m_DataTextWrite << mafString("MementoType") << endl;
    mafString cn = meta->className();
    m_DataTextWrite << cn << endl;
    mafString ot = memento->objectClassType();
    m_DataTextWrite << ot << endl;

    if (cn == "mafResources::mafMementoVME") {
        m_DataTextWrite << mafString("dataSetCollection") << endl;
        //Add number of dataSet Contained by dataSetCollection
    }

    foreach(item, *propList) {
        //If next value is DataSetItem, open DataSet element
        if (item.m_Name == "mafDataSetTime") {
            m_DataTextWrite << mafString("dataSet") << endl;
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
            mafString path = ((mafFile *)this->m_Device)->fileName().section('/', 0, -2);
            mafString fileName;
            mafTextStream(&fileName) << dataHash << "_" << dataTime << ".vtk";
            mafString url;
            mafTextStream(&url) << path << "/" << fileName;

            mafEventArgumentsList argList;
            argList.append(mafEventArgument(char*, (char*)item.m_Value.toByteArray().constData()));
            argList.append(mafEventArgument(mafString, url));
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
    mafString dataHash;
    mafString codecType;

    mafString mementoTagSeparator;
    mafString mementoType;
    mafString objType;
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
                mafString value;
                m_DataTextRead >> value;
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
                mafString codecType;
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

void mafCodecRawASCII::marshall(const mafVariant &value ){
    switch( value.type() ){
        case mafVariant::Int:
        case mafVariant::UInt:
        case mafVariant::LongLong:
        case mafVariant::ULongLong:
                m_DataTextWrite << mafString("int") << ' ';
                m_DataTextWrite << (int)value.toInt() << endl;
                break;
        case mafVariant::Double:
                m_DataTextWrite << mafString("double") << ' ';
                m_DataTextWrite << (double)value.toDouble() << endl;
                break;
        case mafVariant::Bool:
                m_DataTextWrite << mafString("boolean") << ' ';
                m_DataTextWrite << (int)value.toBool() << endl;
                break;
        case mafVariant::Date:
                m_DataTextWrite << mafString("dateTime.iso8601") << ' ';
                m_DataTextWrite << (mafString)value.toDate().toString( Qt::ISODate ) << endl;
                break;
        case mafVariant::DateTime:
                m_DataTextWrite << mafString("dateTime.iso8601") << ' ';
                m_DataTextWrite << (mafString)value.toDateTime().toString( Qt::ISODate ) << endl;
                break;
        case mafVariant::Time:
                m_DataTextWrite << mafString("dateTime.iso8601") << ' ';
                m_DataTextWrite << (mafString)value.toTime().toString( Qt::ISODate ) << endl;
                break;
        case mafVariant::StringList:
        case mafVariant::List: {
                m_DataTextWrite << mafString("list") << ' ';
                foreach( mafVariant item, value.toList() ) {
                        marshall( item );
                    }
                if(value.toList().count() == 0){
                   m_DataTextWrite << endl;
                }
                break;
        }
        case mafVariant::Map: {
            mafMap<mafString, mafVariant> map = value.toMap();
            mafMap<mafString, mafVariant>::ConstIterator index = map.begin();
            m_DataTextWrite << mafString("map") << ' ';
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
            m_DataTextWrite << mafString("hash") << ' ';
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
                marshall( *index );
                ++index;
            }
            break;
        }
        case mafVariant::ByteArray: {
            m_DataTextWrite << mafString("base64") << ' ';
            m_DataTextWrite << value.toByteArray().toBase64() << endl;
            break;
        }
        default: {
            if( value.canConvert(mafVariant::String) ) {
                m_DataTextWrite << mafString("string") << ' ';
                m_DataTextWrite << (mafString)value.toString() << endl;
            }
            else {
               //self representation?
            }
            break;
        }
    }
}

mafVariant mafCodecRawASCII::demarshall( mafString typeName, int multiplicity ) {
    if ( typeName == "string" ) {
        mafString value;
        value = m_DataTextRead.readLine().trimmed();
        return mafVariant( value );
    }
    else if (typeName == "int") {
        int value = 0;
        m_DataTextRead >> value;
        mafVariant val( value );
        return val;
    }
    else if( typeName == "double" ) {
        double value = 0;
        m_DataTextRead >> value;
        mafVariant val( value );
        return val;
    }
    else if( typeName == "boolean" ) {
        int value;
        m_DataTextRead >> value;
        return mafVariant( (bool)value );
    }
    else if( typeName == "datetime" || typeName == "dateTime.iso8601" ) {
        mafString value;
        m_DataTextRead >> value;
        return mafVariant( mafDateTime::fromString( value, Qt::ISODate ) );
    }
    else if( typeName == "list" ) {
        mafList<mafVariant> value;

        int i = 0;
        for (; i < multiplicity; ++i) {
            mafString type;
            int multi = 0;
            m_DataTextRead >> type;
            value.append(mafVariant(demarshall(type, multi)));
        }
        return mafVariant( value );
    }
    else if( typeName == "map" )
    {
        mafMap<mafString,mafVariant> stct;

        int i = 0;
        for (; i < multiplicity; ++i) {
            mafString type;
            int multi = 0;
            m_DataTextRead >> type;
            mafString nodeName = demarshall( type, multi ).toString();
            m_DataTextRead >> type;
            m_DataTextRead >> multi;
            stct[ nodeName ] = mafVariant(demarshall( type, multi ));
        }
        return mafVariant(stct);
    }
    else if( typeName == "hash" )
    {
        mafHash<mafString,mafVariant> hash;

        int i = 0;
        for (; i < multiplicity; ++i) {
            mafString type;
            int multi = 0;
            m_DataTextRead >> type;
            mafString nodeName = demarshall( type, multi ).toString();
            m_DataTextRead >> type;
            m_DataTextRead >> multi;
            hash[ nodeName ] = mafVariant(demarshall( type, multi ));
        }
        return mafVariant(hash);
    }
    else if( typeName == "base64" ) {
        mafVariant returnVariant;
        mafByteArray dest;
        mafByteArray src;
        m_DataTextRead >> src;
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


