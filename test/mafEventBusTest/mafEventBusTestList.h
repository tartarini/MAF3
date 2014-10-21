/*
 *  mafEventBusTestList.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventBusManagerTest.h"
#include "mafEventDispatcherLocalTest.h"
#include "mafEventDispatcherRemoteTest.h"
#include "mafEventDispatcherTest.h"
#include "mafEventTest.h"
#include "mafNetworkConnectorTest.h"
#include "mafTopicRegistryTest.h"

MAF_REGISTER_TEST(mafEventBusManagerTest);
MAF_REGISTER_TEST(mafEventDispatcherLocalTest);
MAF_REGISTER_TEST(mafEventDispatcherRemoteTest);
MAF_REGISTER_TEST(mafEventDispatcherTest);
MAF_REGISTER_TEST(mafEventTest);
MAF_REGISTER_TEST(mafNetworkConnectorTest);
MAF_REGISTER_TEST(mafTopicRegistryTest);
