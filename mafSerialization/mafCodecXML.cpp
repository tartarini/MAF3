/*
 *  mafCodecXML.cpp
 *  mafSerialization
 *
 *  Created by Daniele Giunchi on 19/01/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCodecXML.h"
#include <mafEventBusManager.h>
#include "QDir"

using namespace mafCore;
using namespace mafSerialization;
using namespace mafEventBus;

mafCodecXML::mafCodecXML(const mafString code_location) : mafCodec(code_location) {
    m_EncodingType = "XML";
    m_MementoLevel = -1;
    m_Valid = true;
}

mafCodecXML::~mafCodecXML() {
}

void mafCodecXML::encode(mafMemento *memento) {
    REQUIRE(memento != NULL);
    REQUIRE(m_Device != NULL);
    int dataSize = 0;
    double dataTime = 0;
    mafString dataHash;
    mafString codecType;

    m_MementoLevel++;

    if(m_MementoLevel == 0){
      m_XMLStreamWriter.setDevice(m_Device);
      m_XMLStreamWriter.setAutoFormatting(true);
      m_XMLStreamWriter.writeStartDocument();
    }

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    m_XMLStreamWriter.writeStartElement("memento"); //start memento item

    const QMetaObject* meta = memento->metaObject();
    m_XMLStreamWriter.writeAttribute("mementoType", meta->className());
    m_XMLStreamWriter.writeAttribute("objectClassType", memento->objectClassType());

    mafString className = meta->className();
    if (className == "mafResources::mafMementoVME") {
        m_XMLStreamWriter.writeStartElement("dataSetCollection");
        m_XMLStreamWriter.writeAttribute("name", "dataSetCollection"); //Start DataSetCollection
        //Add number of dataSet Contained by dataSetCollection
    }

    foreach(item, *propList) {
        //If next value is DataSetItem, open DataSet element
        if (item.m_Name == "mafDataSetTime") {
            m_XMLStreamWriter.writeStartElement("dataSet");
            m_XMLStreamWriter.writeAttribute("name", "dataSet");
        }

        m_XMLStreamWriter.writeStartElement("item");
        m_XMLStreamWriter.writeAttribute("name", item.m_Name);
        m_XMLStreamWriter.writeAttribute("multiplicity", mafString::number(item.m_Multiplicity));
        if (item.m_Name == "dataSize" ) {
            dataSize = item.m_Value.toInt();
        }
        if (item.m_Name == "mafDataSetTime" ) {
            dataTime = item.m_Value.toDouble();
        }
        if (item.m_Name == "dataHash" ) {
            dataHash = item.m_Value.toString();
        }
        if (item.m_Name == "codecType" ) {
            codecType = item.m_Value.toString();
        }

        if (item.m_Name == "dataValue") {
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
            marshall(fileName);
            m_XMLStreamWriter.writeEndElement();
        } else {
            marshall(item.m_Value);
        }

        m_XMLStreamWriter.writeEndElement();
    }
    if (className == "mafResources::mafMementoVME") {
        m_XMLStreamWriter.writeEndElement(); //end dataSetCollection item
    }

    QObject *obj;
    foreach(obj, memento->children()) {
        this->encode((mafMemento *)obj);
    }

    m_XMLStreamWriter.writeEndElement(); //end memento item

    if(m_MementoLevel == 0) {
        m_XMLStreamWriter.writeEndDocument();
    }

    m_MementoLevel--;
}

mafMemento *mafCodecXML::decode() {
    REQUIRE(m_Device != NULL);

    m_XMLDocument.setContent(m_Device);

    mafMemento *memento = NULL;
    mafMementoPropertyList *propList;

    if(m_CurrentNode.isNull()) {
        m_CurrentNode = m_XMLDocument.firstChild();
    }

    while (!m_CurrentNode.isNull()) {
       mafDomElement e = m_CurrentNode.toElement();

       if(!e.isNull()) {
           mafString name = e.tagName();
           if(name == "memento") {
               mafString mementoType;
               mafString objType;
               mafDomNamedNodeMap attributes = e.attributes();
               if(attributes.contains("mementoType")) {
                   mementoType = e.attribute("mementoType","");
               }
               if(attributes.contains("objectClassType")) {
                   objType = e.attribute("objectClassType","");
               }

               memento = (mafMemento *)mafNEWFromString(mementoType);
               memento->setObjectClassType(objType);
               propList = memento->mementoPropertyList();
           }

           //collections, items or mementos
           mafDomNodeList childNodeList = m_CurrentNode.childNodes();
           int i=0, size = childNodeList.size();
           for(;i<size;i++) {
               m_CurrentNode = childNodeList.at(i);
               mafDomElement eChild = m_CurrentNode.toElement();
               mafString childName = eChild.tagName();
               if (childName == "dataSetCollection") {
                   //Find all items in dataSetCollection
                   mafDomNodeList list = eChild.elementsByTagName("item");
                   int l=0, listsize = list.size();
                   for(;l<listsize;l++) {
                       mafDomNode node = list.at(l);
                       mafDomElement eChild = node.toElement();
                       mafMementoPropertyItem item = setPropertyItem(eChild);
                       propList->append(item);
                   }
               }
               else if(childName == "item") {
                   mafMementoPropertyItem item = setPropertyItem(eChild);
                   propList->append(item);
               }
               else if (name == "memento") {
                   mafMemento *mChild = decode();
                   mChild->setParent(memento);
                   m_CurrentNode = childNodeList.at(i);
               }
           }
       }
       m_CurrentNode = m_CurrentNode.nextSibling();
     }

    return memento;
}

mafMementoPropertyItem mafCodecXML::setPropertyItem(mafDomElement eChild){
    mafMementoPropertyItem item;
    mafString itemName;
    int multiplicity;

    mafDomNamedNodeMap attributes = eChild.attributes();
    if(attributes.contains("name")) {
        itemName = eChild.attribute("name","");
    }
    if(attributes.contains("multiplicity")) {
        multiplicity = eChild.attribute("multiplicity","").toInt();
    }
    item.m_Name = itemName;
    item.m_Multiplicity = multiplicity;

    if (item.m_Name == "dataValue") {
        //check if eChild is a file Name
        mafString value = demarshall(eChild).toString();
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
    } else {
        item.m_Value = demarshall(eChild);
    }
    return item;
}

void mafCodecXML::marshall(const mafVariant &value ){
    switch( value.type() ){
        case mafVariant::Int:
        case mafVariant::UInt:
        case mafVariant::LongLong:
        case mafVariant::ULongLong:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "i4");
                m_XMLStreamWriter.writeCharacters(value.toString());
                m_XMLStreamWriter.writeEndElement();
                break;
        case mafVariant::Double:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "double");
                m_XMLStreamWriter.writeCharacters(value.toString());
                m_XMLStreamWriter.writeEndElement();
                break;
        case mafVariant::Bool:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "boolean");
                m_XMLStreamWriter.writeCharacters((value.toBool()?"true":"false") );
                m_XMLStreamWriter.writeEndElement();
                break;
        case mafVariant::Date:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "dateTime.iso8601");
                m_XMLStreamWriter.writeCharacters(value.toDate().toString( Qt::ISODate ) );
                m_XMLStreamWriter.writeEndElement();
                break;
        case mafVariant::DateTime:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "dateTime.iso8601");
                m_XMLStreamWriter.writeCharacters(value.toDateTime().toString( Qt::ISODate ) );
                m_XMLStreamWriter.writeEndElement();
                break;
        case mafVariant::Time:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "dateTime.iso8601");
                m_XMLStreamWriter.writeCharacters(value.toTime().toString( Qt::ISODate ) );
                m_XMLStreamWriter.writeEndElement();
                break;
        case mafVariant::StringList:
        case mafVariant::List: {
                m_XMLStreamWriter.writeAttribute("arrayType", "list");
                foreach( mafVariant item, value.toList() ) {
                        marshall(  item );
                    }
                break;
        }
        case mafVariant::Map: {
            m_XMLStreamWriter.writeAttribute("arrayType", "map");
            m_XMLStreamWriter.writeStartElement("struct");
            mafMap<mafString, mafVariant> map = value.toMap();
            mafMap<mafString, mafVariant>::ConstIterator index = map.begin();
            while( index != map.end() ) {
                m_XMLStreamWriter.writeStartElement("member");
                m_XMLStreamWriter.writeAttribute("name", index.key());
                int mult = (index.value().toList().count() == 0) ? 1:index.value().toList().count();
                m_XMLStreamWriter.writeAttribute("multiplicity", mafString::number(mult));
                marshall( *index );
                m_XMLStreamWriter.writeEndElement();
                ++index;
            }
            m_XMLStreamWriter.writeEndElement();
            break;
        }
        case mafVariant::Hash: {
            m_XMLStreamWriter.writeAttribute("arrayType", "hash");
            m_XMLStreamWriter.writeStartElement("struct");
            mafHash<mafString, mafVariant> hash = value.toHash();
            mafHash<mafString, mafVariant>::ConstIterator index = hash.begin();
            while( index != hash.end() ) {
                m_XMLStreamWriter.writeStartElement("member");
                m_XMLStreamWriter.writeAttribute("name", index.key());
                int multi = 1;
                if (index.value().type() == mafVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == mafVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == mafVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                m_XMLStreamWriter.writeAttribute("multiplicity", mafString::number(multi));
                marshall( *index );
                m_XMLStreamWriter.writeEndElement();
                ++index;
            }
            m_XMLStreamWriter.writeEndElement();
            break;
        }
        case mafVariant::ByteArray: {
            m_XMLStreamWriter.writeStartElement("value");
            m_XMLStreamWriter.writeAttribute("dataType", "base64");
            m_XMLStreamWriter.writeCharacters(value.toByteArray().toBase64() );
            m_XMLStreamWriter.writeEndElement();
            break;
        }
        default: {
            if( value.canConvert(mafVariant::String) ) {
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "string");
                m_XMLStreamWriter.writeCharacters( value.toString() );
                m_XMLStreamWriter.writeEndElement();
            }
            else {
               //self representation?
            }
            break;
        }
    }
}

mafVariant mafCodecXML::demarshall( const QDomElement &elem ) {
    if ( elem.tagName().toLower() != "item" && elem.tagName().toLower() != "value" \
        && elem.tagName().toLower() != "member") {
        m_Valid = false;
        mafMsgCritical() << mafString("bad param value");
        return mafVariant();
    }

    if ( elem.tagName().toLower() == "item" && !elem.firstChild().isElement() ) {
        return mafVariant( elem.firstChild().toElement().text() );
    }

    mafDomElement valueElem;
    mafString typeName;
    mafDomNamedNodeMap attributes = elem.attributes();


    if(elem.tagName().toLower() == "item" || elem.tagName().toLower() == "member") {
        int multiplicity = 0;
        if(attributes.contains("multiplicity")) {
            multiplicity = elem.attribute("multiplicity","").toInt();
        }
        if(multiplicity > 1) {
            typeName = elem.attribute("arrayType","");
        }
        else
        {
            if(multiplicity == 0) {
                return mafVariant();
            }
            valueElem = elem.firstChild().toElement();
            mafDomNamedNodeMap attributesValue = valueElem.attributes();
            if(attributesValue.contains("dataType")) {
                typeName = valueElem.attribute("dataType","");
            }
        }
    }
    else if (elem.tagName().toLower() == "value") {
        valueElem = elem;
        mafDomNamedNodeMap attributesValue = valueElem.attributes();
        if(attributesValue.contains("dataType")) {
            typeName = valueElem.attribute("dataType","");
        }
    }

    if ( typeName == "string" ) {
        return mafVariant( valueElem.text() );
    }
    else if (typeName == "int" || typeName == "i4" ) {
        bool ok = false;
        mafVariant val( valueElem.text().toInt( &ok ) );
        if( ok )
                return val;
        mafMsgCritical() << mafString("I was looking for an integer but data was courupt");
    }
    else if( typeName == "double" ) {
        bool ok = false;
        mafVariant val( valueElem.text().toDouble( &ok ) );
        if( ok )
                return val;
        mafMsgCritical() << mafString("I was looking for an double but data was courupt");
    }
    else if( typeName == "boolean" ) {
        return mafVariant( ( valueElem.text().toLower() == "true" || valueElem.text() == "1")?true:false );
    }
    else if( typeName == "datetime" || typeName == "dateTime.iso8601" ) {
        return mafVariant( mafDateTime::fromString( valueElem.text(), Qt::ISODate ) );
    }
    else if( typeName == "list" ) {
        mafList<mafVariant> arr;
        mafDomNode valueNode = elem.firstChild();
        //mafString name = valueNode.toElement().tagName();
        while( !valueNode.isNull() && m_Valid ) {
            arr.append(mafVariant(demarshall( valueNode.toElement())) );
            valueNode = valueNode.nextSibling();
        }
        return mafVariant( arr );
    }
    else if( typeName == "map" )
    {
        mafMap<mafString,mafVariant> stct;
        valueElem = elem.firstChild().toElement();
        QDomNode valueNode = valueElem.firstChild();
        mafString name = valueNode.toElement().tagName();
        while( !valueNode.isNull() && m_Valid ) {
            mafString nodeName = valueNode.toElement().attribute("name");
            if (valueNode.toElement().attribute("arrayType") != "") {
                stct[ nodeName ] = demarshall( valueNode.toElement() );
            } else {
                const QDomElement dataNode = valueNode.toElement().elementsByTagName("value").item(0).toElement();
                mafString nameb = dataNode.tagName();
                stct[ nodeName ] = demarshall( dataNode );
            }
            valueNode = valueNode.nextSibling();
        }
        return mafVariant(stct);
    }
    else if( typeName == "hash" )
    {
        mafHash<mafString,mafVariant> stct;
        valueElem = elem.firstChild().toElement();
        QDomNode valueNode = valueElem.firstChild();
        mafString name = valueNode.toElement().tagName();
        while( !valueNode.isNull() && m_Valid ) {
            mafString nodeName = valueNode.toElement().attribute("name");
            if (valueNode.toElement().attribute("arrayType") != "") {
                stct[ nodeName ] = demarshall( valueNode.toElement() );
            } else {
                const QDomElement dataNode = valueNode.toElement().elementsByTagName("value").item(0).toElement();
                mafString nameb = dataNode.tagName();
                stct[ nodeName ] = demarshall( dataNode );
            }
            valueNode = valueNode.nextSibling();
        }
        return mafVariant(stct);
    }
    else if( typeName == "base64" ) {
        mafVariant returnVariant;
        mafByteArray dest;
        mafByteArray src = valueElem.text().toLatin1();
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
