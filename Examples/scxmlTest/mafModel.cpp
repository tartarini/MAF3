#include "mafModel.h"

using namespace mafCore;

mafModel::mafModel(const QString code_location) : mafObjectBase(code_location) {
}

void mafModel::setServiceIP(QString ip) {
//    qDebug() << "ip: " << ip;
    m_Ip = ip;
}

void mafModel::setServicePort(QString port) {
//    qDebug() << "port: " << port;
    m_Port = port;
}

void mafModel::setServiceProtocol(QString protocol) {
//    qDebug() << "protocol: " << protocol;
    m_Protocol = protocol;
}

void mafModel::setParametersList(QVariantList params) {
    m_ParameterList.clear();
    m_ParameterList.append(params);
    qDebug() << "Num of Params: " << m_ParameterList.count();
    if(m_ParameterList.count() == 5) {
        qDebug() << "Input file: " << m_ParameterList.at(0).toString();
        qDebug() << "Output file: " << m_ParameterList.at(1).toString();
        qDebug() << "Gaussian: " << m_ParameterList.at(2).toFloat();
        qDebug() << "Load: " << m_ParameterList.at(3).toInt();
        qDebug() << "N of Iteration: " << m_ParameterList.at(4).toInt();
    }
}
