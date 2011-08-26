/* mafTextEditor.cpp --- 
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

#include <QPainter>
#include <QStyle>

#include "mafTextEditor.h"
#include "mafTextEditorPreferencesWidget.h"

namespace mafGUI {
    class mafTextEditorExtraArea : public QWidget
    {
    public:
        mafTextEditorExtraArea(mafTextEditor *editor) : QWidget(editor) {
            this->editor = editor;
            this->setAutoFillBackground(true);
        }
        
        QSize sizeHint(void) const {
            return QSize(editor->extraAreaWidth(), 0);
        }
        
    protected:
        void paintEvent(QPaintEvent *event) {
            editor->extraAreaPaintEvent(event);
        }
        
    private:
        mafTextEditor *editor;
    };
    
    class mafTextEditorDocumentPrivate
    {
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
        bool isBinaryData;
        bool hasDecodingError;
        
        QString fileName;
        QTextCodec *codec;
        QTextDocument *document;
        QByteArray decodingErrorSample;
        
        LineTerminatorMode lineTerminatorMode;
    };

}

using namespace mafGUI;


mafTextEditorDocument::mafTextEditorDocument(void)
{
    d = new mafTextEditorDocumentPrivate;

    d->isBinaryData = false;
    d->hasDecodingError = false;
    d->lineTerminatorMode = mafTextEditorDocumentPrivate::NativeLineTerminator;

    d->fileName = "untitled";
    d->codec = QTextCodec::codecForLocale();
    d->document = new QTextDocument(this);

    emit titleChanged(d->fileName);
}

mafTextEditorDocument::~mafTextEditorDocument(void)
{
    delete d->document;
    delete d;
}

bool mafTextEditorDocument::open(const QString& fileName)
{
    if (!fileName.isEmpty()) {
        const QFileInfo fi(fileName);
        d->fileName = fi.absoluteFilePath();

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

        QTextCodec *codec = d->codec;

        if (bytesRead >= 4 && ((uchar(buf[0]) == 0xff && uchar(buf[1]) == 0xfe && uchar(buf[2]) == 0 && uchar(buf[3]) == 0)
                               || (uchar(buf[0]) == 0 && uchar(buf[1]) == 0 && uchar(buf[2]) == 0xfe && uchar(buf[3]) == 0xff))) {
            codec = QTextCodec::codecForName("UTF-32");
        } else if (bytesRead >= 2 && ((uchar(buf[0]) == 0xff && uchar(buf[1]) == 0xfe)
                                      || (uchar(buf[0]) == 0xfe && uchar(buf[1]) == 0xff))) {
            codec = QTextCodec::codecForName("UTF-16");
        } else if (!codec) {
            codec = QTextCodec::codecForLocale();
        }

        QString text = d->codec->toUnicode(buf);
        QByteArray verifyBuf = d->codec->fromUnicode(text);

        int minSize = qMin(verifyBuf.size(), buf.size());
        d->hasDecodingError = (minSize < buf.size()- 4
                               || memcmp(verifyBuf.constData() + verifyBuf.size() - minSize,
                                         buf.constData() + buf.size() - minSize, minSize));

        if (d->hasDecodingError) {
            int p = buf.indexOf('\n', 16384);
            if (p < 0)
                d->decodingErrorSample = buf;
            else
                d->decodingErrorSample = buf.left(p);
        } else {
            d->decodingErrorSample.clear();
        }

        int lf = text.indexOf('\n');
        if (lf > 0 && text.at(lf-1) == QLatin1Char('\r')) {
            d->lineTerminatorMode = mafTextEditorDocumentPrivate::CRLFLineTerminator;
        } else if (lf >= 0) {
            d->lineTerminatorMode = mafTextEditorDocumentPrivate::LFLineTerminator;
        } else {
            d->lineTerminatorMode = mafTextEditorDocumentPrivate::NativeLineTerminator;
        }

        d->document->setModified(false);
        d->document->setUndoRedoEnabled(false);
        if (d->isBinaryData)
            d->document->setHtml(tr("<em>Binary data</em>"));
        else
            d->document->setPlainText(text);
        d->document->setUndoRedoEnabled(true);

        mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout*>(d->document->documentLayout());
        documentLayout->lastSaveRevision = 0;
        d->document->setModified(false);

        emit titleChanged(title);
    }

    return true;
}

bool mafTextEditorDocument::save(const QString& fileName)
{
    QString fName = d->fileName;
    if (!fileName.isEmpty())
        fName = fileName;

    QFile file(fName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QString plainText = d->document->toPlainText();

    if (d->lineTerminatorMode == mafTextEditorDocumentPrivate::CRLFLineTerminator)
        plainText.replace(QLatin1Char('\n'), QLatin1String("\r\n"));

    file.write(d->codec->fromUnicode(plainText));
    if (!file.flush())
        return false;
    file.close();

    const QFileInfo fi(fName);
    d->fileName = fi.absoluteFilePath();

    d->document->setModified(false);
    emit titleChanged(fi.fileName());
    emit changed();

    d->isBinaryData = false;
    d->hasDecodingError = false;
    d->decodingErrorSample.clear();

    return true;
}

bool mafTextEditorDocument::close(void)
{
    d->document->setPlainText("");

    mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout*>(d->document->documentLayout());
    documentLayout->lastSaveRevision = 0;
    d->document->setModified(false);

    d->isBinaryData = false;
    d->hasDecodingError = false;
    d->decodingErrorSample.clear();

    emit titleChanged("untitled");
    emit changed();

    return true;
}

bool mafTextEditorDocument::isReadOnly(void) const
{
    if (d->isBinaryData || d->hasDecodingError)
        return true;
    if (d->fileName.isEmpty())
        return false;
    const QFileInfo fi(d->fileName);
    return !fi.isWritable();
}

bool mafTextEditorDocument::isModified(void) const
{
    return d->document->isModified();
}

bool mafTextEditorDocument::hasDecodingError(void)
{
    return d->hasDecodingError;
}

QString mafTextEditorDocument::fileName(void) const
{
    return d->fileName;
}

QTextDocument *mafTextEditorDocument::document(void)
{
    return d->document;
}

// /////////////////////////////////////////////////////////////////
// mafTextEditorPrivate
// /////////////////////////////////////////////////////////////////

class mafGUI::mafTextEditorPrivate
{
public:
    bool showLineNumbers;
    bool showCurrentLine;
    bool showRevisions;

    mafTextEditorDocument *document;

    mafTextEditorExtraArea *extraArea;
    mafTextEditorPreferencesWidget *preferences;
};

// /////////////////////////////////////////////////////////////////
// mafTextEditorPrivate
// /////////////////////////////////////////////////////////////////

mafTextEditor::mafTextEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    d = new mafTextEditorPrivate;
    d->showLineNumbers = true;
    d->showCurrentLine = true;
    d->showRevisions = true;

    d->extraArea = new mafTextEditorExtraArea(this);
    d->preferences = NULL;

    d->document = new mafTextEditorDocument;
    // begin setting up document
    QTextDocument *doc = d->document->document();
    mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout*>(doc->documentLayout());
    if(!documentLayout) {
        documentLayout = new mafTextEditorDocumentLayout(doc);
        doc->setDocumentLayout(documentLayout);
    }
    // end setting up document
    this->setDocument(doc);

    this->setAutoFillBackground(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setCursorWidth(2);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(onBlockCountChanged(int)));
    connect(this, SIGNAL(modificationChanged(bool)), this, SLOT(onModificationChanged(bool)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(onUpdateRequest(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));

    connect(doc, SIGNAL(contentsChanged()), this, SIGNAL(documentChanged()));

    connect(d->document, SIGNAL(titleChanged(QString)), this, SIGNAL(titleChanged(QString)));

    this->onUpdateExtraAreaWidth();

    this->installEventFilter(this);
}

mafTextEditor::~mafTextEditor(void)
{
    delete d;
}

void mafTextEditor::readSettings(void)
{    
    QSettings settings("inria", "maf");
    settings.beginGroup("editor");
    this->setFont(settings.value("font").value<QFont>());
    this->setShowRevisions(settings.value("showRevisions").toBool());
    this->setShowCurrentLine(settings.value("showCurrentLine").toBool());
    this->setShowLineNumbers(settings.value("showLineNumbers").toBool());
    settings.endGroup();
}

void mafTextEditor::writeSettings(void)
{
    QSettings settings("inria", "maf");
    settings.beginGroup("editor");
    settings.setValue("font", this->font());
    settings.setValue("showRevisions", this->showRevisions());
    settings.setValue("showCurrentLine", this->showCurrentLine());
    settings.setValue("showLineNumbers", this->showLineNumbers());
    settings.endGroup();
}

int mafTextEditor::currentLineNumber(void) const
{
    return this->textCursor().blockNumber() + 1;
}

int mafTextEditor::currentColumnNumber(void) const
{
    return this->textCursor().position() - this->textCursor().block().position() + 1;
}

bool mafTextEditor::open(const QString& fileName)
{
    if (d->document->open(fileName)) {
        moveCursor(QTextCursor::Start);
        setReadOnly(d->document->hasDecodingError());

        /*if(fileName.endsWith(".cpp") || fileName.endsWith(".cxx") || fileName.endsWith(".c") || fileName.endsWith(".h"))
            new mafTextEditorSyntaxHighlighterCpp(this);*/

        /*if(fileName.endsWith(".py"))
            new mafSyntaxHighlighterPython(this->document());*/

        /*if(fileName.endsWith(".tcl"))
            new mafTextEditorSyntaxHighlighterTcl(this);*/

        return true;
    }

    return false;
}

