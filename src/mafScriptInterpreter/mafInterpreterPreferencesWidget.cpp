/* mafInterpreterPreferencesWidget.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 09:24:22 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Aug  6 10:24:27 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 54
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <QtGui>
#include <QtGui/QColorDialog>

#include "mafInterpreterConsole.h"
#include "mafInterpreterPreferencesWidget.h"

#include "ui_mafInterpreterPreferencesWidget.h"

using namespace mafGUI;
using namespace mafScriptInterpreter;

class mafScriptInterpreter::mafInterpreterPreferencesWidgetPrivate
{
public:
    Ui::mafInterpreterPreferencesWidget ui;

    mafInterpreterConsole *interpreter;

    QFont  font;
    QColor foregroundColor;
    QColor backgroundColor;
    int  backgroundOpacity;
};

mafInterpreterPreferencesWidget::mafInterpreterPreferencesWidget(mafInterpreterConsole *interpreter, QWidget *parent) : mafGUIApplicationSettingsPage(parent)
{
    m_PrivateClassPointer = new mafInterpreterPreferencesWidgetPrivate;
    m_PrivateClassPointer->interpreter     = interpreter;
    m_PrivateClassPointer->font            = interpreter->document()->defaultFont();
    m_PrivateClassPointer->foregroundColor = interpreter->foregroundColor();
    m_PrivateClassPointer->backgroundColor = interpreter->backgroundColor();
    m_PrivateClassPointer->backgroundOpacity = interpreter->backgroundOpacity();

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

    { // -- Setting opacity
    m_PrivateClassPointer->ui.backgroundOpacitySlider->setValue(m_PrivateClassPointer->interpreter->backgroundOpacity());
    } // -- Setting opacity

    connect(m_PrivateClassPointer->ui.fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(onFontChosen(QFont)));
    connect(m_PrivateClassPointer->ui.sizeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSizeChosen(QString)));

    connect(m_PrivateClassPointer->ui.textColorToolButton, SIGNAL(clicked()), this, SLOT(onTextColorClicked()));
    connect(m_PrivateClassPointer->ui.backgroundColorToolButton, SIGNAL(clicked()), this, SLOT(onBackgroundColorClicked()));
    connect(m_PrivateClassPointer->ui.backgroundOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onBackgroundOpacityChanged(int)));
    
    m_PageText = mafTr("Interpreter");
    m_PageIcon = QIcon(":/images/config.png");
}

mafInterpreterPreferencesWidget::~mafInterpreterPreferencesWidget(void)
{

}

void mafInterpreterPreferencesWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        this->hide();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        break;
    default:
        break;
    }
}

void mafInterpreterPreferencesWidget::onFontChosen(QFont font)
{
    m_PrivateClassPointer->interpreter->setFont(font);
}

void mafInterpreterPreferencesWidget::onSizeChosen(QString size)
{
    QFont f = m_PrivateClassPointer->interpreter->font();
    f.setPointSize(size.toInt());
    m_PrivateClassPointer->interpreter->setFont(f);
}

void mafInterpreterPreferencesWidget::onTextColorClicked(void)
{
#if QT_VERSION >= 0x040500
    m_PrivateClassPointer->foregroundColor = QColorDialog::getColor(m_PrivateClassPointer->foregroundColor, this, "Choose text color");
#else
    m_PrivateClassPointer->foregroundColor = QColorDialog::getColor(m_PrivateClassPointer->foregroundColor, this);
#endif

    QPixmap pix(m_PrivateClassPointer->ui.textColorToolButton->size());
    pix.fill(m_PrivateClassPointer->foregroundColor);
    m_PrivateClassPointer->ui.textColorToolButton->setIcon(pix);
    
    m_PrivateClassPointer->interpreter->setForegroundColor(m_PrivateClassPointer->foregroundColor);
}

void mafInterpreterPreferencesWidget::onBackgroundColorClicked(void)
{
#if QT_VERSION >= 0x040500
    m_PrivateClassPointer->backgroundColor = QColorDialog::getColor(m_PrivateClassPointer->backgroundColor, this, "Choose background color");
#else
    m_PrivateClassPointer->backgroundColor = QColorDialog::getColor(m_PrivateClassPointer->backgroundColor, this);
#endif

    QPixmap pix(m_PrivateClassPointer->ui.backgroundColorToolButton->size());
    pix.fill(m_PrivateClassPointer->backgroundColor);
    m_PrivateClassPointer->ui.backgroundColorToolButton->setIcon(pix);
    
    
    m_PrivateClassPointer->interpreter->setBackgroundColor(m_PrivateClassPointer->backgroundColor);    

}

void mafInterpreterPreferencesWidget::onBackgroundOpacityChanged(int value)
{
    m_PrivateClassPointer->interpreter->setBackgroundOpacity(value);

}
