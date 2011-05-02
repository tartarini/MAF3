/* mafTextEditor.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 09:23:52 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Jul 22 16:32:59 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 47
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFTEXTEDITOR_H
#define MAFTEXTEDITOR_H

#include "mafScriptInterpreter_global.h"
#include <QtGui/QPlainTextEdit>

namespace mafScriptInterpreter {

class mafTextEditorPreferencesWidget;
class mafTextEditorPrivate;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafTextEditorDocumentLayout : public QPlainTextDocumentLayout
{
    Q_OBJECT

public:
    mafTextEditorDocumentLayout(QTextDocument *document) : QPlainTextDocumentLayout(document) {
        lastSaveRevision = 0;
    }

    ~mafTextEditorDocumentLayout(void) {

    }

public:
    int lastSaveRevision;
};

// /////////////////////////////////////////////////////////////////
// mafTextEditorDocument
// /////////////////////////////////////////////////////////////////

class mafTextEditorDocumentPrivate;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafTextEditorDocument : public QObject
{
    Q_OBJECT

public:
     mafTextEditorDocument(void);
    ~mafTextEditorDocument(void);
    
    bool open (const QString& fileName);
    bool save (const QString& fileName = QString());
    bool close(void);

    bool isReadOnly(void) const;
    bool isModified(void) const;

    bool hasDecodingError(void);

    QString fileName(void) const;

    QTextDocument *document(void);

signals:
    void titleChanged(QString);
    void changed(void);

private:
    mafTextEditorDocumentPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// mafTextEditor
// /////////////////////////////////////////////////////////////////

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafTextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
     mafTextEditor(QWidget *parent = 0);
    ~mafTextEditor(void);

    int currentLineNumber(void) const;
    int currentColumnNumber(void) const;

    bool isReadOnly(void) const;
    bool isModified(void) const;

    bool showLineNumbers(void);
    bool showCurrentLine(void);
    bool showRevisions(void);

    QString fileName(void) const;
    QString currentLine(void) const;

    int  backgroundOpacity(void) const;
    QColor backgroundColor(void) const;
    QColor foregroundColor(void) const;

    mafTextEditorPreferencesWidget *preferencesWidget(QWidget *parent = 0);

    void setShowLineNumbers(bool show);
    void setShowCurrentLine(bool show);
    void setShowRevisions(bool show);

    void setBackgroundOpacity(int opacity);
    void setBackgroundColor(QColor color);
    void setForegroundColor(QColor color);

signals:
    void titleChanged(QString);
    void documentChanged(void);
    void closed(void);

public slots:
    bool open(const QString& fileName);
    bool save(const QString& fileName = QString());

    bool close(void);

    void  readSettings(void);
    void writeSettings(void);

    void zoomIn(int range = 1);
    void zoomOut(int range = 1);

protected:
    void closeEvent(QCloseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void changeEvent(QEvent *event);
    void focusInEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);

    bool eventFilter(QObject *object, QEvent *event);

protected:
    friend class mafTextEditorExtraArea;
    int  extraAreaWidth(void) const;
    void extraAreaPaintEvent(QPaintEvent *event);

protected slots:
    void onBlockCountChanged(int);
    void onModificationChanged(bool);
    void onUpdateExtraAreaWidth(void);
    void onCursorPositionChanged(void);
    void onUpdateRequest(const QRect&, int);

private:
    QString textUnderCursor(void) const;

private:
    mafTextEditorPrivate *d;
};

} //end namespace
    
#endif // MAFTEXTEDITOR_H