bool mafTextEditor::save(const QString& fileName)
{    
    return d->document->save(fileName);
}

bool mafTextEditor::close(void)
{
    return d->document->close();
}

bool mafTextEditor::isReadOnly(void) const
{
    return d->document->isReadOnly();
}

bool mafTextEditor::isModified(void) const
{
    return d->document->isModified();
}

bool mafTextEditor::showLineNumbers(void)
{
    return d->showLineNumbers;
}

bool mafTextEditor::showCurrentLine(void)
{
    return d->showCurrentLine;
}

bool mafTextEditor::showRevisions(void)
{
    return d->showRevisions;
}

QString mafTextEditor::fileName(void) const
{
    return d->document->fileName();
}

QString mafTextEditor::currentLine(void) const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    
    return tc.selectedText();
}

int mafTextEditor::backgroundOpacity(void) const
{
    // return (int)(this->windowOpacity()*255);

    QPalette p(this->palette());
    return p.color(QPalette::Base).alpha();
}

QColor mafTextEditor::backgroundColor(void) const
{
    QPalette p(palette());
    return p.color(QPalette::Base);
}

QColor mafTextEditor::foregroundColor(void) const
{
    QPalette p(palette());
    return p.color(QPalette::Text);
}

mafTextEditorPreferencesWidget *mafTextEditor::preferencesWidget(QWidget *parent)
{
    if(!d->preferences)
        d->preferences = new mafTextEditorPreferencesWidget(this, parent);

    return d->preferences;
}

