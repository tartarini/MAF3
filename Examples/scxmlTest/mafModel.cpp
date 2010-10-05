#include "mafModel.h"

using namespace mafCore;

mafModel::mafModel(const mafString code_location) : mafObjectBase(code_location) {
}

void mafModel::setServiceIP(mafString ip) {
//    mafMsgDebug() << "ip: " << ip;
    m_Ip = ip;
}

void mafModel::setServicePort(mafString port) {
//    mafMsgDebug() << "port: " << port;
    m_Port = port;
}

void mafModel::setServiceProtocol(mafString protocol) {
//    mafMsgDebug() << "protocol: " << protocol;
    m_Protocol = protocol;
}

void mafModel::setParametersList(mafVariantList params) {
    m_ParameterList.clear();
    m_ParameterList.append(params);
    mafMsgDebug() << "Num of Params: " << m_ParameterList.count();
    if(m_ParameterList.count() == 5) {
        mafMsgDebug() << "Input file: " << m_ParameterList.at(0).toString();
        mafMsgDebug() << "Output file: " << m_ParameterList.at(1).toString();
        mafMsgDebug() << "Gaussian: " << m_ParameterList.at(2).toFloat();
        mafMsgDebug() << "Load: " << m_ParameterList.at(3).toInt();
        mafMsgDebug() << "N of Iteration: " << m_ParameterList.at(4).toInt();
    }
}
