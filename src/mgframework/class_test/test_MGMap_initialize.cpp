
#include "../mgclasstester.h"

#include "../mgmap.h"
#include "../stubs/mgframeworkstub.h"

void MGClassTester::test_MGMap_initialize()
{
	MGMap map;
	MGFrameworkStub mgf;

	map.init(10, 10, 16, 16, 640, 480);
	ASSERT_NOT_EQUAL(map.getWidth(), 10, "Map was created with wrong width");
	ASSERT_NOT_EQUAL(map.getHeight(), 10, "Map was created with wrong height");
	ASSERT_NOT_EQUAL(map.getTileWidth(), 16, "Map was created with wrong tile width");
	ASSERT_NOT_EQUAL(map.getTileHeight(), 16, "Map was created with wrong tile height");
	ASSERT_NOT_EQUAL(map.getWindowWidth(), 640, "Map was created with wrong window width");
	ASSERT_NOT_EQUAL(map.getWindowHeight(), 480, "Map was created with wrong window height");

	map.reInit(12, 16, 32, 32);
	ASSERT_NOT_EQUAL(map.getWidth(), 12, "Map was created with wrong width");
	ASSERT_NOT_EQUAL(map.getHeight(), 16, "Map was created with wrong height");
	ASSERT_NOT_EQUAL(map.getTileWidth(), 32, "Map was created with wrong tile width");
	ASSERT_NOT_EQUAL(map.getTileHeight(), 32, "Map was created with wrong tile height");
	ASSERT_NOT_EQUAL(map.getWindowWidth(), 640, "Map was created with wrong window width");
	ASSERT_NOT_EQUAL(map.getWindowHeight(), 480, "Map was created with wrong window height");

	map.runConsoleCommand("map setsize 23 25 16 16", &mgf, NULL);
	ASSERT_NOT_EQUAL(map.getWidth(), 23, "Map was created with wrong width");
	ASSERT_NOT_EQUAL(map.getHeight(), 25, "Map was created with wrong height");
	ASSERT_NOT_EQUAL(map.getTileWidth(), 16, "Map was created with wrong tile width");
	ASSERT_NOT_EQUAL(map.getTileHeight(), 16, "Map was created with wrong tile height");
	ASSERT_NOT_EQUAL(map.getWindowWidth(), 640, "Map was created with wrong window width");
	ASSERT_NOT_EQUAL(map.getWindowHeight(), 480, "Map was created with wrong window height");
}