/*=========================================================================
 
 Program:   Multimod Fundation Library
 Module:    $RCSfile: vtkMAFVolumeSlicer.h,v $
 Language:  C++
 Date:      $Date: 2008-07-03 11:27:45 $
 Version:   $Revision: 1.1 $
 Authors:   Alexander Savenko, Paolo Quadrani
 Project:   MultiMod Project (www.ior.it/multimod)
 
 ==========================================================================
 Copyright (c) 2002 
 CINECA - Interuniversity Consortium (www.cineca.it)
 v. Magnanelli 6/3
 40033 Casalecchio di Reno (BO)
 Italy
 ph. +39-051-6171411 (90 lines) - Fax +39-051-6132198
 
 Use, modification and redistribution of the software, in source or
 binary forms, are permitted provided that the following terms and
 conditions are met:
 
 1) Redistribution of the source code, in verbatim or modified
 form, must retain the above copyright notice, this license,
 the following disclaimer, and any notices that refer to this
 license and/or the following disclaimer.  
 
 2) Redistribution in binary form must include the above copyright
 notice, a copy of this license and the following disclaimer
 in the documentation or with other materials provided with the
 distribution.
 
 3) Modified copies of the source code must be clearly marked as such,
 and must not be misrepresented as verbatim copies of the source code.
 
 THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE SOFTWARE "AS IS"
 WITHOUT EXPRESSED OR IMPLIED WARRANTY INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE.  IN NO EVENT SHALL ANY COPYRIGHT HOLDER OR OTHER PARTY WHO MAY
 MODIFY AND/OR REDISTRIBUTE THE SOFTWARE UNDER THE TERMS OF THIS LICENSE
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, LOSS OF DATA OR DATA BECOMING INACCURATE
 OR LOSS OF PROFIT OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF
 THE USE OR INABILITY TO USE THE SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGES.
 
 =========================================================================*/
// .NAME vtkMAFVolumeSlicer - a filter for creating slices from structured volume data (rectilinear grid or structured points)
//
// .SECTION Description
// Inputs of the filter are:
//   volume (vtkRectilinearGrid or vtkImageData)
//   plane definition: origin and two vectors that define the axes of the image rectangle
//   color mapping: window and level of color mapping
//   
// Outputs of the filter are:
//   image (vtkImageData): The resolution and spacing of the image should be specified by consumer. 
//                         The origin will be automatically set to zero.
//   polyline slice (vtkPolyData): This output includes both the polyline that define the cut and corresponded triangles. The texture coordinates are provided as well.
// .SECTION See Also
// vtkProbeFilter, vtkPlaneSource


#ifndef __vtkMAFVolumeSlicer_h
#define __vtkMAFVolumeSlicer_h

#include "vtkMAF_global.h"
#include <vtkAlgorithm.h>

/// forward declarations
class vtkImageData;
class vtkLinearTransform;

