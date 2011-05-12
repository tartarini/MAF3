/*
 *  mafExternalDataCodecVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafExternalDataCodecVTK.h"
#include <vtkSmartPointer.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetWriter.h>
#include <vtkDataSet.h>
#include <vtkAlgorithmOutput.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafExternalDataCodecVTK::mafExternalDataCodecVTK(const QString code_location) : mafExternalDataCodec(code_location), m_Reader(NULL) {
}

mafExternalDataCodecVTK::~mafExternalDataCodecVTK() {
}

char *mafExternalDataCodecVTK::encode(bool binary) {
    int vtk_err = VTK_OK;

    vtkSmartPointer<vtkDataSetWriter> writer = vtkSmartPointer<vtkDataSetWriter>::New();
    writer->WriteToOutputStringOn();
    if (binary) {
        writer->SetFileTypeToBinary();
    }

    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, this->m_ExternalData);
    writer->SetInputConnection(*dataSet);
    writer->Write();
    writer->Update();
    vtk_err = writer->GetErrorCode();
    if (vtk_err == VTK_ERROR) {
      return "";
    }
    this->m_StringSize = writer->GetOutputStringLength();
    char *output_string = new char[this->m_StringSize+1];
    if (binary) {
        unsigned char *binaryOutput = writer->GetBinaryOutputString();
        memcpy(output_string,binaryOutput,this->m_StringSize+1);
    } else {
        QString output = writer->GetOutputString();
        //vtkDataSetWriter write some junk character at the end of the string,
        // so i must truncate the string..
        output.truncate(this->m_StringSize);
        memcpy(output_string,output.toAscii(),this->m_StringSize+1);
    }
    return output_string;
}

void mafExternalDataCodecVTK::decode(const char *input_string, bool binary) {
    REQUIRE(input_string != NULL);

    m_Reader = vtkDataSetReader::New();
    m_Reader->ReadFromInputStringOn();

    if (binary) {
        m_Reader->SetBinaryInputString(input_string, this->stringSize());
    } else {
       m_Reader->SetInputString(input_string, this->stringSize());
    }

    m_Reader->Update();
    m_Data = new mafProxy<vtkAlgorithmOutput>();
    *m_Data = m_Reader->GetOutputPort(0);
    this->m_ExternalData = m_Data;
}
