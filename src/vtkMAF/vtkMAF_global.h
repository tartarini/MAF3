/*
 *  vtkMAF_Global.h
 *  vtkMAF
 *
 *  Created by Paolo Quadrani on 01/07/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef VTKMAF_GLOBAL_H
#define VTKMAF_GLOBAL_H

#if defined(_MSC_VER)
# pragma warning ( disable : 4275 )
#endif

#if defined(_WIN32)
# if defined(vtkMAF_EXPORTS)
#  define VTK_vtkMAF_EXPORT __declspec( dllexport ) 
# else
#  define VTK_vtkMAF_EXPORT __declspec( dllimport ) 
# endif
#else
# define VTK_vtkMAF_EXPORT
#endif

#endif // VTKMAF_GLOBAL_H
