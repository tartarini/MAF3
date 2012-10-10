/*
 *  mafTextEditorPreferencesWidget.h
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafTextEditorPreferencesWidget.h"

#include "ui_mafTextEditorPreferencesWidget.h"

namespace mafGUI {
/**
 Class Name: mafTextEditorPreferencesWidgetPrivate.
 pimpl pattern.
 */
class mafTextEditorPreferencesWidgetPrivate
{
public:
    Ui::mafTextEditorPreferencesWidget ui; ///< user interface.

    bool showLineNumbers; ///< flag for showing line numbers.
    bool showCurrentLine; ///< flag for showing current line highlighting.
    bool showRevisions; ///< flag for showing revisions.

    QFont font; ///< current font.
};

}  // end namespace

using namespace mafGUI;

mafTextEditorPreferencesWidget::mafTextEditorPreferencesWidget(QWidget *parent) : mafGUIApplicationSettingsPage(parent) {
    m_PageText = mafTr("Text Editor");
    m_PageIcon = QIcon(":/images/config.png");

    m_PrivateClassPointer = new mafTextEditorPreferencesWidgetPrivate;

    QSettings settings;
    settings.beginGroup("editor");
    m_PrivateClassPointer->font   = settings.value("font").value<QFont>();
    m_PrivateClassPointer->showLineNumbers = settings.value("showLineNumbers").toBool();
    m_PrivateClassPointer->showCurrentLine = settings.value("showCurrentLine").toBool();
    m_PrivateClassPointer->showRevisions = settings.value("showRevisions").toBool();
    settings.endGroup();

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
}

mafTextEditorPreferencesWidget::~mafTextEditorPreferencesWidget(void) {

}

void mafTextEditorPreferencesWidget::onFontChosen(QFont font) {
    m_PrivateClassPointer->font = font;
    writeSettings();
}

void mafTextEditorPreferencesWidget::onSizeChosen(QString size) {
    m_PrivateClassPointer->font.setPointSize(size.toInt());
    writeSettings();
}

void mafTextEditorPreferencesWidget::onNumbersCkeckBoxChanged(int state) {
    m_PrivateClassPointer->showLineNumbers = state;
    writeSettings();
}

void mafTextEditorPreferencesWidget::onHighlightCkeckBoxChanged(int state) {
    m_PrivateClassPointer->showCurrentLine = state;
    writeSettings();
}

void mafTextEditorPreferencesWidget::onRevisionsCkeckBoxChanged(int state) {
    m_PrivateClassPointer->showRevisions = state;
    writeSettings();
}

void mafTextEditorPreferencesWidget::writeSettings(void) {
    QSettings settings;
    settings.beginGroup("editor");
    settings.setValue("font", m_PrivateClassPointer->font);
    settings.setValue("showRevisions", m_PrivateClassPointer->showRevisions);
    settings.setValue("showCurrentLine", m_PrivateClassPointer->showCurrentLine);
    settings.setValue("showLineNumbers", m_PrivateClassPointer->showLineNumbers);
    settings.endGroup();
    settings.sync();
}