void mafTextEditor::setShowLineNumbers(bool show)
{
    d->showLineNumbers = show;

    onUpdateExtraAreaWidth();
}

void mafTextEditor::setShowCurrentLine(bool show)
{
    d->showCurrentLine = show;
}

void mafTextEditor::setShowRevisions(bool show)
{
    d->showRevisions = show;

    onUpdateExtraAreaWidth();
}

void mafTextEditor::setBackgroundOpacity(int opacity)
{
    // this->setWindowOpacity((qreal)(opacity/255.0));

    QPalette p(this->palette());

    QColor color = p.color(QPalette::Base);
    color.setAlpha(opacity);
    p.setColor(QPalette::Window, color);

    this->setPalette(p);
    
    this->viewport()->update();
}

void mafTextEditor::setBackgroundColor(QColor color)
{
    QPalette p(this->palette());
    p.setColor(QPalette::Base, color);

    this->setPalette(p);

    this->viewport()->update();
}

void mafTextEditor::setForegroundColor(QColor color)
{
    QPalette p(this->palette());
    p.setColor(QPalette::Text, color);

    this->setPalette(p);

    this->viewport()->update();
}

void mafTextEditor::changeEvent(QEvent *e)
{
    QPlainTextEdit::changeEvent(e);

    if (e->type() == QEvent::ApplicationFontChange || e->type() == QEvent::FontChange) {
        if (d->extraArea) {
            QFont f = d->extraArea->font();
            f.setPointSize(font().pointSize());
            d->extraArea->setFont(f);
            onUpdateExtraAreaWidth();
            d->extraArea->update();
        }
    }
}

