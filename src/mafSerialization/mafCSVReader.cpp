/*
 *  mafCSVReader.h
 *  mafSerialization
 *
 *  Created by Daniele Giunchi.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCSVReader.h"
#include <QTextDecoder>

using namespace mafCore;
using namespace mafSerialization;

mafCSVReader::mafCSVReader(const QString code_location): mafObjectBase(code_location) {
    initializeReader();
}

mafCSVReader::mafCSVReader(QIODevice *device, const QString code_location): mafObjectBase(code_location), m_Device(device), m_String("") {
    initializeReader();
}

mafCSVReader::mafCSVReader(QString &string, const QString code_location): mafObjectBase(code_location),  m_Device(NULL), m_String(string) {
    initializeReader();
}

void mafCSVReader::initializeReader() {
    m_Codec = QTextCodec::codecForLocale();
    m_Position = 0;
    m_RegularExpression = QRegExp("((?:(?:[^,\\n]*,?)|(?:\"[^\"]*\",?))*)\\n");
}

mafCSVReader::~mafCSVReader()
{
        //delete m_Codec;
}

void mafCSVReader::setString(QString &string) {
    initializeReader();
    m_Device = NULL;
    m_String = string;
}

void mafCSVReader::setDevice(QIODevice *device) {
    initializeReader();
    m_String.clear();
    m_Device = device;
}

void mafCSVReader::setTextCodec(const char* codecName){
        //delete m_Codec;
        m_Codec = QTextCodec::codecForName(codecName);
}

QString mafCSVReader::readLine(){
        QString line;

        if(m_String.isNull()){
		//READ DATA FROM DEVICE
		if(m_Device && m_Device->isReadable()){
                        QTextDecoder dec(m_Codec);
                        m_String = dec.toUnicode(m_Device->readAll());
		}else{
                        return QString();
		}
	}
        //mafMsgDebug() << m_String;
	//PARSE
        if((m_Position = m_RegularExpression.indexIn(m_String,m_Position)) != -1) {
                line = m_RegularExpression.cap(1);
                m_Position += m_RegularExpression.matchedLength();
	}
	return line;
	
}

QStringList mafCSVReader::parseLine(){
	return parseLine(readLine());
}

QStringList mafCSVReader::parseLine(QString line){
        QStringList list;
	int pos2 = 0;
        QRegExp rx2("(?:\"([^\"]*)\",?)|(?:([^,]*),?)");
	if(line.size()<1){
                //list << "";
	}else while (line.size()>pos2 && (pos2 = rx2.indexIn(line, pos2)) != -1) {
                QString col;
		if(rx2.cap(1).size()>0)
			col = rx2.cap(1);
		else if(rx2.cap(2).size()>0)
			col = rx2.cap(2);
		
		list << col;			

		if(col.size())
			pos2 += rx2.matchedLength();
		else
			pos2++;
	}
	return list;
}
