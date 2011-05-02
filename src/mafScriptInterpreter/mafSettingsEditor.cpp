/* mafSettingsEditor.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Dec 23 09:56:30 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Dec 23 10:28:47 2009 (+0100)
 *           By: Julien Wintz
 *     Update #: 24
 */

/* Commentary: 
 * See credits at end of file
 */

/* Change log:
 * 
 */

#include "mafSettingsEditor.h"

using namespace mafScriptInterpreter;

mafSettingsEditorVariantDelegate::mafSettingsEditorVariantDelegate(QObject *parent) : QItemDelegate(parent)
{
    boolExp.setPattern("true|false");
    boolExp.setCaseSensitivity(Qt::CaseInsensitive);

    byteArrayExp.setPattern("[\\x00-\\xff]*");
    charExp.setPattern(".");
    colorExp.setPattern("\\(([0-9]*),([0-9]*),([0-9]*),([0-9]*)\\)");
    doubleExp.setPattern("");
    pointExp.setPattern("\\((-?[0-9]*),(-?[0-9]*)\\)");
    rectExp.setPattern("\\((-?[0-9]*),(-?[0-9]*),(-?[0-9]*),(-?[0-9]*)\\)");
    signedIntegerExp.setPattern("-?[0-9]*");
    sizeExp = pointExp;
    unsignedIntegerExp.setPattern("[0-9]*");

    dateExp.setPattern("([0-9]{,4})-([0-9]{,2})-([0-9]{,2})");
    timeExp.setPattern("([0-9]{,2}):([0-9]{,2}):([0-9]{,2})");
    dateTimeExp.setPattern(dateExp.pattern() + "T" + timeExp.pattern());
}

void mafSettingsEditorVariantDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 2) {
        QVariant value = index.model()->data(index, Qt::UserRole);
        if (!isSupportedType(value.type())) {
            QStyleOptionViewItem myOption = option;
            myOption.state &= ~QStyle::State_Enabled;
            QItemDelegate::paint(painter, myOption, index);
            return;
        }
    }

    QItemDelegate::paint(painter, option, index);
}

QWidget *mafSettingsEditorVariantDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & /* option */, const QModelIndex &index) const
{
    if (index.column() != 2)
        return 0;

    QVariant originalValue = index.model()->data(index, Qt::UserRole);
    if (!isSupportedType(originalValue.type()))
        return 0;

    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(false);

    QRegExp regExp;

    switch (originalValue.type()) {
    case QVariant::Bool:
        regExp = boolExp;
        break;
    case QVariant::ByteArray:
        regExp = byteArrayExp;
        break;
    case QVariant::Char:
        regExp = charExp;
        break;
    case QVariant::Color:
        regExp = colorExp;
        break;
    case QVariant::Date:
        regExp = dateExp;
        break;
    case QVariant::DateTime:
        regExp = dateTimeExp;
        break;
    case QVariant::Double:
        regExp = doubleExp;
        break;
    case QVariant::Int:
    case QVariant::LongLong:
        regExp = signedIntegerExp;
        break;
    case QVariant::Point:
        regExp = pointExp;
        break;
    case QVariant::Rect:
        regExp = rectExp;
        break;
    case QVariant::Size:
        regExp = sizeExp;
        break;
    case QVariant::Time:
        regExp = timeExp;
        break;
    case QVariant::UInt:
    case QVariant::ULongLong:
        regExp = unsignedIntegerExp;
        break;
    default:
        ;
    }

    if (!regExp.isEmpty()) {
        QValidator *validator = new QRegExpValidator(regExp, lineEdit);
        lineEdit->setValidator(validator);
    }

    return lineEdit;
}

void mafSettingsEditorVariantDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::UserRole);
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
        lineEdit->setText(displayText(value));
}

void mafSettingsEditorVariantDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (!lineEdit->isModified())
        return;

    QString text = lineEdit->text();
    const QValidator *validator = lineEdit->validator();
    if (validator) {
        int pos;
        if (validator->validate(text, pos) != QValidator::Acceptable)
            return;
    }

    QVariant originalValue = index.model()->data(index, Qt::UserRole);
    QVariant value;

    switch (originalValue.type()) {
    case QVariant::Char:
        value = text.at(0);
        break;
    case QVariant::Color:
        colorExp.exactMatch(text);
        value = QColor(qMin(colorExp.cap(1).toInt(), 255),
                       qMin(colorExp.cap(2).toInt(), 255),
                       qMin(colorExp.cap(3).toInt(), 255),
                       qMin(colorExp.cap(4).toInt(), 255));
        break;
    case QVariant::Date:
        {
            QDate date = QDate::fromString(text, Qt::ISODate);
            if (!date.isValid())
                return;
            value = date;
        }
        break;
    case QVariant::DateTime:
        {
            QDateTime dateTime = QDateTime::fromString(text, Qt::ISODate);
            if (!dateTime.isValid())
                return;
            value = dateTime;
        }
        break;
    case QVariant::Point:
        pointExp.exactMatch(text);
        value = QPoint(pointExp.cap(1).toInt(), pointExp.cap(2).toInt());
        break;
    case QVariant::Rect:
        rectExp.exactMatch(text);
        value = QRect(rectExp.cap(1).toInt(), rectExp.cap(2).toInt(),
                      rectExp.cap(3).toInt(), rectExp.cap(4).toInt());
        break;
    case QVariant::Size:
        sizeExp.exactMatch(text);
        value = QSize(sizeExp.cap(1).toInt(), sizeExp.cap(2).toInt());
        break;
    case QVariant::StringList:
        value = text.split(",");
        break;
    case QVariant::Time:
        {
            QTime time = QTime::fromString(text, Qt::ISODate);
            if (!time.isValid())
                return;
            value = time;
        }
        break;
    default:
        value = text;
        value.convert(originalValue.type());
    }

    model->setData(index, displayText(value), Qt::DisplayRole);
    model->setData(index, value, Qt::UserRole);
}

