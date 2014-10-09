#include "../../project2_test.h"
#include "../mgpathitem.h"
#include "../stubs/mgframeworkstub.h"
#include "../stubs/imgwindowimpl.h"
#include "../mgsymboltable.h"

void Project2Test::test_MGFramework_runOneFrame()
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

void Project2Test::test_MGFramework_fullscreen()
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

void Project2Test::test_MGFramework_toIntFromConstant()
{
	// Setup
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(16, 16, 32, 32);
	mgf.runConsoleCommand("add mo 11", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf.toInt(std::string("17"), NULL), 17, "Integer constant not converted properly");
	ASSERT_NOT_EQUAL(mgf.toInt(std::string("-13"), NULL), -13, "Integer constant not converted properly");
	ASSERT_NOT_EQUAL(mgf.toInt(std::string("getnumberofmo"), NULL), 11, "Integer macro not converted properly");
}

void Project2Test::test_MGFramework_toIntFromSymbol()
{
	// Setup
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	MGSymbolTable symbols;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(16, 16, 32, 32);
	symbols.addSymbol(std::string("s1"), 93);
	symbols.addSymbol(std::string("s2"), -94);
	symbols.addSymbol(std::string("This is a valid symbol name in MGSymbolTable"), 666);

	// Verify
	ASSERT_NOT_EQUAL(mgf.toInt(std::string("s1"), &symbols), 93, "Integer symbol not converted properly");
	ASSERT_NOT_EQUAL(mgf.toInt(std::string("s2"), &symbols), -94, "Integer symbol not converted properly");
	ASSERT_NOT_EQUAL(mgf.toInt(std::string("This is a valid symbol name in MGSymbolTable"), &symbols), 666, "Integer symbol not converted properly");
}

void Project2Test::test_MGFramework_toBoolFromConstant()
{
	// Setup
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(16, 16, 32, 32);
	mgf.runConsoleCommand("add mo 11", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("17"), NULL), true, "Bool constant not converted properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("-13"), NULL), true, "Bool constant not converted properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("getnumberofmo"), NULL), true, "Bool macro not converted properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("on"), NULL), true, "Bool constant not converted properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("off"), NULL), false, "Bool constant not converted properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("0"), NULL), false, "Bool constant not converted properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("1"), NULL), true, "Bool constant not converted properly");
}