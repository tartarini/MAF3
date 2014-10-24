/*
 *  mafSmartPointer.txx
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

template<class T>
mafSmartPointer<T>::mafSmartPointer(T *p) : QPointer<T>(p) {
    m_ObjectBase = p;
};

template<class T>
mafSmartPointer<T>::~mafSmartPointer() {
	this->clear();
	m_ObjectBase->release();
}
