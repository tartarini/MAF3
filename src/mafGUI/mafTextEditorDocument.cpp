/* mafTextEditorDocument.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 09:23:18 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Mon Aug 16 15:01:03 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 120
 */

/* Commentary: 
 * See credits at end of file.
 */

/* Change log:
 * 
 */

#include "mafTextEditorDocument.h"
#include "mafTextEditorDocumentLayout.h"

#include <QTextCodec>
#include <QFileInfo>

namespace mafGUI {
    class mafTextEditorDocumentPrivate {
    public:
        enum LineTerminatorMode {
            LFLineTerminator,
            CRLFLineTerminator,
            NativeLineTerminator =
#if defined (Q_OS_WIN32)
            CRLFLineTerminator
#else
            LFLineTerminator
#endif
        };
        
    public:
        bool m_IsBinaryData;
        bool m_HasDecodingError;
        
        QString m_FileName;
        QTextCodec *m_Codec;
        QTextDocument *m_Document;
        QByteArray m_DecodingErrorSample;
        
        LineTerminatorMode m_LineTerminatorMode;
    };
}

using namespace mafGUI;

mafTextEditorDocument::mafTextEditorDocument(void) {
    m_PrivateClassPointer = new mafTextEditorDocumentPrivate;

    m_PrivateClassPointer->m_IsBinaryData = false;
    m_PrivateClassPointer->m_HasDecodingError = false;
    m_PrivateClassPointer->m_LineTerminatorMode = mafTextEditorDocumentPrivate::NativeLineTerminator;

    m_PrivateClassPointer->m_FileName = "untitled";
    m_PrivateClassPointer->m_Codec = QTextCodec::codecForLocale();
    m_PrivateClassPointer->m_Document = new QTextDocument(this);

    emit titleChanged(m_PrivateClassPointer->m_FileName);
}

mafTextEditorDocument::~mafTextEditorDocument(void) {
    delete m_PrivateClassPointer->m_Document;
    delete m_PrivateClassPointer;
}

bool mafTextEditorDocument::open(const QString& fileName) {
    if (!fileName.isEmpty()) {
        const QFileInfo fi(fileName);
        m_PrivateClassPointer->m_FileName = fi.absoluteFilePath();

        QFile file(fileName);
        if (!file.exists())
            return false;

        if (!fi.isReadable())
            return false;

        if (!fi.isWritable()) {
            if (!file.open(QIODevice::ReadOnly))
                return false;
        } else {
            if (!file.open(QIODevice::ReadWrite))
                return false;
        }
        QString title = fi.fileName();

        QByteArray buf = file.readAll();
        int bytesRead = buf.size();

        QTextCodec *codec = m_PrivateClassPointer->m_Codec;

        if (bytesRead >= 4 && ((uchar(buf[0]) == 0xff && uchar(buf[1]) == 0xfe && uchar(buf[2]) == 0 && uchar(buf[3]) == 0)
                               || (uchar(buf[0]) == 0 && uchar(buf[1]) == 0 && uchar(buf[2]) == 0xfe && uchar(buf[3]) == 0xff))) {
            codec = QTextCodec::codecForName("UTF-32");
        } else if (bytesRead >= 2 && ((uchar(buf[0]) == 0xff && uchar(buf[1]) == 0xfe)
                                      || (uchar(buf[0]) == 0xfe && uchar(buf[1]) == 0xff))) {
            codec = QTextCodec::codecForName("UTF-16");
        } else if (!codec) {
            codec = QTextCodec::codecForLocale();
        }

        QString text = m_PrivateClassPointer->m_Codec->toUnicode(buf);
        QByteArray verifyBuf = m_PrivateClassPointer->m_Codec->fromUnicode(text);

        int minSize = qMin(verifyBuf.size(), buf.size());
        m_PrivateClassPointer->m_HasDecodingError = (minSize < buf.size()- 4
                               || memcmp(verifyBuf.constData() + verifyBuf.size() - minSize,
                                         buf.constData() + buf.size() - minSize, minSize));

        if (m_PrivateClassPointer->m_HasDecodingError) {
            int p = buf.indexOf('\n', 16384);
            if (p < 0)
                m_PrivateClassPointer->m_DecodingErrorSample = buf;
            else
                m_PrivateClassPointer->m_DecodingErrorSample = buf.left(p);
        } else {
            m_PrivateClassPointer->m_DecodingErrorSample.clear();
        }

        int lf = text.indexOf('\n');
        if (lf > 0 && text.at(lf-1) == QLatin1Char('\r')) {
            m_PrivateClassPointer->m_LineTerminatorMode = mafTextEditorDocumentPrivate::CRLFLineTerminator;
        } else if (lf >= 0) {
            m_PrivateClassPointer->m_LineTerminatorMode = mafTextEditorDocumentPrivate::LFLineTerminator;
        } else {
            m_PrivateClassPointer->m_LineTerminatorMode = mafTextEditorDocumentPrivate::NativeLineTerminator;
        }

        m_PrivateClassPointer->m_Document->setModified(false);
        m_PrivateClassPointer->m_Document->setUndoRedoEnabled(false);
        if (m_PrivateClassPointer->m_IsBinaryData)
            m_PrivateClassPointer->m_Document->setHtml(tr("<em>Binary data</em>"));
        else
            m_PrivateClassPointer->m_Document->setPlainText(text);
        m_PrivateClassPointer->m_Document->setUndoRedoEnabled(true);

        mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout*>(m_PrivateClassPointer->m_Document->documentLayout());
        documentLayout->m_LastSaveRevision = 0;
        m_PrivateClassPointer->m_Document->setModified(false);

        emit titleChanged(title);
    }

    return true;
}

