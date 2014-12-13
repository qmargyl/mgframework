#include "../../project2_test.h"
#include "../mgpathitem.h"
#include "../mgstationaryobject.h"
#include "../stubs/mgframeworkstub.h"
#include "../mgmap.h"
#include "../stubs/imgwindowimpl.h"

void Project2Test::test_MGStationaryObject_setTileXY()
{
	// Setup
	MGStationaryObject a; // Dummy
	MGStationaryObject b; // Dummy
	MGStationaryObject c; // Dummy
	MGStationaryObject s;
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(20, 20, 16, 16);
	ASSERT_EQ(s.getTileX(), 0, "SO created in wrong location");
	ASSERT_EQ(s.getTileY(), 0, "SO created in wrong location");
	ASSERT_EQ(s.getID() != 0, true, "SO ID not generated correctly");
	ASSERT_EQ(mgf.m_Map.occupant(0, 0), 0, "Map tile is already occupied");

	// Trigger
	s.setTileXY(0, 0, &mgf);
	
	// Verify
	ASSERT_EQ(mgf.m_Map.occupant(0, 0), s.getID(), "SO was not able to occupy a map tile");

	// Trigger
	s.setTileXY(9, 8, &mgf);

	// Verify
	ASSERT_EQ(mgf.m_Map.occupant(9, 8), s.getID(), "SO was not able to occupy a map tile");
	ASSERT_EQ(mgf.m_Map.occupant(0, 0), 0, "Map tile is still occupied");
	ASSERT_EQ(s.getTileX(), 9, "SO location not updated correctly");
	ASSERT_EQ(s.getTileY(), 8, "SO location not updated correctly");
}

void Project2Test::test_MGStationaryObject_setType()
{
	MGStationaryObject a;
	ASSERT_EQ(a.getType(), 0, "SO created with wrong type");
	
	a.setType(666);
	ASSERT_EQ(a.getType(), 666, "Failed to set SO type");
}

void Project2Test::test_MGStationaryObject_setTileXYWithoutOccupy()
{
	// Setup
	MGStationaryObject a;

	// Trigger
	a.setTileXY(45, 54, NULL, false);

	// Verify
	ASSERT_EQ(a.getTileX(), 45, "SO location not updated correctly");
	ASSERT_EQ(a.getTileY(), 54, "SO location not updated correctly");
}

void Project2Test::test_MGStationaryObject_setOneTexHandle()
{
	// Setup
	MGStationaryObject a;
	MGTexHandle tH;
	std::vector<MGTexHandle*> tHVec;
	tHVec.push_back(&tH);
	ASSERT_EQ(a.getTexHandle(), NULL, "getTexHandle returned incorrectly");
	ASSERT_EQ(tHVec.size(), 1, "Vector not initialized correctly");
	ASSERT_EQ(tHVec[0], &tH, "Vector not initialized correctly");

	// Trigger
	a.setTexHandle(&tH);

	// Verify
	ASSERT_EQ(a.getTexHandle(), &tH, "getTexHandle returned incorrectly");

	// Trigger
	a.setTexHandle(NULL);

	// Verify
	ASSERT_EQ(a.getTexHandle(), NULL, "getTexHandle returned incorrectly");

	// Trigger
	a.setTexHandle(tHVec);

	// Verify
	ASSERT_EQ(a.getTexHandle(), &tH, "getTexHandle returned incorrectly");
}