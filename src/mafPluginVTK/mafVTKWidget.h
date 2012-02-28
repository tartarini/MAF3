/*
 *  mafVTKWidget.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 12/10/10.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKWIDGET_H
#define MAFVTKWIDGET_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <QVTKWidget.h>


class vtkRenderer;

namespace mafPluginVTK {

class mafAxes;


/**
Class name: mafVTKWidget
This class overloads VKT mouse events and forward them to EventBus.
*/
class MAFPLUGINVTKSHARED_EXPORT mafVTKWidget : public QVTKWidget {
    Q_OBJECT
    Q_PROPERTY(QVariant viewObject READ viewObject WRITE setViewObject)
    Q_PROPERTY(bool parallelCameraMode READ parallelCameraMode WRITE setParallelCameraMode)

    /// typedef macro.
    mafSuperclassMacro(QVTKWidget);

public:
    /// Object constructor.
    mafVTKWidget(QWidget* parent = NULL, Qt::WFlags f = 0);

    /// Overloaded mouse press handler
    /*virtual*/ void mousePressEvent(QMouseEvent* event);

    /// Overloaded mouse release handler
    /*virtual*/ void mouseReleaseEvent(QMouseEvent* event);

    /// Overloaded mouse wheel handler
    /*virtual*/ void wheelEvent(QWheelEvent* event);

    /// Overloaded mouse move handler
    /*virtual*/ void mouseMoveEvent(QMouseEvent* event);

    /// Return the parallel camera flag.
    bool parallelCameraMode() const;

    /// Allows to set the parallel camera mode on all layers.
    void setParallelCameraMode(bool on = true);
    
    /// set camera position and orientation giving three fundamental parameters.
    void setCameraParameters(double position[3], double focalPoint[3], double viewUp[3]);

    /// Allows to show the axes representing the global reference system. This method has to be called after that the renderer has been added to the renderwindow.
    void showAxes(bool show = true);

    /// Allows to assign the mafView associated to the widget.
    void setViewObject(const QVariant view);

    /// Allows to retrieve the mafView associated to th widget.
    QVariant viewObject() const;

    /// Allows to retrieve the bounds of visible objects for the given rendering layer.
    double *visibleBounds(QString layerName = "base");

    /// Allows to retrieve the bounds for visible objects in all the created layers.
    double *visibleBoundsOnAllLayers();

    /// Allow to remove all objects from all layers.
    void removeAllObjects();

    //////////////////////////////////////////////// Layers API

    /// Allows to create a new layer. The new layer is put on top of all the other existing layers. Return the renderer associated with the created layer name.
    vtkRenderer *createLayer(const QString layerName);
    
    /// Allows to delete the given layer. Return true on success, false otherwise.
    /** All the objects shown in that layer will be hidden and removed from the renderer.
    Only the layer created by the user can be deleted; "base" and "tool" layers can not be removed.*/
    bool deleteLayer(const QString layerName);

    /// Allows to move the layer at the specific layer index (0 .. n), where '0' is the background, 'n' is the front one
    void moveLayerTo(const QString layerName, unsigned int layerLevel);
    
    /// Allows to hide/show all the objects associated with the given layer.
    void showLayer(const QString layerName, bool show = true);

    /// Return the renderer associated with the given layer name.
    vtkRenderer *renderer(const QString layerName = "base");

    /// Return the list of all available layers.
    QList<QString> layersList() const;

    ////////////////////////////////////////////////

Q_SIGNALS:
    /// picked button pressed.
    void vmePickSignal(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent * e);
    
    /// signal for mouse press event
    void mousePressSignal(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);
    /// signal for mouse release event
    void mouseReleaseSignal(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);
    /// signal for mouse move event
    void mouseMoveSignal(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);

protected:
    /// Object destructor.
    /* virtual */ ~mafVTKWidget();
    
    // overloaded resize handler
    /*virtual*/ void resizeEvent(QResizeEvent* event);

private:
    /// Get key modifiers.
    void getModifiers(vtkRenderWindowInteractor* iren);

    /// handle the event mouse press
    void mousePress(vtkRenderWindowInteractor* iren, QEvent *e);
    /// handle the event mouse release
    void mouseRelease(vtkRenderWindowInteractor* iren, QEvent *e);
    /// handle the event mouse move
    void mouseMove(vtkRenderWindowInteractor* iren, QEvent *e);
    
    /// check if a VTK actor is picked
    void pickProp(vtkRenderWindowInteractor* iren, QEvent *e, mafCore::mafProxyInterface *proxy, double *posPicked);

    /// initialize connections
    void initializeConnections();

    /// initialize layers
    void initializeLayers();
    
    unsigned long m_Modifiers;  ///< Optional modifiers for the button.

    mafAxes *m_Axes; ///< Tool representing a global reference system.
    bool m_InteractionStarted; ///< Monitor the mouse event so to send them to the mafInteractionManager only if the mousepress has been appened.
    
    QHash<QString, vtkRenderer *> m_LayerHash; ///< Hash table that store the renderers associated with specific layer.
    vtkRenderer *m_RendererTool; ///< Renderer associated to the tool layer.
    vtkRenderer *m_RendererBase; ///< Renderer associated to the base layer.
    QObject *m_View; ///< Link to the corresponding mafView
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QList<QString> mafVTKWidget::layersList() const {
    return m_LayerHash.keys();
}

inline void mafVTKWidget::setViewObject(const QVariant view) {
    m_View = view.value<QObject *>();
}

inline QVariant mafVTKWidget::viewObject() const {
    QVariant v;
    v.setValue<QObject *>(m_View);
    return v;
}

} // namespace mafPluginVTK

#endif // MAFVTKWIDGET_H
