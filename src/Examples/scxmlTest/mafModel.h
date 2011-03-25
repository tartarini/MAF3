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
    mafModel(const QString code_location = "");

    /// Set the IP associated with the service to call.
    void setServiceIP(QString ip);

    /// Set the Port associated with the service to call.
    void setServicePort(QString port);

    /// Set the Protocol associated with the service to call.
    void setServiceProtocol(QString protocol);

    /// Return the IP associated with the service.
    QString serviceIP();

    /// Return the Port associated with the service.
    QString servicePort();

    /// Return the Protocol associated with the service.
    QString serviceProtocol();

    /// Return the list of parameters.
    QVariantList parametersList();

    /// Allow to assign the list of parameters.
    void setParametersList(QVariantList params);

private:
    QString m_Ip; ///< IP associated to the service
    QString m_Port; ///< Port associated to the service
    QString m_Protocol; ///< Protocol associated to the service
    QVariantList m_ParameterList; ///< List of parameters to forward to the called service.
};

inline QString mafModel::serviceIP() {
    return m_Ip;
}

inline QString mafModel::servicePort() {
    return m_Port;
}

inline QString mafModel::serviceProtocol() {
    return m_Protocol;
}

inline QVariantList mafModel::parametersList() {
    return m_ParameterList;
}

#endif // MAFMODEL_H
