/*
 *  mafQuaZIP.h
 *  mafPluginZip
 *
 *  Created by Matteo Giacomoni on 03/07/12.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#ifndef MAFQUAZIP_H
#define MAFQUAZIP_H

#include "mafPluginZipDefinitions.h"

//----------------------------------------------------------------------------
// forward declarations :
//----------------------------------------------------------------------------

namespace mafPluginZip {

/**
Class name: mafQuaZIP

wrap class fro QuaZIP library.
Manage zip file: it's possibile to Zip and Unzip files
*/
class MAFPLUGINZIPSHARED_EXPORT mafQuaZIP {
public:
    mafQuaZIP(QString fileName);
    virtual ~mafQuaZIP();

    /// Zip a collection of files
    void zip(){};

    /// unZip a zip file
    bool unZip();

    /// set m_fileName
    void SetFileName(QString fileName);

private:

  QString m_fileName;

};

} // namespace mafPluginZip

#endif // MAFQUAZIP_H
