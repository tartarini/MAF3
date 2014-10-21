/*
 *  mafSerializationTestList.h
 *  mafSerializationTestList
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */
 
#include "mafCodecRawASCIITest.h"
#include "mafCodecRawBinaryTest.h"
#include "mafCodecTest.h"
#include "mafCodecXMLTest.h"
//#include "mafCSVReaderTest.h"
#include "mafSerializationManagerTest.h"
#include "mafSerializerFileSystemTest.h"
#include "mafSerializerTest.h"

MAF_REGISTER_TEST(mafCodecRawASCIITest);
MAF_REGISTER_TEST(mafCodecRawBinaryTest);
MAF_REGISTER_TEST(mafCodecTest);
MAF_REGISTER_TEST(mafCodecXMLTest);
//MAF_REGISTER_TEST(mafCSVReaderTest);
MAF_REGISTER_TEST(mafSerializationManagerTest);
MAF_REGISTER_TEST(mafSerializerFileSystemTest);
MAF_REGISTER_TEST(mafSerializerTest);