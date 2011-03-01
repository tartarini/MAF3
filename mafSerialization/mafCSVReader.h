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

#ifndef mafCSVReader_H
#define mafCSVReader_H

//#include "myclass_api.h"

#include <QObject>
#include <QStringList>
#include <QIODevice>
#include <QTextCodec>
#include <QRegExp>

// Includes list
#include "mafSerialization_global.h"

namespace mafSerialization {

/**
  Class Name: mafCSVReader
  This class is a tool which is able to read and parse a csv file or a string formatted with csv.
  - parseline is the key method which read the current line and return a Sring List with all the elements inside
*/
class MAFSERIALIZATIONSHARED_EXPORT mafCSVReader : public mafCore::mafObjectBase {
        Q_OBJECT

        /// typedef macro.
        mafSuperclassMacro(mafCore::mafObjectBase);

public:
        /// Object constructor.
        mafCSVReader(const QString code_location = "");
        /// Overloaded object constructor
        mafCSVReader(QIODevice *device, const QString code_location = "");
        /// Overloaded object constructor
        mafCSVReader(QString &string, const QString code_location = "");
        /// Object Destructor
        ~mafCSVReader();

        /// set the current string that will be parsed, string variable. It calls also initialize method before set the string. Reset device source.
        void setString(QString &string);
        /// set the current device from which read csv. It calls also initialize method before set the string. Reset string source.
        void setDevice(QIODevice *device);

        /// it returns a line of the file or the string (each line need to terminate with \n.
        QString readLine();
        /// parse a line and retrieve an ordered list of string.
        QStringList parseLine();
        /// static method allow to be used like a global function.
        static QStringList parseLine(QString line);

        /// set the QTextCodec giving a specific name for it.
        void setTextCodec(const char* codecName);
private:
        /// reset position,regular expression and use the default QtextCode.
        void initialize();

        QIODevice *m_Device; ///< device which represents the source of csv input.
        QTextCodec *m_Codec; ///< provides conversions between text encodings.
        QString m_String; ///< string which represents the source of csv input.
        int m_Position; ///< internal position of the input source.
        QRegExp m_RegularExpression; ///< regular expression for determine which is the character separator.
	
};

} // namespace mafSerialization

#endif // mafCSVReader_H
