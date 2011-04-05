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

#include <mafCoreDefinitions.h>
#include <mafObjectFactory.h>
#include <mafCoreSingletons.h>
#include <mafCoreRegistration.h>
#include <mafObjectBase.h>
#include <mafContainer.h>

#include <mafEventBusManager.h>

namespace mafGUI {

/// rule for determines which pattern the text will be highlighted and with which format
struct mafHighlightingRule {
    QRegExp m_Pattern; ///< pattern which represents the keyword need to be highlighted
    QTextCharFormat m_Format; ///< which kind of highlight need to be applied
};

typedef enum {
    mafGUILoadedTypeOperation,
    mafGUILoadedTypeView,
    mafGUILoadedTypeVisualPipe,
    mafGUILoadedTypeVme
} mafGUILoadedType;

} //end namespace

#endif // MAFGUIDEFINITIONS_H
