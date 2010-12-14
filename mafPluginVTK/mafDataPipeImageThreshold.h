/*
 *  mafDataPipeImageThreshold.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 16/04/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATAPIPEIMAGETHRESHOLD_H
#define MAFDATAPIPEIMAGETHRESHOLD_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafDataPipe.h>
#include <mafContainer.h>
#include <vtkSmartPointer.h>

// Foundation Class forwarding list
class vtkImageThreshold;
class vtkImageData;

namespace mafPluginVTK {

// Class forwarding list

/**
 Class name: mafDataPipeImageThreshold
 This class allows you to make a threshoding on input image data.
 */
class MAFPLUGINVTKSHARED_EXPORT mafDataPipeImageThreshold : public mafResources::mafDataPipe {
    Q_OBJECT
    Q_PROPERTY(double lowerThrehsold READ lowerThrehsold WRITE setLowerThrehsold)
    Q_PROPERTY(double upperThrehsold READ upperThrehsold WRITE setUpperThrehsold)
    Q_PROPERTY(unsigned int thresholdMode READ thresholdMode WRITE setThresholdMode)
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafDataPipe);

public:
    /// Object constructor.
    mafDataPipeImageThreshold(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafDataPipeImageThreshold();

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Initialize and create the pipeline
    /*virtual*/ void createPipe();

    /// Allow to set the lower threshold value for thresholding the input image data.
    void setLowerThrehsold(double threshold);

    /// Allow to get the lower threshold value used for thresholding the input image data.
    double lowerThrehsold();

    /// Allow to set the upper threshold value for thresholding the input image data.
    void setUpperThrehsold(double threshold);

    /// Allow to get the upper threshold value used for thresholding the input image data.
    double upperThrehsold();

    ///< Enumeration that define the thresholding mode.
    enum {
        LOWER = 0,
        UPPER,
        BETWEEN
    };

    /// Allow to choose the thresholding type LOWER, UPPER or BETWEEN
    void setThresholdMode(unsigned int mode);

    /// Return the thresholding modality.
    unsigned int thresholdMode();

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

private:
    double m_LowerThreshold; ///< Variable that contains the lower threshold value for the Thresholding operation.
    double m_UpperThreshold; ///< Variable that contains the upper threshold value for the Thresholding operation.
    unsigned int m_ThresholdMode; ///< Threshold mode: LOWER, UPPER, BETWEEN.

    vtkSmartPointer<vtkImageThreshold> m_ThresholdFilter; ///< VTK threshold filter used to thresholding input image data.
    mafCore::mafContainer<vtkImageData> m_OutputValue; ///< Output value coming from the elaboration data pipe.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline double mafDataPipeImageThreshold::lowerThrehsold() {
    return m_LowerThreshold;
}

inline double mafDataPipeImageThreshold::upperThrehsold() {
    return m_UpperThreshold;
}

inline void mafDataPipeImageThreshold::setThresholdMode(unsigned int mode) {
    if(m_ThresholdMode != mode) {
        m_ThresholdMode = mode;
    }
}

inline unsigned int mafDataPipeImageThreshold::thresholdMode() {
    return m_ThresholdMode;
}

} // namespace mafPluginVTK

#endif // MAFDATAPIPEIMAGETHRESHOLD_H
