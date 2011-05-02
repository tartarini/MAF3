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

#include "mafInterpreter.h"
#include "mafInterpreterPreferencesWidget.h"

#include "ui_mafInterpreterPreferencesWidget.h"

using namespace mafGUI;
using namespace mafScriptInterpreter;

class mafScriptInterpreter::mafInterpreterPreferencesWidgetPrivate
{
public:
    Ui::mafInterpreterPreferencesWidget ui;

    mafInterpreter *interpreter;

    QFont  font;
    QColor foregroundColor;
    QColor backgroundColor;
    int  backgroundOpacity;
};

mafInterpreterPreferencesWidget::mafInterpreterPreferencesWidget(mafInterpreter *interpreter, QWidget *parent) : mafGUIApplicationSettingsPage(parent)
{
    d = new mafInterpreterPreferencesWidgetPrivate;
    d->interpreter     = interpreter;
    d->font            = interpreter->document()->defaultFont();
    d->foregroundColor = interpreter->foregroundColor();
    d->backgroundColor = interpreter->backgroundColor();
    d->backgroundOpacity = interpreter->backgroundOpacity();

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

    { // -- Setting text color
    d->ui.textColorToolButton->setIconSize(d->ui.textColorToolButton->size());
    QPixmap pix(d->ui.textColorToolButton->minimumSize());
    pix.fill(d->foregroundColor);
    d->ui.textColorToolButton->setIcon(pix);
    } // -- Setting text color

    { // -- Setting background color
    d->ui.backgroundColorToolButton->setIconSize(d->ui.backgroundColorToolButton->size());
    QPixmap pix(d->ui.backgroundColorToolButton->minimumSize());
    pix.fill(d->backgroundColor);
    d->ui.backgroundColorToolButton->setIcon(pix);
    } // -- Setting background color

    { // -- Setting opacity
    d->ui.backgroundOpacitySlider->setValue(d->interpreter->backgroundOpacity());
    } // -- Setting opacity

    connect(d->ui.fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(onFontChosen(QFont)));
    connect(d->ui.sizeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSizeChosen(QString)));

    connect(d->ui.textColorToolButton, SIGNAL(clicked()), this, SLOT(onTextColorClicked()));
    connect(d->ui.backgroundColorToolButton, SIGNAL(clicked()), this, SLOT(onBackgroundColorClicked()));
    connect(d->ui.backgroundOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onBackgroundOpacityChanged(int)));
    
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
    d->interpreter->setFont(font);
}

void mafInterpreterPreferencesWidget::onSizeChosen(QString size)
{
    QFont f = d->interpreter->font();
    f.setPointSize(size.toInt());
    d->interpreter->setFont(f);
}

void mafInterpreterPreferencesWidget::onTextColorClicked(void)
{
#if QT_VERSION >= 0x040500
    d->foregroundColor = QColorDialog::getColor(d->foregroundColor, this, "Choose text color");
#else
    d->foregroundColor = QColorDialog::getColor(d->foregroundColor, this);
#endif

    QPixmap pix(d->ui.textColorToolButton->size());
    pix.fill(d->foregroundColor);
    d->ui.textColorToolButton->setIcon(pix);
    
    d->interpreter->setForegroundColor(d->foregroundColor);
}

void mafInterpreterPreferencesWidget::onBackgroundColorClicked(void)
{
#if QT_VERSION >= 0x040500
    d->backgroundColor = QColorDialog::getColor(d->backgroundColor, this, "Choose background color");
#else
    d->backgroundColor = QColorDialog::getColor(d->backgroundColor, this);
#endif

    QPixmap pix(d->ui.backgroundColorToolButton->size());
    pix.fill(d->backgroundColor);
    d->ui.backgroundColorToolButton->setIcon(pix);
    
    
    d->interpreter->setBackgroundColor(d->backgroundColor);    

}

void mafInterpreterPreferencesWidget::onBackgroundOpacityChanged(int value)
{
    d->interpreter->setBackgroundOpacity(value);

}
