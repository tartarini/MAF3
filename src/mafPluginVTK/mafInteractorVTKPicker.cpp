/*
 *  mafInteractorVTKPicker.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 13/07/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInteractorVTKPicker.h"
#include "mafVTKWidget.h"
#include <QMouseEvent>
#include <vtkSmartPointer.h>

#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafInteractorVTKPicker::mafInteractorVTKPicker(const QString code_location) : mafResources::mafInteractor(code_location), m_CellPicker(NULL) {
}


mafInteractorVTKPicker::~mafInteractorVTKPicker() {
    if (m_CellPicker) {
        m_CellPicker->Delete();
    }
}

void mafInteractorVTKPicker::mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {

    mafVTKWidget *w = qobject_cast<mafVTKWidget *>(m_GraphicObject);
    if(w == NULL) {
        return;
    }

    QMouseEvent *me = (QMouseEvent *)e;
    int mousePosX = me->x();
    //Qt widget origin is the upper-left corner, while VTK uses the lower-left. So I have to convert the Y point.
    int widgetSize = w->size().height();
    int mousePosY = widgetSize - me->y();

    double posPicked[3];
    mafCore::mafProxy<vtkProp> actorPicked;
    vtkProp *actor = NULL;

    if(me->button() == Qt::LeftButton) {
        m_CellPicker = vtkCellPicker::New();

        vtkRendererCollection *rc = w->GetRenderWindow()->GetRenderers();
        vtkRenderer *r = NULL;
        rc->InitTraversal();
        while(r = rc->GetNextItem()) {
            if(m_CellPicker->Pick(mousePosX, mousePosY, 0, r)) {
                m_CellPicker->GetPickPosition(posPicked);
                Q_EMIT vmePickedVTKSignal(posPicked, modifiers, obj, e);
                return;
            }
        }
    }
}
