/*
 *  mafTestSuite.h
 *  mafTestSuiteEngine
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTESTSUITE_
#define MAFTESTSUITE_

// Includes list
#include "mafTestRegistration.h"
using namespace mafQA;

#if !defined(EXCLUDE_SINGLETONS)
	#include <mafCoreSingletons.h>
	using namespace mafCore;
	#define MAF_TESTSUITE_MAIN() \
	using namespace mafCore; \
	int main(int argc, char *argv[]) \
	{ \
		QCoreApplication app(argc, argv); \
		mafCoreSingletons::mafSingletonsInitialize(); \
		int result= mafTestRegistry::instance()->runTests(argc, argv); \
		mafCoreSingletons::mafSingletonsShutdown(); \
		return result; \
	}
    #define MAF_GUI_TESTSUITE_MAIN() \
    using namespace mafCore; \
    int main(int argc, char *argv[]) \
    { \
        QApplication app(argc, argv); \
        mafCoreSingletons::mafSingletonsInitialize(); \
        int result= mafTestRegistry::instance()->runTests(argc, argv); \
        mafCoreSingletons::mafSingletonsShutdown(); \
        return result; \
    }
#else
	#define MAF_TESTSUITE_MAIN() \
	int main(int argc, char *argv[]) \
	{ \
		QCoreApplication app(argc, argv); \
		int result= mafTestRegistry::instance()->runTests(argc, argv); \
		return result; \
	}
    #define MAF_GUI_TESTSUITE_MAIN() \
    int main(int argc, char *argv[]) \
    { \
        QApplication app(argc, argv); \
        int result= mafTestRegistry::instance()->runTests(argc, argv); \
        return result; \
    }
#endif

#endif // MAFTESTSUITE_
