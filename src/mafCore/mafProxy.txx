/*
 *  mafProxy.txx
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

template<typename T>
mafProxy<T>::operator T *() const {
    return (T *)m_ExternalData;
}

template<typename T>
void mafProxy<T>::operator=(T*val) {
    m_ExternalData = val; updateExternalDataType();
}

template<typename T>
T *mafProxy<T>::operator->() {
    return m_ExternalData;
}

template<typename T>
void mafProxy<T>::setDestructionFunction(mafExternalDataDestructorPointer destruc_function) {
    m_ExternalDestructor = destruc_function;
}

template<typename T>
mafProxy<T>::mafProxy() : mafProxyInterface(), m_ExternalData(NULL), m_ExternalDestructor(NULL) {
}

template<typename T>
mafProxy<T>::~mafProxy() {
    if(m_ExternalDestructor != NULL) {
        (m_ExternalData->*m_ExternalDestructor)();
    }
}

template<typename T>
T *mafProxy<T>::externalData() {
    return m_ExternalData;
}

template<typename T>
bool mafProxy<T>::isEqual(mafProxyInterface *container) {
	mafProxy<T> *data = static_cast<mafProxy<T> *>(container);
	return m_ExternalData == data->externalData();
}

template<typename T>
inline void mafProxy<T>::updateExternalDataType() {
    QString data_type(typeid( T ).name());
    
    // Needed because typeid return the length of the string containing the type
    // before the string itself.
#ifdef _MSC_VER
    data_type = data_type.mid(6);
#else
    int dt_len = data_type.length();
    data_type = (dt_len > 10) ? data_type.mid(2) : data_type.mid(1);
#endif    
    setExternalDataType(data_type);
}

