/*
 *  vtkMAFLog.h
 *  vtkMAF
 *
 *  Created by Paolo Quadrani on 16/01/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */


#include "vtkMAFLog.h"
#include <vtkObjectFactory.h>

#include <QString>
#include <QDebug>

//----------------------------------------------------------------------------
vtkMAFLog *vtkMAFLog::New() 
//----------------------------------------------------------------------------
{
	vtkObject* ret = vtkObjectFactory::CreateInstance("vtkMAFLog");
	if(ret)
	{
		return (vtkMAFLog*)ret;
	}
	return new vtkMAFLog;
}
//----------------------------------------------------------------------------
vtkMAFLog::vtkMAFLog() 
//----------------------------------------------------------------------------
{
	this->Enabled = 1;
}
//----------------------------------------------------------------------------
vtkMAFLog::~vtkMAFLog() 
//----------------------------------------------------------------------------
{
}
//----------------------------------------------------------------------------
void vtkMAFLog::DisplayText(const char* text)
//----------------------------------------------------------------------------
{
	if(!text)
	{
		return;
	}

	if (this->Enabled)
	{
		QString message("[VTK]");
		message += text;
		qDebug() << message;
	}
}

void vtkMAFLog::DisplayErrorText(const char *text) {
	if(!text)
	{
		return;
	}

	if (this->Enabled)
	{
		QString message("[VTK][Error]");
		message += text;
		qCritical() << message;
	}
}

void vtkMAFLog::DisplayWarningText(const char *text) {
	if(!text)
	{
		return;
	}

	if (this->Enabled)
	{
		QString message("[VTK][Warning]");
		message += text;
		qWarning() << message;
	}
}

void vtkMAFLog::DisplayGenerigWarningText(const char *text) {
	DisplayWarningText(text);
}
