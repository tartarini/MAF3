/*
 *  mafOrthoSlice.h
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 09/12/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFORTHOSLICE_H
#define MAFORTHOSLICE_H

#include <QWidget>

namespace Ui {
    class mafOrthoSlice;
}

/**
Class name: mafOrthoSlice
This class represent the UI for the mafViewOrthoSlice compound view.
It has 3 slider that allow the user to move the cutting planes along the main axes of the volume.

@sa mafViewOrthoSlice
*/
class mafOrthoSlice : public QWidget {
    Q_OBJECT

private Q_SLOTS:
    /// Get the slider position and forward it to the algorithmic class.
    void on_xPosition_sliderMoved(int val);
    /// Get the slider position and forward it to the algorithmic class.
    void on_yPosition_sliderMoved(int val);
    /// Get the slider position and forward it to the algorithmic class.
    void on_zPosition_sliderMoved(int val);

Q_SIGNALS:
    /// Signal emitting the position of all the sliders
    void positionUpdated(double *pos);

public:
    /// Object constructor
    explicit mafOrthoSlice(QWidget *parent = 0);
    /// Object destructor.
    ~mafOrthoSlice();

    /// Allows to set the volume's bounds
    void setBounds(double bounds[6]);

    /// Allow to retrieve the position according to the values of the 3 sliders.
    const double *position() const;
    
    /// setter for position
    void setPosition(double pos[3]);

    /// Set the number of decimal digits to consider for precision
    void setDecimalDigits(int decimal = 2);

private:
    Ui::mafOrthoSlice *ui; ///< Pointer to the user interface widget
    double m_Bounds[6]; ///< Array containing the volume's bounds
    int m_Decimals; ///< Number of decimal digit to consider (default is 2)

    double m_Position[3]; ///< Variable containing the position of the 3 axes coordinates.

    /// Calculate the real value of the position in double precision according to number of decimals.
    double calculateRealValue(int val);

    /// Calculate the value to be given to the slider according to number of decimals.
    int calculateSliderValue(double val);
};

inline const double *mafOrthoSlice::position() const {
    const double *p = &m_Position[0];
    return p;
}

#endif // MAFORTHOSLICE_H
