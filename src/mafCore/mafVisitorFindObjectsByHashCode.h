/*
 *  mafVisitorFindObjectsByHashCode.h
 *  mafCore
 *
 *  Created by Daniele Giunchi on 16/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISITORFINDOBJECTSBYHASHCODE_H
#define MAFVISITORFINDOBJECTSBYHASHCODE_H


// Includes list
#include "mafVisitorFindObjects.h"

namespace mafCore {

/**
 Class name: mafVisitorFindObjectsByHashCode
 This class define a visitor object that is able to create a list
 of objects that have the input hash code
 @sa mafVisitor mafVisitorFindObjects
 */
class MAFCORESHARED_EXPORT mafVisitorFindObjectsByHashCode : public mafVisitorFindObjects {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafVisitorFindObjects);

public:
    /// object constructor
    mafVisitorFindObjectsByHashCode(const QString code_location = "");

    /// overloaded object constructor
    mafVisitorFindObjectsByHashCode(const QString *hash_code, const QString code_location = "");

    /// Execute the defined operation on visited object passes as argument.
    /*virtual*/ void visit(mafObjectBase *object);

protected:
    /// Object destructor.
    /* virtual */ ~mafVisitorFindObjectsByHashCode();
private:
    QString m_HashCode;
};

} //mafCore

#endif // MAFVISITORFINDOBJECTSBYHASHCODE_H
