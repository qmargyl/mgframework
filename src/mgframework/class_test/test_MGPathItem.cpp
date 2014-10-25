#include "../../project2_test.h"
#include "../mgpathitem.h"

void Project2Test::test_MGPathItem_createPathItemXY()
{
	// Trigger
	MGPathItem p1(13, 14);

	// Verify
	ASSERT_NOT_EQUAL(p1.getX(), 13, "Incorrect X");
	ASSERT_NOT_EQUAL(p1.getY(), 14, "Incorrect Y");
	ASSERT_NOT_EQUAL((int)p1.getH(), 1, "Incorrect H");
}

void Project2Test::test_MGPathItem_createPathItemXYH()
{
	// Trigger
	MGPathItem p1(567, 789, 13.5);

	// Verify
	ASSERT_NOT_EQUAL(p1.getX(), 567, "Incorrect X");
	ASSERT_NOT_EQUAL(p1.getY(), 789, "Incorrect Y");
	ASSERT_NOT_EQUAL(p1.getH() > 13.4, true, "Incorrect H");
	ASSERT_NOT_EQUAL(p1.getH() < 13.6, true, "Incorrect H");
}

void Project2Test::test_MGPathItem_setH()
{
	// Setup
	MGPathItem p1(13, 14);
	ASSERT_NOT_EQUAL((int)p1.getH(), 1, "Incorrect H");

	// Trigger
	p1.setH(9.6);

	// Verify
	ASSERT_NOT_EQUAL(p1.getH() > 9.5, true, "Incorrect H");
	ASSERT_NOT_EQUAL(p1.getH() < 9.7, true, "Incorrect H");
}

void Project2Test::test_MGPathItem_equalCoordinate()
{
	// Setup
	MGPathItem p1(13, 14);
	MGPathItem p2(13, 14);
	MGPathItem p3(12, 14);
	MGPathItem p4(13, 15);

	// Verify
	ASSERT_NOT_EQUAL(p1.equalCoordinate(&p1), true, "equalCoordinate returned incorrectly");
	ASSERT_NOT_EQUAL(p1.equalCoordinate(&p2), true, "equalCoordinate returned incorrectly");
	ASSERT_NOT_EQUAL(p1.equalCoordinate(&p3), false, "equalCoordinate returned incorrectly");
	ASSERT_NOT_EQUAL(p1.equalCoordinate(&p4), false, "equalCoordinate returned incorrectly");
}