/* mafScriptEditorPool.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Jan 26 09:45:37 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Sun Aug  2 14:48:52 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 22
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFSCRIPTINTERPRETERPOOL_H
#define MAFSCRIPTINTERPRETERPOOL_H

#include "mafScriptInterpreter_global.h"
#include "mafScriptInterpreterDefinitions.h"

namespace mafScriptInterpreter {

class mafScriptEditor;
class mafScriptEditorPoolPrivate;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPool : public QObject
{
    Q_OBJECT

public:
    static mafScriptEditorPool *instance(void);

    mafScriptEditor *console(QString type = "");
    mafScriptEditor *python(void);

private:
     mafScriptEditorPool(void);
    ~mafScriptEditorPool(void);

    static mafScriptEditorPool *m_instance;

private:
    mafScriptEditorPoolPrivate *d;
};

} //end namespace

#endif
