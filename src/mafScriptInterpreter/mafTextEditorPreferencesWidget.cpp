/* mafTextEditorPreferencesWidget.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 09:24:22 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Wed Dec 23 10:40:58 2009 (+0100)
 *           By: Julien Wintz
 *     Update #: 23
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <QtGui>

#include <mafTextEditor.h>
#include <mafTextEditorPreferencesWidget.h>

#include "ui_mafTextEditorPreferencesWidget.h"

using namespace mafGUI;
using namespace mafScriptInterpreter;

class mafScriptInterpreter::mafTextEditorPreferencesWidgetPrivate
{
public:
    Ui::mafTextEditorPreferencesWidget ui;

    mafTextEditor *editor;

    bool showLineNumbers;
    bool showCurrentLine;
    bool showRevisions;

    QFont font;
};

mafTextEditorPreferencesWidget::mafTextEditorPreferencesWidget(mafTextEditor *editor, QWidget *parent) : mafGUIApplicationSettingsPage(parent)
{
    d = new mafTextEditorPreferencesWidgetPrivate;

    d->editor = editor;
    d->font   = editor->document()->defaultFont();

    d->showLineNumbers = editor->showLineNumbers();
    d->showCurrentLine = editor->showCurrentLine();
    d->showRevisions = editor->showRevisions();

    d->ui.setupUi(this);

    d->ui.fontComboBox->setFont(d->font);
    d->ui.sizeComboBox->setCurrentIndex(
            d->font.pointSize() ==  6 ? 0 :
            d->font.pointSize() ==  8 ? 1 :
            d->font.pointSize() ==  9 ? 2 :
            d->font.pointSize() == 10 ? 3 :
            d->font.pointSize() == 11 ? 4 :
            d->font.pointSize() == 12 ? 5 :
            d->font.pointSize() == 14 ? 6 :
            d->font.pointSize() == 16 ? 7 :
            d->font.pointSize() == 18 ? 8 : 5);

    d->ui.numbersCheckBox->setCheckState(d->showLineNumbers ? Qt::Checked : Qt::Unchecked);
    d->ui.highlightCheckBox->setCheckState(d->showCurrentLine ? Qt::Checked : Qt::Unchecked);
    d->ui.revisionsCheckBox->setCheckState(d->showRevisions ? Qt::Checked : Qt::Unchecked);

    connect(d->ui.fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(onFontChosen(QFont)));
    connect(d->ui.sizeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSizeChosen(QString)));

    connect(d->ui.numbersCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(onNumbersCkeckBoxChanged(int)));
    connect(d->ui.highlightCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onHighlightCkeckBoxChanged(int)));
    connect(d->ui.revisionsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onRevisionsCkeckBoxChanged(int)));

    connect(d->ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOkButtonClicked()));
    connect(d->ui.buttonBox, SIGNAL(rejected()), this, SLOT(onCancelButtonClicked()));
}

mafTextEditorPreferencesWidget::~mafTextEditorPreferencesWidget(void)
{

}

void mafTextEditorPreferencesWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:        
        this->hide();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        this->onOkButtonClicked();
        break;
    default:
        break;
    }
}

void mafTextEditorPreferencesWidget::onFontChosen(QFont font)
{
    d->font = font;
}

void mafTextEditorPreferencesWidget::onSizeChosen(QString size)
{
    d->font.setPointSize(size.toInt());
}

void mafTextEditorPreferencesWidget::onNumbersCkeckBoxChanged(int state)
{
    d->showLineNumbers = state;
}

void mafTextEditorPreferencesWidget::onHighlightCkeckBoxChanged(int state)
{
    d->showCurrentLine = state;
}

void mafTextEditorPreferencesWidget::onRevisionsCkeckBoxChanged(int state)
{
    d->showRevisions = state;
}

void mafTextEditorPreferencesWidget::onOkButtonClicked(void)
{
    d->editor->setFont(d->font);
    d->editor->setShowLineNumbers(d->showLineNumbers);
    d->editor->setShowCurrentLine(d->showCurrentLine);
    d->editor->setShowRevisions(d->showRevisions);

    this->hide();

    emit accepted();
}

void mafTextEditorPreferencesWidget::onCancelButtonClicked(void)
{
    this->hide();

    emit rejected();
}
