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


#include "mafTextEditor.h"
#include "mafTextEditorPreferencesWidget.h"

#include "ui_mafTextEditorPreferencesWidget.h"

namespace mafGUI {
	
class mafTextEditorPreferencesWidgetPrivate
{
public:
    Ui::mafTextEditorPreferencesWidget ui;

    mafTextEditor *editor;

    bool showLineNumbers;
    bool showCurrentLine;
    bool showRevisions;

    QFont font;
};

}  // end namespace

using namespace mafGUI;

mafTextEditorPreferencesWidget::mafTextEditorPreferencesWidget(mafTextEditor *editor, QWidget *parent) : mafGUIApplicationSettingsPage(parent)
{
    m_PrivateClassPointer = new mafTextEditorPreferencesWidgetPrivate;

    m_PrivateClassPointer->editor = editor;
    m_PrivateClassPointer->font   = editor->document()->defaultFont();

    m_PrivateClassPointer->showLineNumbers = editor->showLineNumbers();
    m_PrivateClassPointer->showCurrentLine = editor->showCurrentLine();
    m_PrivateClassPointer->showRevisions = editor->showRevisions();

    m_PrivateClassPointer->ui.setupUi(this);

    m_PrivateClassPointer->ui.fontComboBox->setFont(m_PrivateClassPointer->font);
    m_PrivateClassPointer->ui.sizeComboBox->setCurrentIndex(
            m_PrivateClassPointer->font.pointSize() ==  6 ? 0 :
            m_PrivateClassPointer->font.pointSize() ==  8 ? 1 :
            m_PrivateClassPointer->font.pointSize() ==  9 ? 2 :
            m_PrivateClassPointer->font.pointSize() == 10 ? 3 :
            m_PrivateClassPointer->font.pointSize() == 11 ? 4 :
            m_PrivateClassPointer->font.pointSize() == 12 ? 5 :
            m_PrivateClassPointer->font.pointSize() == 14 ? 6 :
            m_PrivateClassPointer->font.pointSize() == 16 ? 7 :
            m_PrivateClassPointer->font.pointSize() == 18 ? 8 : 5);

    m_PrivateClassPointer->ui.numbersCheckBox->setCheckState(m_PrivateClassPointer->showLineNumbers ? Qt::Checked : Qt::Unchecked);
    m_PrivateClassPointer->ui.highlightCheckBox->setCheckState(m_PrivateClassPointer->showCurrentLine ? Qt::Checked : Qt::Unchecked);
    m_PrivateClassPointer->ui.revisionsCheckBox->setCheckState(m_PrivateClassPointer->showRevisions ? Qt::Checked : Qt::Unchecked);

    connect(m_PrivateClassPointer->ui.fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(onFontChosen(QFont)));
    connect(m_PrivateClassPointer->ui.sizeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSizeChosen(QString)));

    connect(m_PrivateClassPointer->ui.numbersCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(onNumbersCkeckBoxChanged(int)));
    connect(m_PrivateClassPointer->ui.highlightCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onHighlightCkeckBoxChanged(int)));
    connect(m_PrivateClassPointer->ui.revisionsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onRevisionsCkeckBoxChanged(int)));

    connect(m_PrivateClassPointer->ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOkButtonClicked()));
    connect(m_PrivateClassPointer->ui.buttonBox, SIGNAL(rejected()), this, SLOT(onCancelButtonClicked()));
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
    m_PrivateClassPointer->font = font;
}

void mafTextEditorPreferencesWidget::onSizeChosen(QString size)
{
    m_PrivateClassPointer->font.setPointSize(size.toInt());
}

void mafTextEditorPreferencesWidget::onNumbersCkeckBoxChanged(int state)
{
    m_PrivateClassPointer->showLineNumbers = state;
}

void mafTextEditorPreferencesWidget::onHighlightCkeckBoxChanged(int state)
{
    m_PrivateClassPointer->showCurrentLine = state;
}

void mafTextEditorPreferencesWidget::onRevisionsCkeckBoxChanged(int state)
{
    m_PrivateClassPointer->showRevisions = state;
}

void mafTextEditorPreferencesWidget::onOkButtonClicked(void)
{
    m_PrivateClassPointer->editor->setFont(m_PrivateClassPointer->font);
    m_PrivateClassPointer->editor->setShowLineNumbers(m_PrivateClassPointer->showLineNumbers);
    m_PrivateClassPointer->editor->setShowCurrentLine(m_PrivateClassPointer->showCurrentLine);
    m_PrivateClassPointer->editor->setShowRevisions(m_PrivateClassPointer->showRevisions);

    this->hide();

    emit accepted();
}

void mafTextEditorPreferencesWidget::onCancelButtonClicked(void)
{
    this->hide();

    emit rejected();
}
