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

	// Also test a non-existing symbol
	EXPECT_ERROR("MGFramework::toInt failed to convert string to integer: this symbol does not exist");
	ASSERT_NOT_EQUAL(mgf.toInt(std::string("this symbol does not exist"), &symbols), 0, "Non-existing integer symbol not converted properly");
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

	// Trigger
	mgf.runConsoleCommand("delete all mo", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("getnumberofmo"), NULL), false, "Bool macro not converted properly");

	// Also test a non-existing constant
	EXPECT_ERROR("MGFramework::toInt failed to convert string to integer: this symbol does not exist");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("this symbol does not exist"), NULL), false, "Bool constant not converted properly");
}

void Project2Test::test_MGFramework_toBoolFromSymbol()
{
	// Setup
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	MGSymbolTable symbols;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(16, 16, 32, 32);
	symbols.addSymbol(std::string("b1"), 47);
	symbols.addSymbol(std::string("b2"), -39);
	symbols.addSymbol(std::string("b3"), 1);
	symbols.addSymbol(std::string("b4"), 0);
	symbols.addSymbol(std::string("-5"), 0); // -5 as an integer is evaluated before the symbol -5

	// Verify
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("b1"), &symbols), true, "Integer symbol not converted to bool properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("b2"), &symbols), true, "Integer symbol not converted to bool properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("b3"), &symbols), true, "Integer symbol not converted to bool properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("b4"), &symbols), false, "Integer symbol not converted to bool properly");
	ASSERT_NOT_EQUAL(mgf.toBool(std::string("-5"), &symbols), true, "Integer constant did not have presidence over symbol");

	// Also test a non-existing symbol
	EXPECT_ERROR("MGFramework::toInt failed to convert string to integer: b0");
	ASSERT_NOT_EQUAL(mgf.toInt(std::string("b0"), &symbols), false, "Non-existing integer symbol not converted to bool properly");
}

void Project2Test::test_MGFramework_unsetWindowProperties()
{
	// Setup
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	ASSERT_NOT_EQUAL(mgf.windowPropertiesSet(), false, "Window properties were not initialized correctly");
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(16, 16, 32, 32);
	ASSERT_NOT_EQUAL(mgf.windowPropertiesSet(), true, "Window properties were not set correctly");
	
	// Trigger
	mgf.unsetWindowProperties();
	
	// Verify
	ASSERT_NOT_EQUAL(mgf.windowPropertiesSet(), false, "Window properties were not unset correctly");
}

void Project2Test::test_MGFramework_setPort()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf.getPort(), 0, "Terminal server port not set correctly");

	// Trigger
	mgf.setPort(123);

	// Verify
	ASSERT_NOT_EQUAL(mgf.getPort(), 123, "Terminal server port not set correctly");

	// Trigger
	mgf.setPort(127);

	// Verify
	ASSERT_NOT_EQUAL(mgf.getPort(), 127, "Terminal server port not set correctly");
}

void Project2Test::test_MGFramework_enableFeatureMiniMap()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf.featureMiniMapEnabled(), true, "MiniMap feature not initialized correctly");

	// Trigger
	mgf.disableFeatureMiniMap();

	// Verify
	ASSERT_NOT_EQUAL(mgf.featureMiniMapEnabled(), false, "MiniMap feature not disabled correctly");

	// Trigger
	mgf.enableFeatureMiniMap();

	// Verify
	ASSERT_NOT_EQUAL(mgf.featureMiniMapEnabled(), true, "MiniMap feature not enabled correctly");
}

void Project2Test::test_MGFramework_countMark()
{
	// Setup
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(16, 16, 32, 32);
	mgf.runConsoleCommand("add mo 6", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMarkedMO(), 0, "Marked MO not initialized correctly");

	// Trigger
	mgf._countMark();
	mgf._countMark();
	mgf._countMark();
	
	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfMarkedMO(), 3, "Marked MO not counted correctly");

	// Trigger
	mgf._countUnMark();
	mgf._countUnMark();
	mgf._countUnMark();
	
	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfMarkedMO(), 0, "Marked MO not counted correctly");
	
	// Also test to count below zero MO, resulting in an error trace
	EXPECT_ERROR("MGFramework::countUnMark decreased number of marked MO below zero");
	mgf._countUnMark();
	
	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfMarkedMO(), 0, "Marked MO not counted correctly");
}

void Project2Test::test_MGFramework_updateFramingBasicCase()
{
	// Setup
	MGFrameworkStub mgf;

	// Trigger
	mgf._activateFraming(17, 19);
	mgf._updateFraming(96, 32);
	mgf._updateFraming(70, 12);
	mgf._updateFraming(64, 90);
	mgf._updateFraming(13, 43);
	mgf._updateFraming(99, 71);
	mgf._deactivateFraming();

	// Verify
	ASSERT_NOT_EQUAL(mgf._getFrameStartX(), 17, "Incorrect frame start x");
	ASSERT_NOT_EQUAL(mgf._getFrameStartY(), 19, "Incorrect frame start y");
	ASSERT_NOT_EQUAL(mgf._getFrameEndX(), 99, "Incorrect frame end x");
	ASSERT_NOT_EQUAL(mgf._getFrameEndY(), 71, "Incorrect frame end y");
}