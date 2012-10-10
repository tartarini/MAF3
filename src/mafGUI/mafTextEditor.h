/*
 *  mafTextEditor.h
 *  mafScriptInterpreter
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 08/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#ifndef MAFTEXTEDITOR_H
#define MAFTEXTEDITOR_H

#include "mafGUI_global.h"
#include <QPlainTextEdit>

namespace mafGUI {

class mafTextEditorPrivate;

/**
 Class Name: mafTextEditor
 Class responsible to show the script console.
*/
class MAFGUISHARED_EXPORT mafTextEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    /// Object constructor
    mafTextEditor(QWidget *parent = 0);
    /// Object destructor
    ~mafTextEditor(void);

    /// Return the current line number of the cursor.
    int currentLineNumber(void) const;
    /// Return the current column number of the cursor.
    int currentColumnNumber(void) const;

    /// Return the status of text area.
    bool isReadOnly(void) const;
    /// Return the modify status.
    bool isModified(void) const;

    /// Allows to show the line numbers.
    bool showLineNumbers(void);
    /// Allows to show the line number on which is positioned the cursor.
    bool showCurrentLine(void);
    /// Allows to show text revisions.
    bool showRevisions(void);

    /// Return the filename associated with the text.
    QString fileName(void) const;
    /// Return the current line (in string format) in which is positioned the cursor.
    QString currentLine(void) const;

    /// Return the background opacity.
    int  backgroundOpacity(void) const;
    /// Return the background color.
    QColor backgroundColor(void) const;
    /// Return the foreground color.
    QColor foregroundColor(void) const;

    /// Allows to visualize the line numbers.
    void setShowLineNumbers(bool show);
    /// Allows to visualize the current line number.
    void setShowCurrentLine(bool show);
    /// Allows to show the revision value.
    void setShowRevisions(bool show);

    /// Allows to set the background opacity
    void setBackgroundOpacity(int opacity);
    /// Allows to set the background color
    void setBackgroundColor(QColor color);
    /// Allows to set the foreground color
    void setForegroundColor(QColor color);

Q_SIGNALS:
    /// Signal used to notify that the console title changed.
    void titleChanged(QString);
    /// Signal used to notify that the document associated with the text widget changed.
    void documentChanged(void);
    /// Signal emitted when the text widget is closed.
    void closed(void);

public Q_SLOTS:
    /// Allows to open a script stored into a file.
    bool open(const QString& fileName);
    /// Allows to save the console content into a file.
    bool save(const QString& fileName = QString());

    /// Close the widget.
    bool close(void);

    /// Read the previous stored settings
    void  readSettings(void);

    /// Zoom in the text area.
    void zoomIn(int range = 1);
    /// Zoom out the text area.
    void zoomOut(int range = 1);

protected:
    /// Intercept the CloseEvent and manage it.
    void closeEvent(QCloseEvent *event);
    /// Intercept the WheelEvent and manage it.
    void wheelEvent(QWheelEvent *event);
    /// Intercept the PaintEvent and manage it.
    void paintEvent(QPaintEvent *event);
    /// Intercept the ResizeEvent and manage it.
    void resizeEvent(QResizeEvent *event);
    /// Intercept the ChangeEvent and manage it.
    void changeEvent(QEvent *event);
    /// Intercept the FocusEvent and manage it.
    void focusInEvent(QFocusEvent *event);
    /// Intercept the KeyPressEvent and manage it.
    void keyPressEvent(QKeyEvent *event);

protected:
    friend class mafTextEditorExtraArea;
    /// Return the dimension of the width of the extra area.
    int  extraAreaWidth(void) const;
    /// Allows to paint the extra area.
    void extraAreaPaintEvent(QPaintEvent *event);

protected Q_SLOTS:
    /// Slot called when count block flag changed.
    void onBlockCountChanged(int);
    /// Slot called when some modification occurred.
    void onModificationChanged(bool);
    /// Slot called when the extra area width is changed.
    void onUpdateExtraAreaWidth(void);
    /// Slot called when the cursor position is changed.
    void onCursorPositionChanged(void);
    /// Slot called when an update is requested.
    void onUpdateRequest(const QRect&, int);

private:
    /// Return the text positioned under the cursor position.
    QString textUnderCursor(void) const;

private:
    mafTextEditorPrivate *m_PrivateClassPointer; ///< private class pointer
};

} //end namespace
    
#endif // MAFTEXTEDITOR_H
