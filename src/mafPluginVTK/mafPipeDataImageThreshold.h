/*
 *  mafPipeDataImageThreshold.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 16/04/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATAPIPEIMAGETHRESHOLD_H
#define MAFDATAPIPEIMAGETHRESHOLD_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafPipeData.h>
#include <mafProxy.h>
#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>

// Foundation Class forwarding list
class vtkImageThreshold;

namespace mafPluginVTK {

// Class forwarding list

/**
 Class name: mafPipeDataImageThreshold
 This class allows you to make a thresholding on input image data.
 */
class MAFPLUGINVTKSHARED_EXPORT mafPipeDataImageThreshold : public mafResources::mafPipeData {
    Q_OBJECT
    Q_PROPERTY(double lowerThrehsold READ lowerThrehsold WRITE setLowerThrehsold)
    Q_PROPERTY(double upperThrehsold READ upperThrehsold WRITE setUpperThrehsold)
    Q_PROPERTY(unsigned int thresholdMode READ thresholdMode WRITE setThresholdMode)
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeData);

public:
    /// Object constructor.
    mafPipeDataImageThreshold(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Allow to set the lower threshold value for thresholding the input image data.
    void setLowerThrehsold(double threshold);

    /// Allow to get the lower threshold value used for thresholding the input image data.
    double lowerThrehsold();

    /// Allow to set the upper threshold value for thresholding the input image data.
    void setUpperThrehsold(double threshold);

    /// Allow to get the upper threshold value used for thresholding the input image data.
    double upperThrehsold();

    ///< Enumeration that define the thresholding mode.
    typedef enum {
        mafThresholdModeLower = 0,
        mafThresholdModeUpper,
        mafThresholdModeBetween
    } mafThresholdMode;

    /// Allow to choose the thresholding type LOWER, UPPER or BETWEEN
    void setThresholdMode(unsigned int mode);

    /// Return the thresholding modality.
    unsigned int thresholdMode();

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeDataImageThreshold();

private:
    double m_LowerThreshold; ///< Variable that contains the lower threshold value for the Thresholding operation.
    double m_UpperThreshold; ///< Variable that contains the upper threshold value for the Thresholding operation.
    unsigned int m_ThresholdMode; ///< Threshold mode: LOWER, UPPER, BETWEEN.

    vtkSmartPointer<vtkImageThreshold> m_ThresholdFilter; ///< VTK threshold filter used to thresholding input image data.
    mafCore::mafProxy<vtkAlgorithmOutput> m_OutputValue; ///< Output value coming from the elaboration data pipe.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline double mafPipeDataImageThreshold::lowerThrehsold() {
    return m_LowerThreshold;
}

inline double mafPipeDataImageThreshold::upperThrehsold() {
    return m_UpperThreshold;
}

inline void mafPipeDataImageThreshold::setThresholdMode(unsigned int mode) {
    if(m_ThresholdMode != mode) {
        m_ThresholdMode = mode;
    }
}

inline unsigned int mafPipeDataImageThreshold::thresholdMode() {
    return m_ThresholdMode;
}

} // namespace mafPluginVTK

#endif // MAFDATAPIPEIMAGETHRESHOLD_H
