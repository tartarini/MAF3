#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafGUIRegistration.h>
#include <mafGUIManager.h>

using namespace mafCore;
using namespace mafGUI;

/**
 Class name: mafGUIManagerTest
 This class implements the test suite for mafGUIManager.
 */
class mafGUIManagerTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        // Register all the creatable objects for the mafResources module.
        mafGUIRegistration::registerGUIObjects();
        m_GUIManager = mafNEW(mafGUI::mafGUIManager);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_GUIManager);
    }

    /// mafGUIManager allocation test case.
    void mafGUIManagerAllocationTest();

private:
    mafGUIManager *m_GUIManager; ///< Reference to the GUI Manager.
};

void mafGUIManagerTest::mafGUIManagerAllocationTest() {
    QVERIFY(m_GUIManager != NULL);
}
