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
	mgf.runConsoleCommand("runoneframe", &mgf, NULL);
	mgf.runConsoleCommand("runoneframe", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(win.getFlipSurfaceCounter(), 3, "Wrong number of rendered frames");
}

void MGClassTester::test_MGFramework_fullscreen()
{
	// Setup
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(16, 16, 32, 32);

	// Trigger
	mgf.runConsoleCommand("window fullscreen on", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(win.getActivateFullscreenCounter(), 1, "Wrong number fullscreen activations");
	ASSERT_NOT_EQUAL(win.getDeactivateFullscreenCounter(), 0, "Wrong number fullscreen deactivations");

	// Trigger
	mgf.runConsoleCommand("window fullscreen off", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(win.getActivateFullscreenCounter(), 1, "Wrong number fullscreen activations");
	ASSERT_NOT_EQUAL(win.getDeactivateFullscreenCounter(), 1, "Wrong number fullscreen deactivations");

	// Trigger
	mgf.runConsoleCommand("window fullscreen on", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(win.getActivateFullscreenCounter(), 2, "Wrong number fullscreen activations");
	ASSERT_NOT_EQUAL(win.getDeactivateFullscreenCounter(), 1, "Wrong number fullscreen deactivations");

	// Trigger
	mgf.runConsoleCommand("window fullscreen off", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(win.getActivateFullscreenCounter(), 2, "Wrong number fullscreen activations");
	ASSERT_NOT_EQUAL(win.getDeactivateFullscreenCounter(), 2, "Wrong number fullscreen deactivations");
}