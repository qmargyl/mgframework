#include "../../project2_test.h"
#include "../mgmovingobject.h"

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