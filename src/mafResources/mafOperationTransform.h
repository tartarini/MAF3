/*
 *  mafOperationTransform.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 17/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOPERATIONTRANSFORM_H
#define MAFOPERATIONTRANSFORM_H

#include "mafOperation.h"

namespace mafResources {

class mafMatrix;
class mafDataSetCollection;

/**
  Class Name: mafOperationTransform
  This operation allow to transform the pose matrix of the input selected VME.
  */
class MAFRESOURCESSHARED_EXPORT mafOperationTransform : public mafOperation {
    Q_OBJECT
    Q_PROPERTY(QString xPos READ xPos WRITE setXPos)
    Q_PROPERTY(QString yPos READ yPos WRITE setYPos)
    Q_PROPERTY(QString zPos READ zPos WRITE setZPos)
    Q_PROPERTY(QString xRot READ xRot WRITE setXRot)
    Q_PROPERTY(QString yRot READ yRot WRITE setYRot)
    Q_PROPERTY(QString zRot READ zRot WRITE setZRot)

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);

public slots:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();

    /// Allows to call the piece of algorithm that is needed to restore the previous state of the operation's execution.
    /*virtual*/ void unDo();

    /// Allows to call the piece of algorithm that is needed to apply the operation again.
    /*virtual*/ void reDo();
    
    /// Set operation parameters.
    /*virtual*/ void setParameters(QVariantList parameters);

public:
    /// Object constructor.
    mafOperationTransform(const QString code_location = "");
    
    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
    
    /// Initialize the operation by copying the input pose matrix.
    /*virtual*/ bool initialize();

    /// Return the X position of the input VME as a string.
    QString xPos() const;

    /// Return the Y position of the input VME as a string.
    QString yPos() const;

    /// Return the Z position of the input VME as a string.
    QString zPos() const;

    /// Assign the X position at the pose matrix of the input VME.
    void setXPos(const QString x);

    /// Assign the Y position at the pose matrix of the input VME.
    void setYPos(const QString y);

    /// Assign the Z position at the pose matrix of the input VME.
    void setZPos(const QString z);

    /// Return the rotation around the X axes as a string.
    QString xRot() const;

    /// Return the rotation around the Y axes as a string.
    QString yRot() const;

    /// Return the rotation around the Z axes as a string.
    QString zRot() const;

    /// Assign a rotation around the X axes
    void setXRot(const QString xrot);

    /// Assign a rotation around the Y axes
    void setYRot(const QString yrot);

    /// Assign a rotation around the Z axes
    void setZRot(const QString zrot);

protected:
    /// Terminate the operation's execution.
    /*virtual*/ void terminated();
    
    /// Object destructor.
    /* virtual */ ~mafOperationTransform();

private:
    mafDataSetCollection *m_DataSetCollection; ///< Dataset collection of the input VME.
    mafMatrix *m_Matrix; ///< New matrix pose.
    mafMatrix *m_OldMatrix; ///< Backup pose matrix
};

} // namespace mafResources

#endif // MAFOPERATIONTRANSFORM_H
