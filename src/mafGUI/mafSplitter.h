/*
 *  mafSplitter.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 25/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */


#ifndef MAFSPLITTER_H
#define MAFSPLITTER_H

#include "mafGUIDefinitions.h"
#include <QSplitter>
#include <QWidget>
#include <mafObjectBase.h>

/**
 Class Name: mafSplitter
 Class used to create split panels like OrthoSlice view.
*/
class MAFGUISHARED_EXPORT mafSplitter : public QSplitter {
	Q_OBJECT
    
public:
    ///Object constructor
    mafSplitter(Qt::Orientation orientation = Qt::Horizontal, QWidget * parent = 0);

    /// Object destructor
    ~mafSplitter();

    /// Allows to adjust position of the split panel
    void adjustPosition(int p);

    /// Allows to assign the mafViews associated with the two panel of the splitter
    void setViews(QList<mafCore::mafObjectBase*> &vlist);

    /// Add a mafView into the views list.
    void addView(mafCore::mafObjectBase *view);

Q_SIGNALS:
    /// Signal used to ask the splitter synchronization.
    void synchronizeSplitterSignal(QObject *splitter);

protected:

    /// Redefine the paint event to allow sending the synchronize event.
    void paintEvent ( QPaintEvent * pe );

    QList<mafCore::mafObjectBase*> m_Views; ///< List of mafView to use into the mafSplitter.
};

#endif // MAFSPLITTER_H
