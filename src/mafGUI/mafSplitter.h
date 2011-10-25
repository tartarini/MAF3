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

/**
 Class Name: mafSplitter
 Class used to create splitted panels like OrthoSlice view.
*/
class MAFGUISHARED_EXPORT mafSplitter : public QSplitter {
	Q_OBJECT
    
public:
    mafSplitter(QWidget * parent = 0);

    void adjustPosition(int p);

Q_SIGNALS:
    /// Signal used to ask the splitter synchronization.
    void synchronizeSplitterSignal(QObject *splitter);

protected:
    void paintEvent ( QPaintEvent * pe );
};

#endif // MAFSPLITTER_H