void mafTextEditor::focusInEvent(QFocusEvent *event)
{
    QPlainTextEdit::focusInEvent(event);
}

#include <iostream>

void mafTextEditor::keyPressEvent(QKeyEvent *event)
{
    QPlainTextEdit::keyPressEvent(event);
}

void mafTextEditor::paintEvent(QPaintEvent *event)
{
    const QColor baseColor = palette().base().color();

    const int blendBase   = (baseColor.value() > 128) ? 0 : 255;
    const int blendFactor = (baseColor.value() > 128) ? 8 : 16;

    const QColor blendColor(
        (blendBase * blendFactor + baseColor.blue()  * (256 - blendFactor)) / 256,
        (blendBase * blendFactor + baseColor.green() * (256 - blendFactor)) / 256,
        (blendBase * blendFactor + baseColor.blue()  * (256 - blendFactor)) / 256);

    QPainter painter(viewport());

    if(d->showCurrentLine) {
        QRect r = cursorRect();
        r.setX(0);
        r.setWidth(viewport()->width());
        painter.fillRect(r, blendColor);
    }

    painter.end();

    QPlainTextEdit::paintEvent(event);
}

void mafTextEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = viewport()->rect();
    d->extraArea->setGeometry(QStyle::visualRect(layoutDirection(), cr, QRect(cr.left(), cr.top(), extraAreaWidth(), cr.height())));
}

void mafTextEditor::closeEvent(QCloseEvent *event)
{
    emit closed();
}

void mafTextEditor::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        const int delta = event->delta();
        if (delta < 0)
            zoomOut();
        else if (delta > 0)
            zoomIn();
        return;
    }

    QPlainTextEdit::wheelEvent(event);
}

void mafTextEditor::zoomIn(int range)
{
    QFont f = font();
    const int newSize = f.pointSize() + range;
    if (newSize <= 0)
        return;
    f.setPointSize(newSize);
    setFont(f);
}

void mafTextEditor::zoomOut(int range)
{
    zoomIn(-range);
}

int mafTextEditor::extraAreaWidth(void) const
{
    if(!d->showLineNumbers && !d->showRevisions)
        return 0;

    mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout *>(this->document()->documentLayout());

    if(!documentLayout)
        return 0;

    int space = 0;
    const QFontMetrics fm(d->extraArea->fontMetrics());

    if (d->showLineNumbers) {
        int digits = 2;
        int max = qMax(1, this->blockCount());
        while (max >= 100) {
            max /= 10;
            ++digits;
        }
        space += fm.width(QLatin1Char('9')) * digits;
    }

    space += 4;

    return space;
}