class VTK_vtkMAF_EXPORT vtkMAFVolumeSlicer : public vtkAlgorithm {
public:
    vtkTypeMacro(vtkMAFVolumeSlicer, vtkAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    // Description:
    // Construct with user-specified implicit function; InsideOut turned off;
    // value set to 0.0; and generate clip scalars turned off.
    static vtkMAFVolumeSlicer *New();

    // Description:
    // Return the textured output.
    vtkImageData *GetTexturedOutput();

    // Description:
    // Return the output port (a vtkAlgorithmOutput) of the textured output.
    vtkAlgorithmOutput* GetTexturedOutputPort() 
    {
      return this->GetOutputPort(1);
    }

    // Description:
    // Return the mtime also considering the implicit plane.
    unsigned long GetMTime();

    /**
     Specify the normal of the plane.*/
    void SetPlaneNormal(double normal[3]);
    void SetPlaneNormal(double nx, double ny, double nz);
    vtkGetVectorMacro(PlaneNormal, double, 3);

    /**
     Specify a point defining the origin of the plane.*/
    void SetPlaneOrigin(double origin[3]);
    void SetPlaneOrigin(double x, double y, double z);
    vtkGetVectorMacro(PlaneOrigin, double, 3);
    
    /** Specify x-axis of the plane*/
    void SetPlaneAxisX(double axis[3]);
    void SetPlaneAxisX(const double *axis);
    vtkGetVectorMacro(PlaneAxisX, double, 3);
    
    /** Specify y-axis of the plane*/
    void SetPlaneAxisY(double axis[3]);
    void SetPlaneAxisY(const double *axis);
    vtkGetVectorMacro(PlaneAxisY, double, 3);
    
    /**
     Set / Get the Window for color modulation. The formula for modulation is 
     (S - (L - W/2))/ W where S is the scalar value, L is the level and W is the window.*/
    vtkSetMacro( Window, double );
    vtkGetMacro( Window, double );
    
    /**
     Set / Get the Level to use -> modulation will be performed on the 
     color based on (S - (L - W/2))/W where S is the scalar value, L is
     the level and W is the window.*/
    vtkSetMacro( Level, double );
    vtkGetMacro( Level, double );
    
    /**
     Set/get auto-spacing feature. In this mode the image spacing is selected automatically to fit the whole slice*/
    vtkSetMacro( AutoSpacing, int );
    vtkGetMacro( AutoSpacing, int );

    /** 
     Transform slicer plane according to the given transformation before slicing.*/
    void SetSliceTransform(vtkLinearTransform *trans);
    
    vtkDataObject *GetInput();

    /*virtual*/ int ProcessRequest(vtkInformation* request,
                                   vtkInformationVector** inputVector,
                                   vtkInformationVector* outputVector);
    
protected:
    vtkMAFVolumeSlicer();
    ~vtkMAFVolumeSlicer();

    int RequestUpdateExtent(
                        vtkInformation* request,
                        vtkInformationVector** inputVector,
                            vtkInformationVector* outputVector);
    
    /*virtual*/ int RequestInformation(vtkInformation* request,
        vtkInformationVector** inputVector,
        vtkInformationVector* outputVector);

    /*virtual*/ int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
    
    /*virtual*/ int FillInputPortInformation( int /*port*/, vtkInformation* info);
    
    /*virtual*/ int FillOutputPortInformation( int port, vtkInformation* info);
    
    /*virtual int ProcessRequest( vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);*/
    
    void ComputeInputUpdateExtents(vtkDataObject *output);
    
    void PrepareVolume();
    void CalculateTextureCoordinates(const double point[3], const int size[2], const double spacing[2], double ts[2]);
    
    template<typename InputDataType, typename OutputDataType> void CreateImage(const InputDataType *input, OutputDataType *output, vtkImageData *outputObject);

    void GeneratePolygonalOutput();
    void GenerateTextureOutput();
  
    int   NumComponents;
    // plane coordinates
    double PlaneOrigin[3];
    double PlaneAxisX[3];
    double PlaneAxisY[3];
    double PlaneAxisZ[3];
    
    double GlobalPlaneOrigin[3];
    double GlobalPlaneAxisX[3];
    double GlobalPlaneAxisY[3];
    double GlobalPlaneAxisZ[3];

    double PlaneNormal[3];
    
    vtkLinearTransform *TransformSlice;
    
    // color mapping
    double Window;
    double Level;
    
    int   AutoSpacing;
    
    // look-up tables and caches
    vtkTimeStamp PreprocessingTime;
    
    double*       VoxelCoordinates[3];
    double       DataOrigin[3];
    double        DataBounds[3][2];
    int          DataDimensions[3];
    double        SamplingTableMultiplier[3];

private:
    vtkMAFVolumeSlicer(const vtkMAFVolumeSlicer&);  // Not implemented.
    void operator=(const vtkMAFVolumeSlicer&);  // Not implemented.
};

#endif
