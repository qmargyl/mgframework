#include "../mgclasstester.h"
#include "../mgpathitem.h"
#include "../stubs/mgframeworkstub.h"


void MGClassTester::test_MGFramework_runOneFrame()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);

	// Trigger
	mgf.runConsoleCommand("runoneframe", &mgf, NULL);
}