bool mafSettingsEditorVariantDelegate::isSupportedType(QVariant::Type type)
{
    switch (type) {
    case QVariant::Bool:
    case QVariant::ByteArray:
    case QVariant::Char:
    case QVariant::Color:
    case QVariant::Date:
    case QVariant::DateTime:
    case QVariant::Double:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::Point:
    case QVariant::Rect:
    case QVariant::Size:
    case QVariant::String:
    case QVariant::StringList:
    case QVariant::Time:
    case QVariant::UInt:
    case QVariant::ULongLong:
        return true;
    default:
        return false;
    }
}

QString mafSettingsEditorVariantDelegate::displayText(const QVariant &value)
{
    switch (value.type()) {
    case QVariant::Bool:
    case QVariant::ByteArray:
    case QVariant::Char:
    case QVariant::Double:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::String:
    case QVariant::UInt:
    case QVariant::ULongLong:
        return value.toString();
    case QVariant::Color:
        {
            QColor color = qvariant_cast<QColor>(value);
            return QString("(%1,%2,%3,%4)")
                   .arg(color.red()).arg(color.green())
                   .arg(color.blue()).arg(color.alpha());
        }
    case QVariant::Date:
        return value.toDate().toString(Qt::ISODate);
    case QVariant::DateTime:
        return value.toDateTime().toString(Qt::ISODate);
    case QVariant::Invalid:
        return "<Invalid>";
    case QVariant::Point:
        {
            QPoint point = value.toPoint();
            return QString("(%1,%2)").arg(point.x()).arg(point.y());
        }
    case QVariant::Rect:
        {
            QRect rect = value.toRect();
            return QString("(%1,%2,%3,%4)")
                   .arg(rect.x()).arg(rect.y())
                   .arg(rect.width()).arg(rect.height());
        }
    case QVariant::Size:
        {
            QSize size = value.toSize();
            return QString("(%1,%2)").arg(size.width()).arg(size.height());
        }
    case QVariant::StringList:
        return value.toStringList().join(",");
    case QVariant::Time:
        return value.toTime().toString(Qt::ISODate);
    default:
        break;
    }
    return QString("<%1>").arg(value.typeName());
}

// /////////////////////////////////////////////////////////////////
// mafSettingsEditorTree
// /////////////////////////////////////////////////////////////////

mafSettingsEditorTree::mafSettingsEditorTree(QWidget *parent) : QTreeWidget(parent)
{
    setItemDelegate(new mafSettingsEditorVariantDelegate(this));

    QStringList labels;
    labels << tr("Setting") << tr("Type") << tr("Value");
    setHeaderLabels(labels);
    header()->setResizeMode(0, QHeaderView::Stretch);
    header()->setResizeMode(2, QHeaderView::Stretch);

    settings = 0;
    refreshTimer.setInterval(2000);
    autoRefresh = false;

    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                        QIcon::Normal, QIcon::Off);
    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                        QIcon::Normal, QIcon::On);
    keyIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(maybeRefresh()));
}

void mafSettingsEditorTree::setSettingsObject(QSettings *settings)
{
    delete this->settings;
    this->settings = settings;
    clear();

    if (settings) {
        settings->setParent(this);
        refresh();
        if (autoRefresh)
            refreshTimer.start();
    } else {
        refreshTimer.stop();
    }
}

QSize mafSettingsEditorTree::sizeHint(void) const
{
    return QSize(800, 600);
}

void mafSettingsEditorTree::setAutoRefresh(bool autoRefresh)
{
    this->autoRefresh = autoRefresh;
    if (settings) {
        if (autoRefresh) {
            maybeRefresh();
            refreshTimer.start();
        } else {
            refreshTimer.stop();
        }
    }
}

void mafSettingsEditorTree::setFallbacksEnabled(bool enabled)
{
    if (settings) {
        settings->setFallbacksEnabled(enabled);
        refresh();
    }
}

