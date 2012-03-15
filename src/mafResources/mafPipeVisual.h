/*
 *  mafPipeVisual.h
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISUALPIPE_H
#define MAFVISUALPIPE_H

// Includes list
#include "mafPipe.h"
#include <mafProxyInterface.h>

namespace mafResources {

/**
 Class name: mafPipeVisual
 This is the base class for the MAF3 visual pipes. The class defines the topic used to pick VMEs:
 - maf.local.resources.interaction.vmePick
 */
class MAFRESOURCESSHARED_EXPORT mafPipeVisual : public mafPipe {
    Q_OBJECT
    Q_PROPERTY(QObject *graphicObject READ graphicObject WRITE setGraphicObject)
    Q_PROPERTY(int status READ status WRITE setStatus)
    Q_PROPERTY(QString opacityValue READ opacityValue)
    Q_PROPERTY(int opacityValueSlider_sliderPosition READ sliderPosition)
    
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipe);

public:
     /// Object constructor.
    mafPipeVisual(const QString code_location = "");

    /// Get output of the pipe.
    mafCore::mafProxyInterface *output();

    /// Return the visibility status
    bool visibility() const;
    
    /// Pass to the visual pipe an object (or a group of objects) via mafProxyInterface
    /** graphicObject parameter  represents the graphic components for that specific pipeline, for example, VTK use vtkRenderer */
    void setGraphicObject(QObject *graphicObject);

    /// return the graphic object used for paint the scene.
    QObject *graphicObject() const;

    /// Set the status (LOCAL or GLOBAL) to modify its behavour
    void setStatus(int status);

    /// return the status of the pipe visual (LOCAL or GLOBAL);
    int status() const;
        
protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisual();

    /// update visibility for actor or volume passed as parameter
    virtual void updateVisibility();

    mafCore::mafProxyInterface *m_Output; ///< Output for visual pipe.
    QObject *m_GraphicObject; ///< represents the graphic object for render the scene.
    double m_OpacityValue; ///< Opacity value.
    
private:
    /// Register signals and slots connections with the event bus.
    void initializeConnections();

    bool m_Visibility; ///< Contains the visibility status of the owned object/s
    int m_Status; ///< Contains the status (LOCAL or GLOBAL)

    
Q_SIGNALS:
    /// signal emitted when the pick already happened. The information will be forwarded to the Interaction Manager.
    void vmePickedSignal(double *pickPos, unsigned long modifiers, mafVME *vme, QEvent *e);
    
public Q_SLOTS:
    /// Set the visibility of its rendering scene.
    virtual void setVisibility(bool visible);
    
    /// Called when a new graphic object has been set.
    /** This method has to be re-defined to update stuff linked to the graphic object.*/
    virtual void updatedGraphicObject();

    /// Set opacity value from slider.
    void on_opacityValueSlider_sliderMoved(int value);

    /// Get opacity value;
    QString opacityValue();

    /// Set Slider postion;
    int sliderPosition();

private Q_SLOTS:
    /// Forward the vmePick event if the pick hits the current visualized VME.
    bool vmePick(double *pickPos, unsigned long, mafCore::mafProxyInterface *actor, QEvent *e);
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafProxyInterface *mafPipeVisual::output() {
    return m_Output;
}

inline bool mafPipeVisual::visibility() const {
    return m_Visibility;
}
    
inline QObject *mafPipeVisual::graphicObject() const {
    return m_GraphicObject;
}

inline void mafPipeVisual::setStatus(int status) {
    m_Status = status;
}

inline int mafPipeVisual::status() const {
    return m_Status;
}

inline int mafPipeVisual::sliderPosition() {
    return (m_OpacityValue*100);
}   

} //namespace mafResources

#endif // MAFVISUALPIPE_H
