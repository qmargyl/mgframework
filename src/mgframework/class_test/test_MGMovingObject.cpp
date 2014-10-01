#include "../../project2_test.h"
#include "../mgmovingobject.h"
#include "../mgpathitem.h"
#include "../mgpathgenerator.h"
#include "../stubs/mgframeworkstub.h"
#include "../mgmap.h"
#include "../stubs/imgwindowimpl.h"

void Project2Test::test_MGMovingObject_initialize()
{
	// Setup
	MGMovingObject m;
	ASSERT_NOT_EQUAL(m.isMarked(), false, "MO was created marked");
	ASSERT_NOT_EQUAL(m.isCreated(), true, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isIdle(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isMoving(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isStuck(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_NOT_EQUAL(m.getTileX(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getTileY(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getDestTileX(), 0, "MO created with wrong destination");
	ASSERT_NOT_EQUAL(m.getDestTileY(), 0, "MO created with wrong destination");
	ASSERT_NOT_EQUAL(m.getXOffset(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getYOffset(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getOwner(), 0, "MO created with wrong owner");

	// Trigger
	m.initialize();

	// Verify
	ASSERT_NOT_EQUAL(m.isMarked(), false, "MO was created marked");
	ASSERT_NOT_EQUAL(m.isCreated(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isIdle(), true, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isMoving(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isStuck(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_NOT_EQUAL(m.getTileX(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getTileY(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getDestTileX(), 0, "MO created with wrong destination");
	ASSERT_NOT_EQUAL(m.getDestTileY(), 0, "MO created with wrong destination");
	ASSERT_NOT_EQUAL(m.getXOffset(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getYOffset(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getOwner(), 0, "MO created with wrong owner");
}

void Project2Test::test_MGMovingObject_mark()
{
	// Setup
	MGMovingObject m;
	ASSERT_NOT_EQUAL(m.isMarked(), false, "MO was created marked");

	// Trigger
	m.mark();

	// Verify
	ASSERT_NOT_EQUAL(m.isMarked(), true, "MO was not marked");

	// Trigger
	m.unMark();
	ASSERT_NOT_EQUAL(m.isMarked(), false, "MO was marked");
}

void Project2Test::test_MGMovingObject_setPathSingleStep()
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

	// Initialize the MO
	ASSERT_NOT_EQUAL(m.isCreated(), true, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isIdle(), false, "MO was created in wrong state");
	m.initialize();
	ASSERT_NOT_EQUAL(m.isCreated(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isIdle(), true, "MO was created in wrong state");

	// Set MO location
	m.setTileXY(3, 3, &mgf);

	ASSERT_NOT_EQUAL(m.isCreated(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isIdle(), true, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isMoving(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.isStuck(), false, "MO was created in wrong state");
	ASSERT_NOT_EQUAL(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_NOT_EQUAL(m.getTileX(), 3, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getTileY(), 3, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getDestTileX(), 0, "MO created with wrong destination");
	ASSERT_NOT_EQUAL(m.getDestTileY(), 0, "MO created with wrong destination");
	ASSERT_NOT_EQUAL(m.getXOffset(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getYOffset(), 0, "MO created in wrong location");
	ASSERT_NOT_EQUAL(m.getOwner(), 0, "MO created with wrong owner");

	// Trigger (set a path from current location to one tile away)
	m.setPath(map.calculatePath(MGFBASICPATH1, 3, 3, 4, 4));

	// Verify
	ASSERT_NOT_EQUAL(m.isCreated(), false, "MO in wrong state");
	ASSERT_NOT_EQUAL(m.isIdle(), true, "MO in wrong state");
	ASSERT_NOT_EQUAL(m.isMoving(), false, "MO in wrong state");
	ASSERT_NOT_EQUAL(m.isStuck(), false, "MO in wrong state");
	ASSERT_NOT_EQUAL(m.anyMovingMO(), false, "Some MO are moving");
	ASSERT_NOT_EQUAL(m.getTileX(), 3, "MO in wrong location");
	ASSERT_NOT_EQUAL(m.getTileY(), 3, "MO in wrong location");
	ASSERT_NOT_EQUAL(m.getDestTileX(), 3, "MO with wrong destination");
	ASSERT_NOT_EQUAL(m.getDestTileY(), 3, "MO with wrong destination");
	ASSERT_NOT_EQUAL(m.getXOffset(), 0, "MO in wrong location");
	ASSERT_NOT_EQUAL(m.getYOffset(), 0, "MO in wrong location");

	// Trigger
//	mgf.runConsoleCommand("runoneframe", &mgf, NULL);
	m.update(&mgf);

	// TODO: Investigate why the MO state is not updated at this point

	// Verify
//	ASSERT_NOT_EQUAL(m.isCreated(), false, "MO in wrong state");
//	ASSERT_NOT_EQUAL(m.isIdle(), false, "MO in wrong state");
//	ASSERT_NOT_EQUAL(m.isMoving(), true, "MO in wrong state");
//	ASSERT_NOT_EQUAL(m.isStuck(), false, "MO in wrong state");
//	ASSERT_NOT_EQUAL(m.anyMovingMO(), true, "Some MO are moving");
//	ASSERT_NOT_EQUAL(m.getTileX(), 3, "MO in wrong location");
//	ASSERT_NOT_EQUAL(m.getTileY(), 3, "MO in wrong location");
//	ASSERT_NOT_EQUAL(m.getDestTileX(), 4, "MO with wrong destination");
//	ASSERT_NOT_EQUAL(m.getDestTileY(), 4, "MO with wrong destination");
//	ASSERT_NOT_EQUAL(m.getXOffset() != 0, true, "MO in wrong location");
//	ASSERT_NOT_EQUAL(m.getYOffset() != 0, true, "MO in wrong location");

	m.printHistory();
}