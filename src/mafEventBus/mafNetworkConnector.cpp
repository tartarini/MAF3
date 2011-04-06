/*
 *  mafNetworkConnector.cpp
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafNetworkConnector.h"

using namespace mafEventBus;

mafNetworkConnector::mafNetworkConnector() {

}

QString mafNetworkConnector::protocol() {
    return m_Protocol;
}
