/*
 *  mafCodec.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 14/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCodec.h"

using namespace mafCore;
using namespace mafSerialization;

mafCodec::mafCodec(const QString code_location) : mafObjectBase(code_location), m_EncodingType(""), m_Device(NULL), m_LevelEncode(0), m_LevelDecode(-1) {
}

mafCodec::~mafCodec() {
}

void mafCodec::buildMementoTree() {
    int listSize = m_MementoList.count();
    int i = 0;
    for(i; i < listSize-1; i++){
        int parentLevel = m_MementoList.at(i).first;
        mafMemento *mementoParent = m_MementoList.at(i).second;
        int childLevel = m_MementoList.at(i+1).first;
        mafMemento *mementoChild = m_MementoList.at(i+1).second;
        int parentRelation = childLevel - parentLevel;
        if (parentRelation > 0) {
            mementoChild->setParent(mementoParent);
        } else {
            //cycle searching for last memento with level parent of chilMemento
            for (int n = i; n >= 0; n--){
                int level = m_MementoList.at(n).first;
                if (level == (childLevel-1)) {
                    mafMemento *mementoParent = m_MementoList.at(n).second;
                    mementoChild->setParent(mementoParent);
                    break;
                }
            }
        }
    }
}

