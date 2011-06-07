/*
 *  mafPipeData.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeData.h"
#include "mafDataSet.h"
#include "mafVME.h"

using namespace mafResources;

mafPipeData::mafPipeData(const QString code_location) : mafPipe(code_location), m_DecoratorPipe(NULL), m_Output(NULL), m_InPlace(false) {
}

mafPipeData::~mafPipeData() {
    if (!m_InPlace) {
        mafDEL(m_Output);
    }
    mafDEL(m_DecoratorPipe);
}

int mafPipeData::addInput(mafVME *vme) {
    REQUIRE(vme != NULL);
    if(this->inputList()->contains(vme)) {
        qWarning("%s", mafTr("Object %1 already present in input list").arg(vme->objectName()).toAscii().data());
        return this->inputList()->indexOf(vme);
    }

    this->inputList()->append(vme);
    vme->retain();

    setModified();
    return this->inputList()->count() - 1;
}

void mafPipeData::updatePipe(double t) {
    if(m_DecoratorPipe) {
        m_DecoratorPipe->updatePipe(t);
    }
    setModified(false);
}

mafVME *mafPipeData::output(double t) {
    REQUIRE(inputList()->count() > 0);

    if(m_Output == NULL) {
        if (m_InPlace) {
            m_Output = inputList()->at(0);
        } else {
            m_Output = mafNEW(mafResources::mafVME);
        }
        m_Output->setTimestamp(t);
        setModified();
    }

    mafVME *out_vme = m_Output;

    if(modified()) {
        updatePipe(t);
        emit(modifiedObject());
    }

    if(m_DecoratorPipe) {
        out_vme = m_DecoratorPipe->output(t);
    }

    ENSURE(out_vme != NULL);
    return out_vme;
}
