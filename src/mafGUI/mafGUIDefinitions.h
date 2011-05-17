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
#include <mafProxy.h>

#include <mafEventBusManager.h>

namespace mafGUI {
    
static void mafConnectObjectWithGUI(mafCore::mafObjectBase *obj, QWidget *gui) {
    obj->connectObjectSlotsByName((QObject *)gui);
    // and initialize the widgets value with those contained into the operation's properties.
    obj->initializeUI((QObject *)gui);
    // do the same thing for all the the operations'children
    QObjectList children = obj->children();
    foreach(QObject *child, children) {
        mafCore::mafObjectBase *child_obj = qobject_cast<mafCore::mafObjectBase *>(child);
        if(child_obj) {
            child_obj->connectObjectSlotsByName((QObject *)gui);
            child_obj->initializeUI((QObject *)gui);
        }
    }
}

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
    

typedef enum {
    mafItemStatusCheckable,
    mafItemStatusNotCheckable,
    mafItemStatusMutexCheckable,
} mafItemStatus;

    
typedef enum {
    mafTreeModelStatusUpdate,
    mafTreeModelStatusGenerate,
} mafTreeModelStatus;
    
} //end namespace

#endif // MAFGUIDEFINITIONS_H
