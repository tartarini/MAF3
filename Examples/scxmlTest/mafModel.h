#ifndef MAFMODEL_H
#define MAFMODEL_H

// Includes list
#include "mafObjectBase.h"

/**
 Class name: mafModel
 This class provides a model class to store information for the workflow logic.
 @sa mafStateMachine
 */
class mafModel : public mafCore::mafObjectBase {
    Q_OBJECT
    Q_PROPERTY(QString ip READ serviceIP WRITE setServiceIP)
    Q_PROPERTY(QString port READ servicePort WRITE setServicePort)
    Q_PROPERTY(QString protocol READ serviceProtocol WRITE setServiceProtocol)
    Q_PROPERTY(QVariantList parameters READ parametersList WRITE setParametersList)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafModel(const mafString code_location = "");

    /// Set the IP associated with the service to call.
    void setServiceIP(mafString ip);

    /// Set the Port associated with the service to call.
    void setServicePort(mafString port);

    /// Set the Protocol associated with the service to call.
    void setServiceProtocol(mafString protocol);

    /// Return the IP associated with the service.
    mafString serviceIP();

    /// Return the Port associated with the service.
    mafString servicePort();

    /// Return the Protocol associated with the service.
    mafString serviceProtocol();

    /// Return the list of parameters.
    mafVariantList parametersList();

    /// Allow to assign the list of parameters.
    void setParametersList(mafVariantList params);

private:
    mafString m_Ip; ///< IP associated to the service
    mafString m_Port; ///< Port associated to the service
    mafString m_Protocol; ///< Protocol associated to the service
    mafVariantList m_ParameterList; ///< List of parameters to forward to the called service.
};

inline mafString mafModel::serviceIP() {
    return m_Ip;
}

inline mafString mafModel::servicePort() {
    return m_Port;
}

inline mafString mafModel::serviceProtocol() {
    return m_Protocol;
}

inline mafVariantList mafModel::parametersList() {
    return m_ParameterList;
}

#endif // MAFMODEL_H
