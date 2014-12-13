#include "../../project2_test.h"
#include "../mgmap.h"
#include "../mgpathitem.h"
#include "../stubs/mgframeworkstub.h"

void Project2Test::test_MGMap_initialize()
{
	MGMap map;
	MGFrameworkStub mgf;

	map.init(10, 10, 16, 16, 640, 480);
	ASSERT_EQ(map.getWidth(), 10, "Map was created with wrong width");
	ASSERT_EQ(map.getHeight(), 10, "Map was created with wrong height");
	ASSERT_EQ(map.getTileWidth(), 16, "Map was created with wrong tile width");
	ASSERT_EQ(map.getTileHeight(), 16, "Map was created with wrong tile height");
	ASSERT_EQ(map.getWindowWidth(), 640, "Map was created with wrong window width");
	ASSERT_EQ(map.getWindowHeight(), 480, "Map was created with wrong window height");

	map.reInit(12, 16, 32, 32);
	ASSERT_EQ(map.getWidth(), 12, "Map was created with wrong width");
	ASSERT_EQ(map.getHeight(), 16, "Map was created with wrong height");
	ASSERT_EQ(map.getTileWidth(), 32, "Map was created with wrong tile width");
	ASSERT_EQ(map.getTileHeight(), 32, "Map was created with wrong tile height");
	ASSERT_EQ(map.getWindowWidth(), 640, "Map was created with wrong window width");
	ASSERT_EQ(map.getWindowHeight(), 480, "Map was created with wrong window height");

	map.runConsoleCommand("map setsize 23 25 16 16", &mgf, NULL);
	ASSERT_EQ(map.getWidth(), 23, "Map was created with wrong width");
	ASSERT_EQ(map.getHeight(), 25, "Map was created with wrong height");
	ASSERT_EQ(map.getTileWidth(), 16, "Map was created with wrong tile width");
	ASSERT_EQ(map.getTileHeight(), 16, "Map was created with wrong tile height");
	ASSERT_EQ(map.getWindowWidth(), 640, "Map was created with wrong window width");
	ASSERT_EQ(map.getWindowHeight(), 480, "Map was created with wrong window height");
}

void Project2Test::test_MGMap_calculateOneStepPath()
{
	// Setup
	MGMap map;
	map.init(10, 10, 16, 16, 640, 480);

	// Trigger
	std::list<MGPathItem> path = map.calculatePath(MGFBASICPATH1, 3, 3, 4, 4);

	// Verify
	ASSERT_EQ(path.size(), 2, "Path of incorrect length was created with basic algorithm");
	ASSERT_EQ(path.front().getX(), 3, "Basic algorithm path content is incorrect");
	ASSERT_EQ(path.front().getY(), 3, "Basic algorithm path content is incorrect");
	ASSERT_EQ(path.back().getX(), 4, "Basic algorithm path content is incorrect");
	ASSERT_EQ(path.back().getY(), 4, "Basic algorithm path content is incorrect");

	// Trigger
	path = map.calculatePath(MGFASTARLIST, 3, 3, 4, 4);

	// Verify
	ASSERT_EQ(path.size(), 2, "Path of incorrect length was created with A* algorithm");
	ASSERT_EQ(path.front().getX(), 3, "A* algorithm path content is incorrect");
	ASSERT_EQ(path.front().getY(), 3, "A* algorithm path content is incorrect");
	ASSERT_EQ(path.back().getX(), 4, "A* algorithm path content is incorrect");
	ASSERT_EQ(path.back().getY(), 4, "A* algorithm path content is incorrect");
}