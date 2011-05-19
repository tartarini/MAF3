/*
 *  mafCodecXML.cpp
 *  mafSerialization
 *
 *  Created by Daniele Giunchi and Roberto Mucci on 19/01/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCodecXML.h"
#include <mafEventBusManager.h>
#include "QDir"
#include "QPair"

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
    
    //TODO: I presume m_Device is a file...
    QString path;
    QFile *file = qobject_cast<QFile*>(m_Device);
    if(file) {
        path = ((QFile *) m_Device)->fileName().section('/', 0, -2);
    }
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    const QMetaObject* meta = memento->metaObject();
    QString mementoType = meta->className();
    QString ot = memento->objectClassType();
    // "I" if is an Inheritance memento, "C" if is a Composition memento.
    QString serializationPattern = (memento->serializationPattern() == mafSerializationPatternInheritance) ? "I" : "C"; 
    serializationPattern.append(QString::number(m_LevelEncode));

    ++m_MementoLevel;

    if(m_MementoLevel == 0){
      m_XMLStreamWriter.setDevice(m_Device);
      m_XMLStreamWriter.setAutoFormatting(true);
      m_XMLStreamWriter.writeStartDocument();
      m_XMLStreamWriter.writeStartElement("mementoRoot"); //start memento item
    }

    m_XMLStreamWriter.writeStartElement("memento"); //start memento item
    m_XMLStreamWriter.writeAttribute("mementoType", mementoType);
    m_XMLStreamWriter.writeAttribute("serializationPattern", serializationPattern);
    m_XMLStreamWriter.writeAttribute("objectClassType", ot);

    foreach(item, *propList) {
        m_XMLStreamWriter.writeStartElement("item");
        m_XMLStreamWriter.writeAttribute("name", item.m_Name);
        m_XMLStreamWriter.writeAttribute("multiplicity", QString::number(item.m_Multiplicity));
        marshall(item.m_Value);
        if (mementoType == "mafResources::mafMementoDataSet") {
          // use mafMementoDataSet to encode dataSet items.
          memento->encodeItem(&item, path);
        } 
        
        m_XMLStreamWriter.writeEndElement();
    }
    m_XMLStreamWriter.writeEndElement(); //end memento item

    QObject *obj;
    ++m_LevelEncode;
    foreach(obj, memento->children()) {
        this->encode((mafMemento *)obj);
    }
    --m_LevelEncode;

    if(m_MementoLevel == 0) {
        m_XMLStreamWriter.writeEndElement(); //memento root
        m_XMLStreamWriter.writeEndDocument();
    }
    m_MementoLevel--;
}

mafMemento *mafCodecXML::decode() {
    REQUIRE(m_Device != NULL);

    if(m_LevelDecode == -1) {  
      m_XMLStreamReader.setDevice(m_Device);
      m_XMLStreamReader.readNextStartElement(); //start document item
      m_XMLStreamReader.readNextStartElement(); //mementoRoot
    }

    //TODO: I presume m_Device is a file...
    QString path;
    QFile *file = qobject_cast<QFile*>(m_Device);
    if(file) {
        path = ((QFile *) m_Device)->fileName().section('/', 0, -2);
    }
    QString mementoType;
    QString objType;

    QString n = m_XMLStreamReader.name().toString();
    mementoType = m_XMLStreamReader.attributes().value("mementoType").toString();
    QString serializationPatternString = m_XMLStreamReader.attributes().value("serializationPattern").toString();
    objType = m_XMLStreamReader.attributes().value("objectClassType").toString();

    mafMemento* memento = (mafMemento *)mafNEWFromString(mementoType);
    memento->setObjectClassType(objType);
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    //Set the serializationPattern for the memento: "I" if is an Inheritance memento, "C" if is a Composition memento.
    if (serializationPatternString.contains("I")) {
      memento->setSerializationPattern(mafSerializationPatternInheritance);
    } else if (serializationPatternString.contains("C")) {
      memento->setSerializationPattern(mafSerializationPatternComposition);
    }
    m_LevelDecode = serializationPatternString.right(1).toUInt();

     //Fill the list of levelDecode and memento.
    m_MementoList.push_back(qMakePair(m_LevelDecode, memento));

     while (!m_XMLStreamReader.atEnd() && !m_XMLStreamReader.hasError()) {
      if (!m_XMLStreamReader.readNextStartElement()) {
        continue;
      }

      QString name = m_XMLStreamReader.name().toString();
      if (name == "item") {
        item.m_Name = m_XMLStreamReader.attributes().value("name").toString();
        item.m_Multiplicity = m_XMLStreamReader.attributes().value("multiplicity").toString().toUInt();
        item.m_Value = demarshall(&m_XMLStreamReader);
        if (mementoType == "mafResources::mafMementoDataSet") {
          memento->decodeItem(&item, path);
        } 
        propList->append(item);
      }

      if (name == "memento") {
        int parentLevel = m_LevelDecode;
        mafMemento *mChild = decode();
        m_LevelDecode = parentLevel;
      }
    }
     // If it is the last memento, build memento tree.
    if(m_LevelDecode == 0) { 
        buildMementoTree();
    }
    return memento;
}

void mafCodecXML::buildMementoTree() {
    int listSize = m_MementoList.count();
    int i = 0;
    for(i; i < listSize-1; i++){
        int parentLevel = m_MementoList.at(i).first;
        mafMemento *mementoParent = m_MementoList.at(i).second;
        int childLevel = m_MementoList.at(i+1).first;
        mafMemento *mementoChild = m_MementoList.at(i+1).second;
        int parentRelation = childLevel - parentLevel;
        if (parentRelation > 0) {
            mementoChild->setParent(mementoParent);
        } else {
            //cycle searching for last memento with level parent of chilMemento
            for (int n = i; n >= 0; n--){
                int level = m_MementoList.at(n).first;
                if (level == (childLevel-1)) {
                    mafMemento *mementoParent = m_MementoList.at(n).second;
                    mementoChild->setParent(mementoParent);
                    break;
                }
            }
        }
    }
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
                        marshall( item );
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

QVariant mafCodecXML::demarshall( QXmlStreamReader *xmlStream ) {
    QString tagName = xmlStream->name().toString().toLower();
    if ( tagName != "item" && tagName != "value" && tagName != "member") {
        m_Valid = false;
        qCritical() << QString("bad param value");
        return QVariant();
    }

    QVariant valueElem;
    QString typeName;
    int multiplicity = -1;
    QXmlStreamAttributes attributes = xmlStream->attributes();

    if(tagName == "item" || tagName == "member") {
        if(attributes.hasAttribute("multiplicity")) {
            multiplicity = attributes.value("multiplicity").toString().toUInt();
        } 
        if (multiplicity == 0) {
            return QVariant();
        } else if (multiplicity < 0){
            qCritical() << QString("bad param value");
            return QVariant();
        } else if (multiplicity > 1){
            typeName = attributes.value("arrayType").toString();
        }  else {
            xmlStream->readNextStartElement();
            QXmlStreamAttributes attributesValue = xmlStream->attributes();
            if(attributesValue.hasAttribute("dataType")) {
              typeName = xmlStream->attributes().value("dataType").toString();
            }
        }
    } else if (tagName == "value") {
        QXmlStreamAttributes attributesValue = xmlStream->attributes();
        if(attributesValue.hasAttribute("dataType")) {
          typeName = xmlStream->attributes().value("dataType").toString();
        }
    }

    if ( typeName == "string" ) {
      xmlStream->readNext();
      return QVariant(xmlStream->text().toString());
    } else if (typeName == "int" || typeName == "i4" ) {
        bool ok = false;
        xmlStream->readNext();
        QVariant val( xmlStream->text().toString().toInt( &ok ) );
        if( ok )
                return val;
        qCritical() << QString("I was looking for an integer but data was corrupted");
    } else if( typeName == "double" ) {
        bool ok = false;
        xmlStream->readNext();
        QVariant val( xmlStream->text().toString().toDouble( &ok ) );
        if( ok )
                return val;
        qCritical() << QString("I was looking for an double but data was corrupted");
    }else if( typeName == "boolean" ) {
      xmlStream->readNext();
      return QVariant( ( xmlStream->text().toString().toLower() == "true" || xmlStream->text() == "1")?true:false );
    } else if( typeName == "datetime" || typeName == "dateTime.iso8601" ) {
      xmlStream->readNext();
      return QVariant( QDateTime::fromString( xmlStream->text().toString(), Qt::ISODate ) );
    } else if( typeName == "list" ) {
      QVariantList arr;
      int i = 0;
      for (i; i < multiplicity; i++) {
        if (!m_XMLStreamReader.readNextStartElement()) {
          --i;
          continue;
        }
        arr.append(QVariant(demarshall(xmlStream)) );
      }
      return QVariant( arr );
    } else if( typeName == "map" )
    {
      QMap<QString,QVariant> map;
      int i = 0;
      for (i; i < multiplicity; i++) {
        if (!m_XMLStreamReader.readNextStartElement()) {
          --i;
          continue;
        }
        QXmlStreamAttributes attributesValue = xmlStream->attributes();
        QString nodeName = attributesValue.value("name").toString();
        map[ nodeName ] = demarshall(xmlStream);
      }
      return QVariant(map);
    } else if( typeName == "hash" )
    {
      QHash<QString,QVariant> stct;
      int i = 0;
      for (i; i < multiplicity; i++) {
        if (!m_XMLStreamReader.readNextStartElement()) {
          --i;
          continue;
        }
        QXmlStreamAttributes attributesValue = xmlStream->attributes();
        QString nodeName = attributesValue.value("name").toString();
        stct[ nodeName ] = demarshall( xmlStream );
      }
      return QVariant(stct);
    } else if( typeName == "base64" ) {
      xmlStream->readNext();
      QVariant returnVariant;
      QByteArray dest;
      QByteArray src = xmlStream->text().toString().toLatin1();
      dest = QByteArray::fromBase64( src );
      QDataStream ds(&dest, QIODevice::ReadOnly);
      ds.setVersion(QDataStream::Qt_4_6);
      ds >> returnVariant;
      if( returnVariant.isValid() ) {
          return returnVariant;
      } else {
          return QVariant( dest );
      }
    }
    qCritical() << QString( "Cannot handle type %1").arg(typeName);
    return QVariant();
}
