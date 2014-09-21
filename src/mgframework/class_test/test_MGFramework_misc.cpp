#include "../mgclasstester.h"
#include "../mgpathitem.h"
#include "../stubs/mgframeworkstub.h"
#include "../stubs/imgwindowimpl.h"


void MGClassTester::test_MGFramework_runOneFrame()
{
	// Setup
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(16, 16, 32, 32);

	// Trigger
	mgf.runConsoleCommand("runoneframe", &mgf, NULL);
}