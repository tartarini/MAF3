/*
 *  mafInterpreterPreferencesWidget.cpp
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <QtGui>
#include <QtGui/QColorDialog>

#include "mafInterpreterPreferencesWidget.h"

#include "ui_mafInterpreterPreferencesWidget.h"

using namespace mafGUI;
using namespace mafScriptInterpreter;
/**
 Class Name: mafInterpreterPreferencesWidgetPrivate.
 pimpl pattern.
 */
class mafScriptInterpreter::mafInterpreterPreferencesWidgetPrivate
{
public:
    Ui::mafInterpreterPreferencesWidget ui; ///< user interface.

    QFont  font;
    QColor foregroundColor;
    QColor backgroundColor;
};

mafInterpreterPreferencesWidget::mafInterpreterPreferencesWidget(QWidget *parent) : mafGUIApplicationSettingsPage(parent) {
    m_PageText = mafTr("Script Interpreter");
    m_PageIcon = QIcon(":/images/config.png");

    m_PrivateClassPointer = new mafInterpreterPreferencesWidgetPrivate;

    QSettings settings;
    m_PrivateClassPointer->font            = settings.value("editor/font").value<QFont>();
    settings.beginGroup("interpreter");
    m_PrivateClassPointer->foregroundColor = settings.value("foregroundcolor", Qt::black).value<QColor>();
    m_PrivateClassPointer->backgroundColor = settings.value("backgroundcolor", Qt::white).value<QColor>();
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

    { // -- Setting text color
    m_PrivateClassPointer->ui.textColorToolButton->setIconSize(m_PrivateClassPointer->ui.textColorToolButton->size());
    QPixmap pix(m_PrivateClassPointer->ui.textColorToolButton->minimumSize());
    pix.fill(m_PrivateClassPointer->foregroundColor);
    m_PrivateClassPointer->ui.textColorToolButton->setIcon(pix);
    } // -- Setting text color

    { // -- Setting background color
    m_PrivateClassPointer->ui.backgroundColorToolButton->setIconSize(m_PrivateClassPointer->ui.backgroundColorToolButton->size());
    QPixmap pix(m_PrivateClassPointer->ui.backgroundColorToolButton->minimumSize());
    pix.fill(m_PrivateClassPointer->backgroundColor);
    m_PrivateClassPointer->ui.backgroundColorToolButton->setIcon(pix);
    } // -- Setting background color

    connect(m_PrivateClassPointer->ui.fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(onFontChosen(QFont)));
    connect(m_PrivateClassPointer->ui.sizeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSizeChosen(QString)));

    connect(m_PrivateClassPointer->ui.textColorToolButton, SIGNAL(clicked()), this, SLOT(onTextColorClicked()));
    connect(m_PrivateClassPointer->ui.backgroundColorToolButton, SIGNAL(clicked()), this, SLOT(onBackgroundColorClicked()));
}

mafInterpreterPreferencesWidget::~mafInterpreterPreferencesWidget(void) {

}

void mafInterpreterPreferencesWidget::writeSettings(void) {
    QSettings settings;
    settings.setValue("editor/font", m_PrivateClassPointer->font);
    settings.beginGroup("interpreter");
    settings.setValue("backgroundcolor", m_PrivateClassPointer->backgroundColor);
    settings.setValue("foregroundcolor", m_PrivateClassPointer->foregroundColor);
    settings.endGroup();
    settings.sync();
}

void mafInterpreterPreferencesWidget::onFontChosen(QFont font) {
    m_PrivateClassPointer->font = font;
    writeSettings();
}

void mafInterpreterPreferencesWidget::onSizeChosen(QString size) {
    QFont f = m_PrivateClassPointer->font;
    f.setPointSize(size.toInt());
    m_PrivateClassPointer->font = f;
    writeSettings();
}

void mafInterpreterPreferencesWidget::onTextColorClicked(void) {
#if QT_VERSION >= 0x040500
    m_PrivateClassPointer->foregroundColor = QColorDialog::getColor(m_PrivateClassPointer->foregroundColor, this, "Choose text color");
#else
    m_PrivateClassPointer->foregroundColor = QColorDialog::getColor(m_PrivateClassPointer->foregroundColor, this);
#endif

    QPixmap pix(m_PrivateClassPointer->ui.textColorToolButton->size());
    pix.fill(m_PrivateClassPointer->foregroundColor);
    m_PrivateClassPointer->ui.textColorToolButton->setIcon(pix);
    
    //m_PrivateClassPointer->interpreter->setForegroundColor(m_PrivateClassPointer->foregroundColor);
    writeSettings();
}

void mafInterpreterPreferencesWidget::onBackgroundColorClicked(void) {
#if QT_VERSION >= 0x040500
    m_PrivateClassPointer->backgroundColor = QColorDialog::getColor(m_PrivateClassPointer->backgroundColor, this, "Choose background color");
#else
    m_PrivateClassPointer->backgroundColor = QColorDialog::getColor(m_PrivateClassPointer->backgroundColor, this);
#endif

    QPixmap pix(m_PrivateClassPointer->ui.backgroundColorToolButton->size());
    pix.fill(m_PrivateClassPointer->backgroundColor);
    m_PrivateClassPointer->ui.backgroundColorToolButton->setIcon(pix);
    
    
    //m_PrivateClassPointer->interpreter->setBackgroundColor(m_PrivateClassPointer->backgroundColor);    
    writeSettings();
}
