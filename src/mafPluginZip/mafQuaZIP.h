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

// Includes list
#include "mafPluginZipDefinitions.h"
#include "mafObjectBase.h"

//----------------------------------------------------------------------------
// forward declarations :
//----------------------------------------------------------------------------

namespace mafPluginZip {

/**
Class name: mafQuaZIP

wrap class fro QuaZIP library.
Manage zip file: it's possibile to Zip and Unzip files
*/
class MAFPLUGINZIPSHARED_EXPORT mafQuaZIP : mafCore::mafObjectBase {

  Q_OBJECT

public:
    mafQuaZIP(QString fileName);
    virtual ~mafQuaZIP();

    /// Zip a collection of files
    bool zip();

    /// unZip a zip file
    bool unZip();

    /// set m_fileName
    void setFileName(QString fileName);

    /// add a file to zip archive
    void addFileToArchive(QString fileName);

private:

  QString m_fileName;

  QStringList m_filesToArchive;

};

} // namespace mafPluginZip

#endif // MAFQUAZIP_H