bool mafTextEditorDocument::save(const QString& fileName) {
    QString fName = m_PrivateClassPointer->m_FileName;
    if (!fileName.isEmpty())
        fName = fileName;

    QFile file(fName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QString plainText = m_PrivateClassPointer->m_Document->toPlainText();

    if (m_PrivateClassPointer->m_LineTerminatorMode == mafTextEditorDocumentPrivate::CRLFLineTerminator)
        plainText.replace(QLatin1Char('\n'), QLatin1String("\r\n"));

    file.write(m_PrivateClassPointer->m_Codec->fromUnicode(plainText));
    if (!file.flush())
        return false;
    file.close();

    const QFileInfo fi(fName);
    m_PrivateClassPointer->m_FileName = fi.absoluteFilePath();

    m_PrivateClassPointer->m_Document->setModified(false);
    emit titleChanged(fi.fileName());
    emit changed();

    m_PrivateClassPointer->m_IsBinaryData = false;
    m_PrivateClassPointer->m_HasDecodingError = false;
    m_PrivateClassPointer->m_DecodingErrorSample.clear();

    return true;
}

bool mafTextEditorDocument::close(void) {
    m_PrivateClassPointer->m_Document->setPlainText("");

    mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout*>(m_PrivateClassPointer->m_Document->documentLayout());
    documentLayout->m_LastSaveRevision = 0;
    m_PrivateClassPointer->m_Document->setModified(false);

    m_PrivateClassPointer->m_IsBinaryData = false;
    m_PrivateClassPointer->m_HasDecodingError = false;
    m_PrivateClassPointer->m_DecodingErrorSample.clear();

    emit titleChanged("untitled");
    emit changed();

    return true;
}

bool mafTextEditorDocument::isReadOnly(void) const {
    if (m_PrivateClassPointer->m_IsBinaryData || m_PrivateClassPointer->m_HasDecodingError)
        return true;
    if (m_PrivateClassPointer->m_FileName.isEmpty())
        return false;
    const QFileInfo fi(m_PrivateClassPointer->m_FileName);
    return !fi.isWritable();
}

bool mafTextEditorDocument::isModified(void) const {
    return m_PrivateClassPointer->m_Document->isModified();
}

bool mafTextEditorDocument::hasDecodingError(void) {
    return m_PrivateClassPointer->m_HasDecodingError;
}

QString mafTextEditorDocument::fileName(void) const {
    return m_PrivateClassPointer->m_FileName;
}

QTextDocument *mafTextEditorDocument::document(void) {
    return m_PrivateClassPointer->m_Document;
}
