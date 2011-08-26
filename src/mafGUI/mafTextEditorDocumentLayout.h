/*
 *  mafTextEditorDocumentLayout.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/08/11.
 *  Copyright 2011 SCS S.r.l. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTEXTEDITORDOCUMENTLAYOUT_H
#define MAFTEXTEDITORDOCUMENTLAYOUT_H

#include "mafGUI_global.h"

#include <QPlainTextDocumentLayout>

namespace mafGUI {

class mafTextEditorDocumentLayout : public QPlainTextDocumentLayout {
    Q_OBJECT

public:
    mafTextEditorDocumentLayout(QTextDocument *document) : QPlainTextDocumentLayout(document) {
        m_LastSaveRevision = 0;
    }

    ~mafTextEditorDocumentLayout(void) {}

public:
    int m_LastSaveRevision;
};

}  //end namespace mafGUI

#endif // MAFTEXTEDITORDOCUMENT_H
