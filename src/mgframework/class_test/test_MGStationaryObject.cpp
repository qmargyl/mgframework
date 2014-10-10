#include "../../project2_test.h"
#include "../mgpathitem.h"
#include "../mgstationaryobject.h"
#include "../stubs/mgframeworkstub.h"
#include "../mgmap.h"
#include "../stubs/imgwindowimpl.h"

void Project2Test::test_MGStationaryObject_initialize()
{
	// Setup
	MGStationaryObject s;
	ASSERT_NOT_EQUAL(s.getTileX(), 0, "SO created in wrong location");
	ASSERT_NOT_EQUAL(s.getTileY(), 0, "SO created in wrong location");
	ASSERT_NOT_EQUAL(s.getOwner(), 0, "SO created with wrong owner");
	
	// Trigger
	s.initialize();

	// Verify (what exactly?!?)
	ASSERT_NOT_EQUAL(s.getTileX(), 0, "SO created in wrong location");
	ASSERT_NOT_EQUAL(s.getTileY(), 0, "SO created in wrong location");
	ASSERT_NOT_EQUAL(s.getOwner(), 0, "SO created with wrong owner");
}

void Project2Test::test_MGStationaryObject_setTileXY()
{
	// Setup
	MGStationaryObject a; // Dummy
	MGStationaryObject b; // Dummy
	MGStationaryObject c; // Dummy
	MGStationaryObject s;
	s.initialize();
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(20, 20, 16, 16);
	ASSERT_NOT_EQUAL(s.getTileX(), 0, "SO created in wrong location");
	ASSERT_NOT_EQUAL(s.getTileY(), 0, "SO created in wrong location");
	ASSERT_NOT_EQUAL(s.getID() != 0, true, "SO ID not generated correctly");
	ASSERT_NOT_EQUAL(mgf.m_Map.occupant(0, 0), 0, "Map tile is already occupied");

	// Trigger
	s.setTileXY(0, 0, &mgf);
	
	// Verify
	ASSERT_NOT_EQUAL(mgf.m_Map.occupant(0, 0), s.getID(), "SO was not able to occupy a map tile");

	// Trigger
	s.setTileXY(9, 8, &mgf);
	
	// Verify
	ASSERT_NOT_EQUAL(mgf.m_Map.occupant(9, 8), s.getID(), "SO was not able to occupy a map tile");
	ASSERT_NOT_EQUAL(mgf.m_Map.occupant(0, 0), 0, "Map tile is still occupied");
	ASSERT_NOT_EQUAL(s.getTileX(), 9, "SO location not updated correctly");
	ASSERT_NOT_EQUAL(s.getTileY(), 8, "SO location not updated correctly");
}

void Project2Test::test_MGStationaryObject_copy()
{
	// Setup
	MGStationaryObject a;
	MGStationaryObject b;
	MGFrameworkStub mgf;
	IMGWindowImpl win;
	mgf.setWindowProperties(1024, 768, 32, false, std::string("test"), &win);
	mgf.init(20, 20, 16, 16);
	a.initialize();
	b.initialize();
	a.setTileXY(3, 5, &mgf);
	b.setTileXY(1, 1, &mgf);
	a.enableLogging();
	b.disableLogging();
	ASSERT_NOT_EQUAL(a.getTileX(), 3, "SO location not updated correctly");
	ASSERT_NOT_EQUAL(a.getTileY(), 5, "SO location not updated correctly");
	ASSERT_NOT_EQUAL(b.getTileX(), 1, "SO location not updated correctly");
	ASSERT_NOT_EQUAL(b.getTileY(), 1, "SO location not updated correctly");
	ASSERT_NOT_EQUAL(a.getID() == b.getID(), false, "SO ID not generated correctly");
	ASSERT_NOT_EQUAL(a.loggingEnabled() == b.loggingEnabled(), false, "SO logging not set correctly");
	
	// Trigger
	b.copy(&a);
	
	// Verify
	ASSERT_NOT_EQUAL(a.getTileX(), 3, "SO location not copied correctly");
	ASSERT_NOT_EQUAL(a.getTileY(), 5, "SO location not copied correctly");
	ASSERT_NOT_EQUAL(b.getTileX(), 3, "SO location not copied correctly");
	ASSERT_NOT_EQUAL(b.getTileY(), 5, "SO location not copied correctly");
	ASSERT_NOT_EQUAL(a.getID() == b.getID(), true, "SO ID not copied correctly");
	ASSERT_NOT_EQUAL(a.loggingEnabled() == b.loggingEnabled(), true, "SO logging not copied correctly");
}