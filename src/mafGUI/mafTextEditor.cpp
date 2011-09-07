/*
 *  mafTextEditor.cpp
 *  mafScriptInterpreter
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTextEditor.h"
#include "mafTextEditorDocument.h"
#include "mafTextEditorDocumentLayout.h"

#include <QPainter>
#include <QStyle>
#include <QSettings>
#include <QTextBlock>


namespace mafGUI {
    class mafTextEditorExtraArea : public QWidget {
    public:
        mafTextEditorExtraArea(mafTextEditor *editor) : QWidget(editor) {
            this->m_Editor = editor;
            this->setAutoFillBackground(true);
        }
        
        QSize sizeHint(void) const {
            return QSize(m_Editor->extraAreaWidth(), 0);
        }
        
    protected:
        void paintEvent(QPaintEvent *event) {
            m_Editor->extraAreaPaintEvent(event);
        }
        
    private:
        mafTextEditor *m_Editor;
    };
} // end namespace mafGUI

// /////////////////////////////////////////////////////////////////
// mafTextEditorPrivate
// /////////////////////////////////////////////////////////////////

class mafGUI::mafTextEditorPrivate {
public:
    bool m_ShowLineNumbers;
    bool m_ShowCurrentLine;
    bool m_ShowRevisions;

    mafTextEditorDocument *m_Document;
    mafTextEditorExtraArea *m_ExtraArea;
};

// /////////////////////////////////////////////////////////////////
// mafTextEditorPrivate
// /////////////////////////////////////////////////////////////////

using namespace mafGUI;

mafTextEditor::mafTextEditor(QWidget *parent) : QPlainTextEdit(parent) {
    m_PrivateClassPointer = new mafTextEditorPrivate;

    m_PrivateClassPointer->m_ExtraArea = new mafTextEditorExtraArea(this);
    m_PrivateClassPointer->m_Document = new mafTextEditorDocument;

    // begin setting up document
    QTextDocument *doc = m_PrivateClassPointer->m_Document->document();
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

    connect(m_PrivateClassPointer->m_Document, SIGNAL(titleChanged(QString)), this, SIGNAL(titleChanged(QString)));

    this->onUpdateExtraAreaWidth();
    this->installEventFilter(this);

    readSettings();
}

mafTextEditor::~mafTextEditor(void) {
    delete m_PrivateClassPointer;
}

void mafTextEditor::readSettings(void) {
    QSettings settings;
    settings.beginGroup("editor");
    this->setFont(settings.value("font").value<QFont>());
    this->setShowRevisions(settings.value("showRevisions").toBool());
    this->setShowCurrentLine(settings.value("showCurrentLine").toBool());
    this->setShowLineNumbers(settings.value("showLineNumbers").toBool());
    settings.endGroup();
}

int mafTextEditor::currentLineNumber(void) const {
    return this->textCursor().blockNumber() + 1;
}

int mafTextEditor::currentColumnNumber(void) const {
    return this->textCursor().position() - this->textCursor().block().position() + 1;
}

bool mafTextEditor::open(const QString& fileName) {
    if (m_PrivateClassPointer->m_Document->open(fileName)) {
        moveCursor(QTextCursor::Start);
        setReadOnly(m_PrivateClassPointer->m_Document->hasDecodingError());

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

bool mafTextEditor::save(const QString& fileName) {    
    return m_PrivateClassPointer->m_Document->save(fileName);
}

bool mafTextEditor::close(void) {
    return m_PrivateClassPointer->m_Document->close();
}

bool mafTextEditor::isReadOnly(void) const
{
    return m_PrivateClassPointer->m_Document->isReadOnly();
}

bool mafTextEditor::isModified(void) const {
    return m_PrivateClassPointer->m_Document->isModified();
}

bool mafTextEditor::showLineNumbers(void) {
    return m_PrivateClassPointer->m_ShowLineNumbers;
}

bool mafTextEditor::showCurrentLine(void) {
    return m_PrivateClassPointer->m_ShowCurrentLine;
}

bool mafTextEditor::showRevisions(void) {
    return m_PrivateClassPointer->m_ShowRevisions;
}

QString mafTextEditor::fileName(void) const {
    return m_PrivateClassPointer->m_Document->fileName();
}

QString mafTextEditor::currentLine(void) const {
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    
    return tc.selectedText();
}

int mafTextEditor::backgroundOpacity(void) const {
    QPalette p(this->palette());
    return p.color(QPalette::Base).alpha();
}

QColor mafTextEditor::backgroundColor(void) const {
    QPalette p(palette());
    return p.color(QPalette::Base);
}

QColor mafTextEditor::foregroundColor(void) const {
    QPalette p(palette());
    return p.color(QPalette::Text);
}

void mafTextEditor::setShowLineNumbers(bool show) {
    m_PrivateClassPointer->m_ShowLineNumbers = show;
    onUpdateExtraAreaWidth();
}

void mafTextEditor::setShowCurrentLine(bool show) {
    m_PrivateClassPointer->m_ShowCurrentLine = show;
}

void mafTextEditor::setShowRevisions(bool show) {
    m_PrivateClassPointer->m_ShowRevisions = show;
    onUpdateExtraAreaWidth();
}

void mafTextEditor::setBackgroundOpacity(int opacity) {
    QPalette p(this->palette());

    QColor color = p.color(QPalette::Base);
    color.setAlpha(opacity);
    p.setColor(QPalette::Window, color);

    this->setPalette(p);
    this->viewport()->update();
}

void mafTextEditor::setBackgroundColor(QColor color) {
    QPalette p(this->palette());
    p.setColor(QPalette::Base, color);

    this->setPalette(p);
    this->viewport()->update();
}

void mafTextEditor::setForegroundColor(QColor color) {
    QPalette p(this->palette());
    p.setColor(QPalette::Text, color);

    this->setPalette(p);
    this->viewport()->update();
}

void mafTextEditor::changeEvent(QEvent *e) {
    QPlainTextEdit::changeEvent(e);

    if (e->type() == QEvent::ApplicationFontChange || e->type() == QEvent::FontChange) {
        if (m_PrivateClassPointer->m_ExtraArea) {
            QFont f = m_PrivateClassPointer->m_ExtraArea->font();
            f.setPointSize(font().pointSize());
            m_PrivateClassPointer->m_ExtraArea->setFont(f);
            onUpdateExtraAreaWidth();
            m_PrivateClassPointer->m_ExtraArea->update();
        }
    }
}

void mafTextEditor::focusInEvent(QFocusEvent *event) {
    QPlainTextEdit::focusInEvent(event);
}

#include <iostream>

void mafTextEditor::keyPressEvent(QKeyEvent *event) {
    QPlainTextEdit::keyPressEvent(event);
}

void mafTextEditor::paintEvent(QPaintEvent *event) {
    const QColor baseColor = palette().base().color();

    const int blendBase   = (baseColor.value() > 128) ? 0 : 255;
    const int blendFactor = (baseColor.value() > 128) ? 8 : 16;

    const QColor blendColor(
        (blendBase * blendFactor + baseColor.blue()  * (256 - blendFactor)) / 256,
        (blendBase * blendFactor + baseColor.green() * (256 - blendFactor)) / 256,
        (blendBase * blendFactor + baseColor.blue()  * (256 - blendFactor)) / 256);

    QPainter painter(viewport());

    if(m_PrivateClassPointer->m_ShowCurrentLine) {
        QRect r = cursorRect();
        r.setX(0);
        r.setWidth(viewport()->width());
        painter.fillRect(r, blendColor);
    }

    painter.end();

    QPlainTextEdit::paintEvent(event);
}

void mafTextEditor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);

    QRect cr = viewport()->rect();
    m_PrivateClassPointer->m_ExtraArea->setGeometry(QStyle::visualRect(layoutDirection(), cr, QRect(cr.left(), cr.top(), extraAreaWidth(), cr.height())));
}

void mafTextEditor::closeEvent(QCloseEvent *event) {
    Q_EMIT closed();
}

void mafTextEditor::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        const int delta = event->delta();
        if (delta < 0) {
            zoomOut();
        } else if (delta > 0) {
            zoomIn();
        }
        return;
    }

    QPlainTextEdit::wheelEvent(event);
}

void mafTextEditor::zoomIn(int range) {
    QFont f = font();
    const int newSize = f.pointSize() + range;
    if (newSize <= 0) {
        return;
    }
    f.setPointSize(newSize);
    setFont(f);
}

void mafTextEditor::zoomOut(int range) {
    zoomIn(-range);
}

int mafTextEditor::extraAreaWidth(void) const {
    if(!m_PrivateClassPointer->m_ShowLineNumbers && !m_PrivateClassPointer->m_ShowRevisions) {
        return 0;
    }

    mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout *>(this->document()->documentLayout());
    if(!documentLayout) {
        return 0;
    }

    int space = 0;
    const QFontMetrics fm(m_PrivateClassPointer->m_ExtraArea->fontMetrics());

    if (m_PrivateClassPointer->m_ShowLineNumbers) {
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

void mafTextEditor::extraAreaPaintEvent(QPaintEvent *event) {
    if(!m_PrivateClassPointer->m_ShowLineNumbers && !m_PrivateClassPointer->m_ShowRevisions) {
        return;
    }

    mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout *>(this->document()->documentLayout());

    QPalette palette = m_PrivateClassPointer->m_ExtraArea->palette();
    palette.setCurrentColorGroup(QPalette::Active);

    QPainter painter(m_PrivateClassPointer->m_ExtraArea);
    QFontMetrics fm(painter.fontMetrics());

    const int extraAreaWidth = m_PrivateClassPointer->m_ExtraArea->width();

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

        if (m_PrivateClassPointer->m_ShowRevisions && block.revision() != documentLayout->m_LastSaveRevision) {
            painter.save();
            painter.setRenderHint(QPainter::Antialiasing, false);
            if (block.revision() < 0)
                painter.setPen(QPen(Qt::darkGreen, 2));
            else
                painter.setPen(QPen(Qt::red, 2));
            painter.drawLine(extraAreaWidth-1, top, extraAreaWidth-1, bottom-1);
            painter.restore();
        }

        if (m_PrivateClassPointer->m_ShowLineNumbers) {
            const QString &number = QString::number(blockNumber + 1);
            painter.drawText(0, top, extraAreaWidth - 4, fm.height(), Qt::AlignRight, number);
        }

        block = nextVisibleBlock;
        blockNumber = nextVisibleBlockNumber;
    }
}

void mafTextEditor::onBlockCountChanged(int) { // >> slotUpdateExtraAreaWidth in basetexteditor.cpp 
    this->onUpdateExtraAreaWidth();
}

void mafTextEditor::onUpdateExtraAreaWidth(void) {
    this->setViewportMargins(this->extraAreaWidth(), 0, 0, 0);
}

void mafTextEditor::onModificationChanged(bool changed) {
    if (changed) {
        return;
    }

    mafTextEditorDocumentLayout *documentLayout = qobject_cast<mafTextEditorDocumentLayout*>(this->document()->documentLayout());
    int oldLastSaveRevision = documentLayout->m_LastSaveRevision;
    documentLayout->m_LastSaveRevision = this->document()->revision();

    if (oldLastSaveRevision != documentLayout->m_LastSaveRevision) {
        QTextBlock block = this->document()->begin();
        while (block.isValid()) {
            if (block.revision() < 0 || block.revision() != oldLastSaveRevision) {
                block.setRevision(-documentLayout->m_LastSaveRevision - 1);
            } else {
                block.setRevision(documentLayout->m_LastSaveRevision);
            }
            block = block.next();
        }
    }
    m_PrivateClassPointer->m_ExtraArea->update();
}

void mafTextEditor::onUpdateRequest(const QRect &r, int dy) {
    if (dy) {
        m_PrivateClassPointer->m_ExtraArea->scroll(0, dy);
    } else if (r.width() > 4) { // wider than cursor width, not just cursor blinking
        m_PrivateClassPointer->m_ExtraArea->update(0, r.y(), m_PrivateClassPointer->m_ExtraArea->width(), r.height());
    }

    if (r.contains(viewport()->rect())) {
        this->onUpdateExtraAreaWidth();
    }
}

void mafTextEditor::onCursorPositionChanged(void) {
    this->viewport()->update();
}

QString mafTextEditor::textUnderCursor(void) const {
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}
