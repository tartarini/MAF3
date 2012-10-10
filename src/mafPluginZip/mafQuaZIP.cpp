/*
 *  mafQuaZIP.h
 *  mafPluginZip
 *
 *  Created by Matteo Giacomoni on 03/07/12.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafQuaZIP.h"
#include <quazip.h>
#include <quazipfile.h>

using namespace mafPluginZip;
using namespace mafEventBus;

mafQuaZIP::mafQuaZIP(QString fileName) : mafCore::mafObjectBase()
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

  QFileInfo info(m_fileName);
  QString path = info.absolutePath();

  QuaZipFile file(&zip);

  for(bool f=zip.goToFirstFile(); f; f=zip.goToNextFile()) {
    
    QString name = zip.getCurrentFileName();

    QString fullPath = path;
    fullPath.append("/");
    fullPath.append(name);
    QFile out(fullPath);
    out.open(QIODevice::WriteOnly);

    file.open(QIODevice::ReadOnly);

    char c;
    while (file.getChar(&c))
    {
      out.putChar(c);      
    }

    file.close();

    out.flush();
    out.close();
  }

  zip.close();

  return true;
}

void mafPluginZip::mafQuaZIP::setFileName( QString fileName )
{
  m_fileName = fileName;
}

bool mafPluginZip::mafQuaZIP::zip()
{
//   QuaZip zip(m_fileName);
//   qDebug() << zip.open(QuaZip::mdCreate);
// 
//   QByteArray fileNameCodec;
//   if (!fileNameCodec.isEmpty())
//     zip.setFileNameCodec(fileNameCodec);
// 
// 
//   foreach (QString fileName, m_filesToArchive) {
//     QFile inFile(fileName);
//     if (!inFile.open(QIODevice::ReadOnly)) {
//       qDebug("File name: %s", fileName.toUtf8().constData());
// 
//       return false;
//     }
//     QuaZipFile outFile(&zip);
//     bool result = outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileName, inFile.fileName()));
// 
//     if (!result)
//     {
//       return false;
//     }
//     for (qint64 pos = 0, len = inFile.size(); pos < len; ) {
//       char buf[4096];
//       qint64 readSize = qMin(static_cast<qint64>(4096), len - pos);
//       qint64 l;
//       if ((l = inFile.read(buf, readSize)) != readSize) {
//         qDebug("Reading %ld bytes from %s at %ld returned %ld",
//           static_cast<long>(readSize),
//           fileName.toUtf8().constData(),
//           static_cast<long>(pos),
//           static_cast<long>(l));
//         return false;
//       }
//       result = outFile.write(buf, readSize);
// 
//       if (!result)
//       {
//         return false;
//       }
// 
//       pos += readSize;
//     }
//     inFile.close();
//     outFile.close();
//     if (outFile.getZipError() != ZIP_OK)
//     {
//       return false;
//     }
//     
//   }
// 
//   zip.close();
// 
//   return true;

  QuaZip zip(m_fileName);
  if (!zip.open(QuaZip::mdCreate)) {
    qWarning("Couldn't open %s", m_fileName.toUtf8().constData());
    return false;
  }
  foreach (QString fileName, m_filesToArchive) {
    QuaZipFile zipFile(&zip);

    QString filePath = fileName;
    QFileInfo fileInfo(fileName);
    if (!zipFile.open(QIODevice::WriteOnly, 
      QuaZipNewInfo(QFileInfo(fileName).fileName(), QFileInfo(fileName).filePath()), NULL, 0, 
      fileInfo.isDir() ? 0 : 8)) {
        qWarning("Couldn't open %s in %s", fileName.toUtf8()
          .constData(), m_fileName.toUtf8().constData());
        return false;
    }
    if (!fileInfo.isDir()) {
      QFile file(filePath);
      if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open %s", filePath.toUtf8()
          .constData());
        return false;
      }
      while (!file.atEnd()) {
        char buf[4096];
        qint64 l = file.read(buf, 4096);
        if (l <= 0) {
          qWarning("Couldn't read %s", filePath.toUtf8()
            .constData());
          return false;
        }
        if (zipFile.write(buf, l) != l) {
          qWarning("Couldn't write to %s in %s", 
            filePath.toUtf8().constData(),
            m_fileName.toUtf8().constData());
          return false;
        }
      }
      file.close();
    }
    zipFile.close();
  }
  zip.close();
  return true;
}

void mafPluginZip::mafQuaZIP::addFileToArchive( QString fileName )
{
  m_filesToArchive.push_back(fileName);
}
