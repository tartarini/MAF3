/* mafSettingsEditor.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Dec 23 09:54:06 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Dec 23 10:29:20 2009 (+0100)
 *           By: Julien Wintz
 *     Update #: 19
 */

/* Commentary: 
 * See credits at en of file
 */

/* Change log:
 * 
 */

#ifndef MAFSETTINGSEDITOR_H
#define MAFSETTINGSEDITOR_H

#include "mafScriptInterpreter_global.h"

#include <QtCore>
#include <QtGui>

namespace mafScriptInterpreter {

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafSettingsEditorVariantDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    mafSettingsEditorVariantDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    static bool isSupportedType(QVariant::Type type);
    static QString displayText(const QVariant &value);

private:
    mutable QRegExp boolExp;
    mutable QRegExp byteArrayExp;
    mutable QRegExp charExp;
    mutable QRegExp colorExp;
    mutable QRegExp dateExp;
    mutable QRegExp dateTimeExp;
    mutable QRegExp doubleExp;
    mutable QRegExp pointExp;
    mutable QRegExp rectExp;
    mutable QRegExp signedIntegerExp;
    mutable QRegExp sizeExp;
    mutable QRegExp timeExp;
    mutable QRegExp unsignedIntegerExp;
};

// /////////////////////////////////////////////////////////////////
// mafSettingsEditorTree
// /////////////////////////////////////////////////////////////////

class mafSettingsEditorTree : public QTreeWidget
{
    Q_OBJECT

public:
    mafSettingsEditorTree(QWidget *parent = 0);

    void setSettingsObject(QSettings *settings);
    QSize sizeHint() const;

public slots:
    void setAutoRefresh(bool autoRefresh);
    void setFallbacksEnabled(bool enabled);
    void maybeRefresh(void);
    void refresh(void);

protected:
    bool event(QEvent *event);

private slots:
    void updateSetting(QTreeWidgetItem *item);

private:
    void updateChildItems(QTreeWidgetItem *parent);
    QTreeWidgetItem *createItem(const QString &text, QTreeWidgetItem *parent, int index);
    QTreeWidgetItem *childAt(QTreeWidgetItem *parent, int index);
    int childCount(QTreeWidgetItem *parent);
    int findChild(QTreeWidgetItem *parent, const QString &text, int startIndex);
    void moveItemForward(QTreeWidgetItem *parent, int oldIndex, int newIndex);

    QSettings *settings;
    QTimer refreshTimer;
    bool autoRefresh;
    QIcon groupIcon;
    QIcon keyIcon;
};

// /////////////////////////////////////////////////////////////////
// mafSettingsEditor
// /////////////////////////////////////////////////////////////////

class mafSettingsEditorPrivate;

class mafSettingsEditor : public QWidget
{
    Q_OBJECT

public:
     mafSettingsEditor(QWidget *parent = 0);
    ~mafSettingsEditor(void);

    void setSettings(const QString& organisation, const QString& application);

private:
    mafSettingsEditorPrivate *d;
};

} // end namespace
    
#endif

/* Credits: Main part of this file has been inspired by the qt's settings example
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * Contact: Nokia Corporation (qt-info@nokia.com)
 *
 * This file is part of the examples of the Qt Toolkit.
 *
 * Commercial Usage
 * Licensees holding valid Qt Commercial licenses may use this file in
 * accordance with the Qt Commercial License Agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and Nokia.
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * In addition, as a special exception, Nokia gives you certain additional
 * rights.  These rights are described in the Nokia Qt LGPL Exception
 * version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 *
 * If you have questions regarding the use of this file, please contact
 * Nokia at qt-info@nokia.com.
 *
 */
