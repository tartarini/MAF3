/*
 *  vtkMAFLog.h
 *  vtkPluginVTK
 *
 *  Created by Paolo Quadrani on 16/01/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef VTKMAFLOG_H
#define VTKMAFLOG_H

#include "vtkMAF_global.h"
#include <vtkOutputWindow.h>

//----------------------------------------------------------------------------
// forward refs
//----------------------------------------------------------------------------

/**
 Class Name: vtkMAFLog
 This class allows to forward VTK log messages into the MAF3 logging system.
 */
class VTK_vtkMAF_EXPORT vtkMAFLog : public vtkOutputWindow {
public:
	static vtkMAFLog *New();
	vtkTypeMacro(vtkMAFLog, vtkOutputWindow);

	/** Enable/Disable log message displaying. */
	vtkSetMacro(Enabled,int);

	/** Enable/Disable log message displaying. */
	vtkGetMacro(Enabled,int);

	/** Enable/Disable log message displaying. */
	vtkBooleanMacro(Enabled,int);

	/** Put the text into the log file. New lines are converted to carriage return new lines. */
	/*virtual*/ void DisplayText(const char*);

	/** Put the text into the log file. New lines are converted to carriage return new lines. */
	/*virtual*/ void DisplayErrorText(const char*);

	/** Put the text into the log file. New lines are converted to carriage return new lines. */
	/*virtual*/ void DisplayWarningText(const char*);

	/** Put the text into the log file. New lines are converted to carriage return new lines. */
	/*virtual*/ void DisplayGenerigWarningText(const char*);

protected:
	vtkMAFLog();
	~vtkMAFLog();

	int Enabled;

private:
	vtkMAFLog(const vtkMAFLog&);       // Not implemented.
	void operator=(const vtkMAFLog&);  // Not implemented.
};

#endif VTKMAFLOG_H
