/*
 *  mafTextEditorDocument.h
 *  mafScriptInterpreter
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTEXTEDITORDOCUMENT_H
#define MAFTEXTEDITORDOCUMENT_H

#include "mafGUI_global.h"
#include <QTextDocument>

namespace mafGUI {

// /////////////////////////////////////////////////////////////////
// mafTextEditorDocument
// /////////////////////////////////////////////////////////////////

class mafTextEditorDocumentPrivate;

class MAFGUISHARED_EXPORT mafTextEditorDocument : public QObject {
    Q_OBJECT

public:
     mafTextEditorDocument(void);
    ~mafTextEditorDocument(void);
    
    bool open (const QString& fileName);
    bool save (const QString& fileName = QString());
    bool close(void);

    bool isReadOnly(void) const;
    bool isModified(void) const;

    bool hasDecodingError(void);

    QString fileName(void) const;

    QTextDocument *document(void);

signals:
    void titleChanged(QString);
    void changed(void);

private:
    mafTextEditorDocumentPrivate *m_PrivateClassPointer;
};

} //end namespace mafGUI
    
#endif // MAFTEXTEDITORDOCUMENT_H