void mafSettingsEditorTree::maybeRefresh(void)
{
    if (state() != EditingState)
        refresh();
}

void mafSettingsEditorTree::refresh(void)
{
    if (!settings)
        return;

    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
               this, SLOT(updateSetting(QTreeWidgetItem *)));

    settings->sync();
    updateChildItems(0);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
            this, SLOT(updateSetting(QTreeWidgetItem *)));
}

bool mafSettingsEditorTree::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate) {
        if (isActiveWindow() && autoRefresh)
            maybeRefresh();
    }
    return QTreeWidget::event(event);
}

void mafSettingsEditorTree::updateSetting(QTreeWidgetItem *item)
{
    QString key = item->text(0);
    QTreeWidgetItem *ancestor = item->parent();
    while (ancestor) {
        key.prepend(ancestor->text(0) + "/");
        ancestor = ancestor->parent();
    }

    settings->setValue(key, item->data(2, Qt::UserRole));
    if (autoRefresh)
        refresh();
}

void mafSettingsEditorTree::updateChildItems(QTreeWidgetItem *parent)
{
    int dividerIndex = 0;

    foreach (QString group, settings->childGroups()) {
        QTreeWidgetItem *child;
        int childIndex = findChild(parent, group, dividerIndex);
        if (childIndex != -1) {
            child = childAt(parent, childIndex);
            child->setText(1, "");
            child->setText(2, "");
            child->setData(2, Qt::UserRole, QVariant());
            moveItemForward(parent, childIndex, dividerIndex);
        } else {
            child = createItem(group, parent, dividerIndex);
        }
        child->setIcon(0, groupIcon);
        ++dividerIndex;

        settings->beginGroup(group);
        updateChildItems(child);
        settings->endGroup();
    }

    foreach (QString key, settings->childKeys()) {
        QTreeWidgetItem *child;
        int childIndex = findChild(parent, key, 0);

        if (childIndex == -1 || childIndex >= dividerIndex) {
            if (childIndex != -1) {
                child = childAt(parent, childIndex);
                for (int i = 0; i < child->childCount(); ++i)
                    delete childAt(child, i);
                moveItemForward(parent, childIndex, dividerIndex);
            } else {
                child = createItem(key, parent, dividerIndex);
            }
            child->setIcon(0, keyIcon);
            ++dividerIndex;
        } else {
            child = childAt(parent, childIndex);
        }

        QVariant value = settings->value(key);
        if (value.type() == QVariant::Invalid) {
            child->setText(1, "Invalid");
        } else {
            child->setText(1, value.typeName());
        }
        child->setText(2, mafSettingsEditorVariantDelegate::displayText(value));
        child->setData(2, Qt::UserRole, value);
    }

    while (dividerIndex < childCount(parent))
        delete childAt(parent, dividerIndex);
}

QTreeWidgetItem *mafSettingsEditorTree::createItem(const QString &text, QTreeWidgetItem *parent, int index)
{
    QTreeWidgetItem *after = 0;
    if (index != 0)
        after = childAt(parent, index - 1);

    QTreeWidgetItem *item;
    if (parent)
        item = new QTreeWidgetItem(parent, after);
    else
        item = new QTreeWidgetItem(this, after);

    item->setText(0, text);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    return item;
}

QTreeWidgetItem *mafSettingsEditorTree::childAt(QTreeWidgetItem *parent, int index)
{
    if (parent)
        return parent->child(index);
    else
        return topLevelItem(index);
}

int mafSettingsEditorTree::childCount(QTreeWidgetItem *parent)
{
    if (parent)
        return parent->childCount();
    else
        return topLevelItemCount();
}

int mafSettingsEditorTree::findChild(QTreeWidgetItem *parent, const QString &text, int startIndex)
{
    for (int i = startIndex; i < childCount(parent); ++i) {
        if (childAt(parent, i)->text(0) == text)
            return i;
    }
    return -1;
}

void mafSettingsEditorTree::moveItemForward(QTreeWidgetItem *parent, int oldIndex, int newIndex)
{
    for (int i = 0; i < oldIndex - newIndex; ++i)
        delete childAt(parent, newIndex);
}

// /////////////////////////////////////////////////////////////////
// mafSettingsEditor
// /////////////////////////////////////////////////////////////////

class mafScriptInterpreter::mafSettingsEditorPrivate
{
public:
    mafSettingsEditorTree *tree;
};

mafSettingsEditor::mafSettingsEditor(QWidget *parent) : QWidget(parent), d(new mafSettingsEditorPrivate)
{
    d->tree = new mafSettingsEditorTree(this);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->tree);
}

mafSettingsEditor::~mafSettingsEditor(void)
{
    delete d;

    d = NULL;
}

void mafSettingsEditor::setSettings(const QString& organisation, const QString& application)
{
    d->tree->setSettingsObject(new QSettings(organisation, application));
}

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
