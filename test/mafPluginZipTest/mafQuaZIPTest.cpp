/*
 *  mafQuaZIPTest.cpp
 *  mafPluginZipTest
 *
 *  Created by Matteo Giacomoni on 06/07/12.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafTestConfig.h>

#include <mafQuaZIP.h>
#include <QFile>
#include <QTextStream>

using namespace mafCore;
using namespace mafPluginZip;


/**
 Class name: mafQuaZIPTest
 This class implements the test suite for mafImporterVTK.
 */
class mafQuaZIPTest: public QObject {
    Q_OBJECT

    /// Prepare the test data to be used into the test suite.
    void initializeTestData(){};
    
private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
    }

    /// test for unZip() method
    void unZipTest();

    /// test zip() method
    void zipTest();
    

private:

  QString m_ZipFile; ///< ZIP filename to import.

};


void mafQuaZIPTest::unZipTest() {

  QString resultFileName = MAF_DATA_DIR;
  resultFileName.append("/ZIP/zipTest.txt");
  QFile::remove(resultFileName);

  m_ZipFile = MAF_DATA_DIR;
  m_ZipFile.append("/ZIP/mafTestZIPTestData.zip");

  mafQuaZIP quaZipTool(m_ZipFile);
  QVERIFY(quaZipTool.unZip());

  QVERIFY(QFile::exists(resultFileName));


  QFile fileResult(resultFileName);
  QVERIFY(fileResult.open(QIODevice::ReadOnly));
  QTextStream stream ( &fileResult );
  QString line;
  while( !stream.atEnd()) {
    line = stream.readLine();
    
    QVERIFY(line == "TEST");
  }
  fileResult.close();

}

void mafQuaZIPTest::zipTest() {

  m_ZipFile = MAF_DATA_DIR;
  m_ZipFile.append("/ZIP/mafTestZIPTestDataToZip.zip");
  
  QString file1Name = MAF_DATA_DIR;
  file1Name.append("/ZIP/file1.txt");
  QFile file1(file1Name);
  file1.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out1(&file1);
  out1 << "TEST 1\n";
  file1.close(); 


  mafQuaZIP quaZipTool(m_ZipFile);
  quaZipTool.addFileToArchive(file1Name);
  QVERIFY(quaZipTool.zip());

  QFile::remove(file1Name);

  quaZipTool.unZip();

  QVERIFY(file1.open(QIODevice::ReadOnly));
  QTextStream stream ( &file1 );
  QString line;
  while( !stream.atEnd()) {
    line = stream.readLine();

    QVERIFY(line == "TEST 1");
  }
  file1.close();
}




MAF_REGISTER_TEST(mafQuaZIPTest);
#include "mafQuaZIPTest.moc"
