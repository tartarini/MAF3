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


#include "mafQuaZIP.h"
#include <quazip.h>
#include <quazipfile.h>

using namespace mafPluginZip;
using namespace mafEventBus;

mafQuaZIP::mafQuaZIP(QString fileName) 
{
  m_fileName = fileName;
}
mafQuaZIP::~mafQuaZIP() 
{
}
bool mafPluginZip::mafQuaZIP::unZip()
{
  if (m_fileName.isEmpty())
  {
    return false;
  }

  QuaZip zip(m_fileName);
  qDebug() << zip.open(QuaZip::mdUnzip);

  QuaZipFile file(&zip);
  QFile out("application.app");
  out.open(QIODevice::WriteOnly);

  for(bool f=zip.goToFirstFile(); f; f=zip.goToNextFile()) {
    
    QString name = file.getFileName();

    file.open(QIODevice::ReadOnly);
    //same functionality as QIODevice::readData() -- data is a char*, maxSize is qint64
    char c;
    while (file.getChar(&c))
    {
      out.putChar(c);      
    }

    file.close();
  }

  out.flush();
  out.close();

  zip.close();

  return true;
}

void mafPluginZip::mafQuaZIP::SetFileName( QString fileName )
{
  m_fileName = fileName;
}
