/*
 *  mafTestRegistration.cpp
 *  mafTestSuiteEngine
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTESTREGISTRATION_
#define MAFTESTREGISTRATION_

// Includes list
#include "mafTestRegistry.h"

namespace mafQA {

/**
 * A macro to register a test class.
 *
 * This macro will create a static variable which registers the
 * testclass with the TestRegistry, and creates an instance of the
 * test class.
 *
 * Execute this macro in the body of your unit test's .cpp file, e.g.
 *    class MyTest {
 *			...
 *		};
 *
 *		MAF_REGISTER_TEST(MyTest);
 */

#define MAF_REGISTER_TEST(TestClass) \
    static mafTestRegistration<TestClass> TestClass##Registration

	/**
	 * A wrapper class around a test to manage registration and static
	 * creation of an instance of the test class.
     * This class is used by MAF_REGISTER_TEST(), and you should not
	 * use this class directly.
	 */
    template<typename T>
    class mafTestRegistration {
		public:
            ///!brief Registration class constructor.
            /** The constructor also register also the test passed as typename into the test suite registry. */
            mafTestRegistration() : m_TestToRegister(NULL){
                m_TestToRegister = new T();
				if(mafTestRegistry::instance()->isRegistered(m_TestToRegister) == false) {
                    mafTestRegistry::instance()->registerTest(m_TestToRegister);
				} else {
					delete m_TestToRegister;
					m_TestToRegister = NULL;
				}
			}

            ///!brief Registration class destructor
            ~mafTestRegistration() {
				if(m_TestToRegister) {
                    delete m_TestToRegister;
				}
			}
		
		private:
            T* m_TestToRegister; ///< Test to be registered into the test suite.
	};

}

#endif //MAFTESTREGISTRATION_