void mafTextEditor::extraAreaPaintEvent(QPaintEvent *event)
{
    if(!d->showLineNumbers && !d->showRevisions)
        return;

    mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout *>(this->document()->documentLayout());

    QPalette palette = d->extraArea->palette();
    palette.setCurrentColorGroup(QPalette::Active);

    QPainter painter(d->extraArea);

    QFontMetrics fm(painter.fontMetrics());

    const int extraAreaWidth = d->extraArea->width();

    painter.fillRect(event->rect(), palette.color(QPalette::Base));
    painter.fillRect(event->rect().intersected(QRect(0, 0, extraAreaWidth, INT_MAX)), palette.color(QPalette::Background));

    QTextBlock block = this->firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top;

    while (block.isValid() && top <= event->rect().bottom()) {

        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        QTextBlock nextBlock = block.next();

        QTextBlock nextVisibleBlock = nextBlock;
        int nextVisibleBlockNumber = blockNumber + 1;

        if (!nextVisibleBlock.isVisible()) {
#if QT_VERSION >= 0x040500
            nextVisibleBlock = this->document()->findBlockByLineNumber(nextVisibleBlock.firstLineNumber());
#else
	    nextVisibleBlock = this->document()->findBlockByNumber(nextVisibleBlock.blockNumber());
#endif
            nextVisibleBlockNumber = nextVisibleBlock.blockNumber();
        }

        painter.setPen(palette.color(QPalette::Dark));

        if (d->showRevisions && block.revision() != documentLayout->lastSaveRevision) {
            painter.save();
            painter.setRenderHint(QPainter::Antialiasing, false);
            if (block.revision() < 0)
                painter.setPen(QPen(Qt::darkGreen, 2));
            else
                painter.setPen(QPen(Qt::red, 2));
            painter.drawLine(extraAreaWidth-1, top, extraAreaWidth-1, bottom-1);
            painter.restore();
        }

        if (d->showLineNumbers) {
            const QString &number = QString::number(blockNumber + 1);
            painter.drawText(0, top, extraAreaWidth - 4, fm.height(), Qt::AlignRight, number);
        }

        block = nextVisibleBlock;
        blockNumber = nextVisibleBlockNumber;
    }
}

void mafTextEditor::onBlockCountChanged(int) // >> slotUpdateExtraAreaWidth in basetexteditor.cpp
{   
    this->onUpdateExtraAreaWidth();
}

void mafTextEditor::onUpdateExtraAreaWidth(void)
{
    this->setViewportMargins(this->extraAreaWidth(), 0, 0, 0);
}

void mafTextEditor::onModificationChanged(bool changed)
{
    if (changed)
        return;

    mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout*>(this->document()->documentLayout());
    int oldLastSaveRevision = documentLayout->lastSaveRevision;
    documentLayout->lastSaveRevision = this->document()->revision();

    if (oldLastSaveRevision != documentLayout->lastSaveRevision) {
        QTextBlock block = this->document()->begin();
        while (block.isValid()) {
            if (block.revision() < 0 || block.revision() != oldLastSaveRevision) {
                block.setRevision(-documentLayout->lastSaveRevision - 1);
            } else {
                block.setRevision(documentLayout->lastSaveRevision);
            }
            block = block.next();
        }
    }
    d->extraArea->update();
}

void mafTextEditor::onUpdateRequest(const QRect &r, int dy)
{
    if (dy)
        d->extraArea->scroll(0, dy);
    else if (r.width() > 4) { // wider than cursor width, not just cursor blinking
        d->extraArea->update(0, r.y(), d->extraArea->width(), r.height());
    }

    if (r.contains(viewport()->rect()))
        this->onUpdateExtraAreaWidth();
}

void mafTextEditor::onCursorPositionChanged(void)
{
    this->viewport()->update();
}

QString mafTextEditor::textUnderCursor(void) const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

// /////////////////////////////////////////////////////////////////
// CREDITS
// /////////////////////////////////////////////////////////////////

/*******************************************************************
 **
 ** Some parts of this file are part of Qt Creator
 **
 ** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
 **
 ** Contact:  Qt Software Information (qt-info@nokia.com)
 **
 ** Commercial Usage
 **
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 ** GNU Lesser General Public License Usage
 **
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** If you are unsure which license is appropriate for your use, please
 ** contact the sales department at qt-sales@nokia.com.
 **
 ******************************************************************/
