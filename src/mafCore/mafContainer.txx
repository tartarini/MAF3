/*
 *  mafContainer.txx
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

template<typename T>
mafContainer<T>::operator T *() const {
    return (T *)m_ExternalData;
}

template<typename T>
void mafContainer<T>::operator=(T*val) {
    m_ExternalData = val; updateExternalDataType();
}

template<typename T>
T *mafContainer<T>::operator->() {
    return m_ExternalData;
}

template<typename T>
void mafContainer<T>::setDestructionFunction(mafExternalDataDestructorPointer destruc_function) {
    m_ExternalDestructor = destruc_function;
}

template<typename T>
mafContainer<T>::mafContainer() : mafContainerInterface(), m_ExternalData(NULL), m_ExternalDestructor(NULL) {
}

template<typename T>
mafContainer<T>::~mafContainer() {
    if(m_ExternalDestructor != NULL) {
        (m_ExternalData->*m_ExternalDestructor)();
    }
    setExternalDataType("");
}

template<typename T>
T *mafContainer<T>::externalData() {
    return m_ExternalData;
}

template<typename T>
bool mafContainer<T>::isEqual(mafContainerInterface *container) {
bool result = this->externalDataType().compare(container->externalDataType()) == 0;
if (result) {
    mafContainer<T> *data = static_cast<mafContainer<T> *>(container);
    result = m_ExternalData == data->externalData();
}
return result;
}

template<typename T>
inline void mafContainer<T>::updateExternalDataType() {
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

