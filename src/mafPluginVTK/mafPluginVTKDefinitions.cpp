/*
 *  mafPipesLibraryDefinitions.cpp
 *  mafPipesLibrary
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginVTKDefinitions.h"

#include <vtkAlgorithmOutput.h>
#include <vtkAlgorithm.h>
#include <vtkDataObject.h>

QString vtkClassTypeNameExtract(vtkAlgorithmOutput *algorithmOutput) {
    if(algorithmOutput != NULL) {
        vtkAlgorithm *producer = algorithmOutput->GetProducer();
        vtkDataObject *data = producer->GetOutputDataObject(0);
        return data->GetClassName();
    } 
    return "";
}