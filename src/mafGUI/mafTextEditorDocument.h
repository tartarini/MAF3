/* mafTextEditorDocument.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 09:23:52 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Jul 22 16:32:59 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 47
 */

/* Commentary: 
 * 
 */

/* Change log:
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
