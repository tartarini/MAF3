/*
 *  mafGUIDefinitions.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 27/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFGUIDEFINITIONS_H
#define MAFGUIDEFINITIONS_H

#include "mafGUI_global.h"

#include <QWidget>
#include <QUiLoader>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

#define mafUiLoader QUiLoader

#include <mafCoreDefinitions.h>
#include <mafObjectFactory.h>
#include <mafCoreSingletons.h>
#include <mafCoreRegistration.h>

#include <mafEventBusManager.h>

namespace mafGUI {

#define mafSyntaxHighlighter QSyntaxHighlighter
#define mafRegExp QRegExp
#define mafTextCharFormat QTextCharFormat

/// rule for determines which pattern the text will be highlighted and with which format
struct mafHighlightingRule {
    mafRegExp m_Pattern; ///< pattern which represents the keyword need to be highlighted
    mafTextCharFormat m_Format; ///< which kind of highlight need to be applied
};

} //end namespace

#endif // MAFGUIDEFINITIONS_H
