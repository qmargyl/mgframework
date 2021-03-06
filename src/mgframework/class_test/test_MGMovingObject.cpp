#include "../../project2_test.h"
#include "../mgmovingobject.h"
#include "../mgpathitem.h"
#include "../mgpathgenerator.h"
#include "../stubs/mgframeworkstub.h"
#include "../mgmap.h"
#include "../stubs/imgwindowimpl.h"

void Project2Test::test_MGMovingObject_initialize()
{
	// Trigger
	MGMovingObject m;
	ASSERT_EQ(m.isMarked(), false, "MO was created marked");
	ASSERT_EQ(m.isIdle(), true, "MO was created in wrong state");
	ASSERT_EQ(m.isMoving(), false, "MO was created in wrong state");
	ASSERT_EQ(m.isStuck(), false, "MO was created in wrong state");
	ASSERT_EQ(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_EQ(m.getTileX(), 0, "MO created in wrong location");
	ASSERT_EQ(m.getTileY(), 0, "MO created in wrong location");
	ASSERT_EQ(m.getNextTileX(), 0, "MO created with wrong destination");
	ASSERT_EQ(m.getNextTileY(), 0, "MO created with wrong destination");
	ASSERT_EQ(m.getXOffset(), 0, "MO created in wrong location");
	ASSERT_EQ(m.getYOffset(), 0, "MO created in wrong location");
	ASSERT_EQ(m.getOwner(), 0, "MO created with wrong owner");
}

void Project2Test::test_MGMovingObject_mark()
{
	// Setup
	MGMovingObject m;
	ASSERT_EQ(m.isMarked(), false, "MO was created marked");

	// Trigger
	m.mark();

	// Verify
	ASSERT_EQ(m.isMarked(), true, "MO was not marked");

	// Trigger
	m.unMark();
	ASSERT_EQ(m.isMarked(), false, "MO was marked");
}

void Project2Test::test_MGMovingObject_setBasicPathStartMoving()
{
	// Setup
	MGMovingObject m;
	m.enableHistory();
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(20, 20, 16, 16);
	MGMap map;
	map.init(20, 20, 16, 16, 1024, 768);
	m.setSpeed(0.5, 16); // Two tiles per second
	ASSERT_EQ(m.isIdle(), true, "MO was created in wrong state");

	// Set MO location
	m.setTileXY(3, 3, &mgf);

	ASSERT_EQ(m.isIdle(), true, "MO was created in wrong state");
	ASSERT_EQ(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_EQ(m.getTileX(), 3, "MO created in wrong location");
	ASSERT_EQ(m.getTileY(), 3, "MO created in wrong location");
	ASSERT_EQ(m.getNextTileX(), 0, "MO created with wrong destination");
	ASSERT_EQ(m.getNextTileY(), 0, "MO created with wrong destination");
	ASSERT_EQ(m.getXOffset(), 0, "MO created in wrong location");
	ASSERT_EQ(m.getYOffset(), 0, "MO created in wrong location");
	ASSERT_EQ(m.getOwner(), 0, "MO created with wrong owner");

	// Trigger (set a basic path from current location to one tile away)
	m.setPath(map.calculatePath(MGFBASICPATH1, 3, 3, 4, 4));

	// Verify
	ASSERT_EQ(m.isIdle(), true, "MO in wrong state");
	ASSERT_EQ(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_EQ(m.getTileX(), 3, "MO in wrong location");
	ASSERT_EQ(m.getTileY(), 3, "MO in wrong location");
	ASSERT_EQ(m.getNextTileX(), 0, "MO with wrong destination");
	ASSERT_EQ(m.getNextTileY(), 0, "MO with wrong destination");
	ASSERT_EQ(m.getXOffset(), 0, "MO in wrong location");
	ASSERT_EQ(m.getYOffset(), 0, "MO in wrong location");

	// Trigger
	m.update(&mgf);
	// Now the current location should have been removed from the path. MO still idle.
	m.update(&mgf);
	// New destination tile set. MO is moving.

	// Verify
	m.printHistory();
	ASSERT_EQ(m.isMoving(), true, "MO in wrong state");
	ASSERT_EQ(m.anyMovingMO(), true, "Some MO are moving");
	ASSERT_EQ(m.getTileX(), 3, "MO in wrong location");
	ASSERT_EQ(m.getTileY(), 3, "MO in wrong location");
	ASSERT_EQ(m.getNextTileX(), 4, "MO with wrong destination");
	ASSERT_EQ(m.getNextTileY(), 4, "MO with wrong destination");
}

void Project2Test::test_MGMovingObject_setAStarPathStartMoving()
{
	// Setup
	MGMovingObject m;
	m.enableHistory();
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(20, 20, 16, 16);
	MGMap map;
	map.init(20, 20, 16, 16, 1024, 768);
	m.setSpeed(0.5, 16); // Two tiles per second
	ASSERT_EQ(m.isIdle(), true, "MO was created in wrong state");

	// Set MO location
	m.setTileXY(3, 3, &mgf);

	ASSERT_EQ(m.isIdle(), true, "MO was created in wrong state");
	ASSERT_EQ(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_EQ(m.getTileX(), 3, "MO created in wrong location");
	ASSERT_EQ(m.getTileY(), 3, "MO created in wrong location");
	ASSERT_EQ(m.getNextTileX(), 0, "MO created with wrong destination");
	ASSERT_EQ(m.getNextTileY(), 0, "MO created with wrong destination");
	ASSERT_EQ(m.getXOffset(), 0, "MO created in wrong location");
	ASSERT_EQ(m.getYOffset(), 0, "MO created in wrong location");
	ASSERT_EQ(m.getOwner(), 0, "MO created with wrong owner");

	// Trigger (set an A* path from current location to one tile away)
	m.setPath(map.calculatePath(MGFASTARLIST, 3, 3, 4, 4));

	// Verify
	ASSERT_EQ(m.isIdle(), true, "MO in wrong state");
	ASSERT_EQ(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_EQ(m.getTileX(), 3, "MO in wrong location");
	ASSERT_EQ(m.getTileY(), 3, "MO in wrong location");
	ASSERT_EQ(m.getNextTileX(), 0, "MO with wrong destination");
	ASSERT_EQ(m.getNextTileY(), 0, "MO with wrong destination");
	ASSERT_EQ(m.getXOffset(), 0, "MO in wrong location");
	ASSERT_EQ(m.getYOffset(), 0, "MO in wrong location");

	// Trigger
	m.update(&mgf);
	// Now the current location should have been removed from the path. MO still idle.
	m.update(&mgf);
	// New destination tile set. MO is moving.

	// Verify
	m.printHistory();
	ASSERT_EQ(m.isMoving(), true, "MO in wrong state");
	ASSERT_EQ(m.anyMovingMO(), true, "No MO is moving");
	ASSERT_EQ(m.getTileX(), 3, "MO in wrong location");
	ASSERT_EQ(m.getTileY(), 3, "MO in wrong location");
	ASSERT_EQ(m.getNextTileX(), 4, "MO with wrong destination");
	ASSERT_EQ(m.getNextTileY(), 4, "MO with wrong destination");
}

void Project2Test::test_MGMovingObject_setAStarPathGetStuck()
{
	// Setup
	MGMovingObject m;
	m.enableHistory();
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(20, 20, 16, 16);
	MGMap map;
	map.init(20, 20, 16, 16, 1024, 768);
	m.setSpeed(0.5, 16); // Two tiles per second
	ASSERT_EQ(m.isIdle(), true, "MO was created in wrong state");

	// Set MO location and path
	m.setTileXY(3, 3, &mgf);
	m.setPath(map.calculatePath(MGFASTARLIST, 3, 3, 4, 4));
	ASSERT_EQ(m.isIdle(), true, "MO in wrong state");
	m.update(&mgf);
	// Now the current location should have been removed from the path. MO still idle.
	ASSERT_EQ(m.isIdle(), true, "MO in wrong state");

	// Create another MO on the destination tile to make it occupied
	mgf.runConsoleCommand("add mo 1 -x 4 -y 4 -owner 123", &mgf, NULL);

	// Trigger
	m.update(&mgf);

	// Verify - Wanted destination tile is occupied -> MO is stuck.
	m.printHistory();
	ASSERT_EQ(m.isStuck(), true, "MO in wrong state");
	ASSERT_EQ(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_EQ(m.getTileX(), 3, "MO in wrong location");
	ASSERT_EQ(m.getTileY(), 3, "MO in wrong location");
	ASSERT_EQ(m.getNextTileX(), 3, "MO with wrong destination");
	ASSERT_EQ(m.getNextTileY(), 3, "MO with wrong destination");
}