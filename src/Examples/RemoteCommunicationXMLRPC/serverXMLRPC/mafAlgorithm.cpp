/*
 *  mafAlgorithm.cpp
 *  serverXMLRPC
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafAlgorithm.h"
#include <mafIdProvider.h>
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

mafAlgorithm::mafAlgorithm(const QString code_location) : mafOperation(code_location) {
}

void mafAlgorithm::setParameters(QList<QVariant> parameters) {
    m_FileNameInput = parameters.at(0).toString();
    m_FileNameOutput = parameters.at(1).toString();
    m_IterationParameter = parameters.at(2).toInt();
}

void mafAlgorithm::execute() {
    qDebug("%s", mafTr("Executing testProcess!!!").toAscii().data());

    QStringList commandAndParameters;
    commandAndParameters << m_FileNameInput.toAscii()  \
                        << m_FileNameOutput.toAscii() \
                        << QString::number(m_IterationParameter);

    bool ok = QProcess::execute("./testProcess" ,commandAndParameters) == 0;
}
