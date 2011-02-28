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

mafCodecXML::mafCodecXML(const QString code_location) : mafCodec(code_location) {
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
    QString dataHash;
    QString codecType;

    ++m_MementoLevel;

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

    QString className = meta->className();
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
        m_XMLStreamWriter.writeAttribute("multiplicity", QString::number(item.m_Multiplicity));
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
           QString name = e.tagName();
           if(name == "memento") {
               QString mementoType;
               QString objType;
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
           for(;i<size;++i) {
               m_CurrentNode = childNodeList.at(i);
               mafDomElement eChild = m_CurrentNode.toElement();
               QString childName = eChild.tagName();
               if (childName == "dataSetCollection") {
                   //Find all items in dataSetCollection
                   mafDomNodeList list = eChild.elementsByTagName("item");
                   int l=0, listsize = list.size();
                   for(;l<listsize;++l) {
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
    QString itemName;
    int multiplicity = 1;

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
        QString value = demarshall(eChild).toString();
        QString path = ((QFile *)this->m_Device)->fileName().section('/', 0, -2);
        QByteArray url;
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

void mafCodecXML::marshall(const QVariant &value ){
    switch( value.type() ){
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ULongLong:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "i4");
                m_XMLStreamWriter.writeCharacters(value.toString());
                m_XMLStreamWriter.writeEndElement();
                break;
        case QVariant::Double:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "double");
                m_XMLStreamWriter.writeCharacters(value.toString());
                m_XMLStreamWriter.writeEndElement();
                break;
        case QVariant::Bool:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "boolean");
                m_XMLStreamWriter.writeCharacters((value.toBool()?"true":"false") );
                m_XMLStreamWriter.writeEndElement();
                break;
        case QVariant::Date:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "dateTime.iso8601");
                m_XMLStreamWriter.writeCharacters(value.toDate().toString( Qt::ISODate ) );
                m_XMLStreamWriter.writeEndElement();
                break;
        case QVariant::DateTime:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "dateTime.iso8601");
                m_XMLStreamWriter.writeCharacters(value.toDateTime().toString( Qt::ISODate ) );
                m_XMLStreamWriter.writeEndElement();
                break;
        case QVariant::Time:
                m_XMLStreamWriter.writeStartElement("value");
                m_XMLStreamWriter.writeAttribute("dataType", "dateTime.iso8601");
                m_XMLStreamWriter.writeCharacters(value.toTime().toString( Qt::ISODate ) );
                m_XMLStreamWriter.writeEndElement();
                break;
        case QVariant::StringList:
        case QVariant::List: {
                m_XMLStreamWriter.writeAttribute("arrayType", "list");
                foreach( QVariant item, value.toList() ) {
                        marshall(  item );
                    }
                break;
        }
        case QVariant::Map: {
            m_XMLStreamWriter.writeAttribute("arrayType", "map");
            m_XMLStreamWriter.writeStartElement("struct");
            QMap<QString, QVariant> map = value.toMap();
            QMap<QString, QVariant>::ConstIterator index = map.begin();
            while( index != map.end() ) {
                m_XMLStreamWriter.writeStartElement("member");
                m_XMLStreamWriter.writeAttribute("name", index.key());
                int mult = (index.value().toList().count() == 0) ? 1:index.value().toList().count();
                m_XMLStreamWriter.writeAttribute("multiplicity", QString::number(mult));
                marshall( *index );
                m_XMLStreamWriter.writeEndElement();
                ++index;
            }
            m_XMLStreamWriter.writeEndElement();
            break;
        }
        case QVariant::Hash: {
            m_XMLStreamWriter.writeAttribute("arrayType", "hash");
            m_XMLStreamWriter.writeStartElement("struct");
            QHash<QString, QVariant> hash = value.toHash();
            QHash<QString, QVariant>::ConstIterator index = hash.begin();
            while( index != hash.end() ) {
                m_XMLStreamWriter.writeStartElement("member");
                m_XMLStreamWriter.writeAttribute("name", index.key());
                int multi = 1;
                if (index.value().type() == QVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == QVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == QVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                m_XMLStreamWriter.writeAttribute("multiplicity", QString::number(multi));
                marshall( *index );
                m_XMLStreamWriter.writeEndElement();
                ++index;
            }
            m_XMLStreamWriter.writeEndElement();
            break;
        }
        case QVariant::ByteArray: {
            m_XMLStreamWriter.writeStartElement("value");
            m_XMLStreamWriter.writeAttribute("dataType", "base64");
            m_XMLStreamWriter.writeCharacters(value.toByteArray().toBase64() );
            m_XMLStreamWriter.writeEndElement();
            break;
        }
        default: {
            if( value.canConvert(QVariant::String) ) {
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

QVariant mafCodecXML::demarshall( const QDomElement &elem ) {
    if ( elem.tagName().toLower() != "item" && elem.tagName().toLower() != "value" \
        && elem.tagName().toLower() != "member") {
        m_Valid = false;
        qCritical() << QString("bad param value");
        return QVariant();
    }

    if ( elem.tagName().toLower() == "item" && !elem.firstChild().isElement() ) {
        return QVariant( elem.firstChild().toElement().text() );
    }

    mafDomElement valueElem;
    QString typeName;
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
                return QVariant();
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
        return QVariant( valueElem.text() );
    }
    else if (typeName == "int" || typeName == "i4" ) {
        bool ok = false;
        QVariant val( valueElem.text().toInt( &ok ) );
        if( ok )
                return val;
        qCritical() << QString("I was looking for an integer but data was courupt");
    }
    else if( typeName == "double" ) {
        bool ok = false;
        QVariant val( valueElem.text().toDouble( &ok ) );
        if( ok )
                return val;
        qCritical() << QString("I was looking for an double but data was courupt");
    }
    else if( typeName == "boolean" ) {
        return QVariant( ( valueElem.text().toLower() == "true" || valueElem.text() == "1")?true:false );
    }
    else if( typeName == "datetime" || typeName == "dateTime.iso8601" ) {
        return QVariant( QDateTime::fromString( valueElem.text(), Qt::ISODate ) );
    }
    else if( typeName == "list" ) {
        QList<QVariant> arr;
        mafDomNode valueNode = elem.firstChild();
        //QString name = valueNode.toElement().tagName();
        while( !valueNode.isNull() && m_Valid ) {
            arr.append(QVariant(demarshall( valueNode.toElement())) );
            valueNode = valueNode.nextSibling();
        }
        return QVariant( arr );
    }
    else if( typeName == "map" )
    {
        QMap<QString,QVariant> stct;
        valueElem = elem.firstChild().toElement();
        QDomNode valueNode = valueElem.firstChild();
        QString name = valueNode.toElement().tagName();
        while( !valueNode.isNull() && m_Valid ) {
            QString nodeName = valueNode.toElement().attribute("name");
            if (valueNode.toElement().attribute("arrayType") != "") {
                stct[ nodeName ] = demarshall( valueNode.toElement() );
            } else {
                const QDomElement dataNode = valueNode.toElement().elementsByTagName("value").item(0).toElement();
                QString nameb = dataNode.tagName();
                stct[ nodeName ] = demarshall( dataNode );
            }
            valueNode = valueNode.nextSibling();
        }
        return QVariant(stct);
    }
    else if( typeName == "hash" )
    {
        QHash<QString,QVariant> stct;
        valueElem = elem.firstChild().toElement();
        QDomNode valueNode = valueElem.firstChild();
        QString name = valueNode.toElement().tagName();
        while( !valueNode.isNull() && m_Valid ) {
            QString nodeName = valueNode.toElement().attribute("name");
            if (valueNode.toElement().attribute("arrayType") != "") {
                stct[ nodeName ] = demarshall( valueNode.toElement() );
            } else {
                const QDomElement dataNode = valueNode.toElement().elementsByTagName("value").item(0).toElement();
                QString nameb = dataNode.tagName();
                stct[ nodeName ] = demarshall( dataNode );
            }
            valueNode = valueNode.nextSibling();
        }
        return QVariant(stct);
    }
    else if( typeName == "base64" ) {
        QVariant returnVariant;
        QByteArray dest;
        QByteArray src = valueElem.text().toLatin1();
        dest = QByteArray::fromBase64( src );
        QDataStream ds(&dest, mafIODevice::ReadOnly);
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
