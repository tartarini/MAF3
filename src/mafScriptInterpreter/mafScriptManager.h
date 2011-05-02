/* mafScriptManager.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Aug  4 21:01:13 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Tue Aug  4 22:02:31 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 12
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFSCRIPTMANAGER_H
#define MAFSCRIPTMANAGER_H

#include "mafScriptInterpreter_global.h"
#include <QtCore>

namespace mafScriptInterpreter {

class mafScriptEditor;
class mafScriptManagerPrivate;
    
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptManager : public QObject
{
    Q_OBJECT

public:
    static mafScriptManager *instance(void);

    void   initialize(void);
    void uninitialize(void);

    void  readSettings(void);
    void writeSettings(void);

    mafScriptEditor *  loadScript(const QString& path);
    mafScriptEditor *unloadScript(const QString& path);
    mafScriptEditor *reloadScript(const QString& path);

    QString scriptPath(void) const;
    QString modulePath(void) const;

protected:
     mafScriptManager(void);
    ~mafScriptManager(void);

private:
    mafScriptManagerPrivate *d;
};
    
} //end namespace

#endif
