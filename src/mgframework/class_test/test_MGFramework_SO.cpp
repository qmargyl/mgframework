#include "../../project2_test.h"
#include "../stubs/mgframeworkstub.h"


void Project2Test::test_MGFramework_createSO()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF created with SO");

	EXPECT_ERROR("MGFramework::addSO cannot create 1 SO on a 0 by 0 map");
	mgf._addSO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF created SOs without initializing the map");
	mgf.init(16, 16, 32, 32);

	// Trigger/Verify
	mgf._addSO(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF failed to create zero SO");
	mgf._addSO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 1, "MGF failed to create SO");
	mgf._addSO(3);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 4, "MGF failed to create SO");
	mgf._addSO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 5, "MGF failed to create SO");
	mgf._addSO(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 20, "MGF failed to create SO");
	mgf._addSO(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 20, "MGF failed to create zero SO");
}

void Project2Test::test_MGFramework_deleteSO()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF created with SO");
	mgf.init(16, 16, 32, 32);
	mgf._addSO(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 15, "MGF failed to create SO");

	// Trigger/Verify

	// Delete first SO
	mgf._deleteSO(mgf.nthSO(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 14, "MGF failed to delete SO");
	mgf._deleteSO(mgf.nthSO(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 13, "MGF failed to delete SO");
	mgf._deleteSO(mgf.nthSO(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 12, "MGF failed to delete SO");

	// Delete last SO
	mgf._deleteSO(mgf.nthSO(11));
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 11, "MGF failed to delete SO");

	// Delete arbitrary SO
	mgf._deleteSO(mgf.nthSO(7));
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 10, "MGF failed to delete SO");

	// Delete SO outside container
	mgf._deleteSO(mgf.nthSO(17));
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 10, "MGF failed to delete zero SO");

	// Delete all SO
	mgf._deleteAllSO();
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF failed to delete SO");

	// Delete all SO when no SO exist
	mgf._deleteAllSO();
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF failed to run deleteAllSO");
}

void Project2Test::test_MGFramework_deleteFirstSO()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF created with SO");
	mgf.init(16, 16, 32, 32);
	mgf._addSO(2);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 2, "MGF failed to create SO");

	std::list<MGStationaryObject>::iterator it = mgf.nthSO(0);
	mgf.nthSO(0)->setOwner(0);
	mgf.nthSO(1)->setOwner(1);

	// Trigger - delete first SO
	mgf._deleteSO(it);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 1, "MGF failed to delete SO");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getOwner(), 1, "MGF did not delete the right SO");

	// Setup - create another 3 SO
	it = mgf.nthSO(0);
	mgf._addSO(3);
	mgf.nthSO(0)->setOwner(0);
	mgf.nthSO(1)->setOwner(1);
	mgf.nthSO(2)->setOwner(2);
	mgf.nthSO(3)->setOwner(3);

	// Trigger - delete first SO again
	mgf._deleteSO(it);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 3, "MGF failed to delete SO");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getOwner(), 1, "MGF did not delete the right SO");
	ASSERT_NOT_EQUAL(mgf.nthSO(1)->getOwner(), 2, "MGF did not delete the right SO");
	ASSERT_NOT_EQUAL(mgf.nthSO(2)->getOwner(), 3, "MGF did not delete the right SO");
}

void Project2Test::test_MGFramework_deleteFewSO()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);

	// Create the SO in order from top to bottom on screen so that
	// the order is kept when they are sorted.
	mgf.runConsoleCommand("add so 1 -x 5 -y 1", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 1, "MGF failed to create SO");
	ASSERT_NOT_EQUAL(mgf._m_SO().size(), 1, "MGF failed to create SO");
	mgf.nthSO(0)->setOwner(1);
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getOwner(), 1, "MGF failed to set SO owner");

	mgf.runConsoleCommand("add so 1 -x 5 -y 2", &mgf, NULL);
	mgf.runConsoleCommand("add so 1 -x 5 -y 3", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 3, "MGF failed to create SO");
	ASSERT_NOT_EQUAL(mgf._m_SO().size(), 3, "MGF failed to create SO");
	mgf.nthSO(1)->setOwner(2);
	mgf.nthSO(2)->setOwner(2);
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getOwner(), 1, "MGF failed to set SO owner");
	ASSERT_NOT_EQUAL(mgf.nthSO(1)->getOwner(), 2, "MGF failed to set SO owner");
	ASSERT_NOT_EQUAL(mgf.nthSO(2)->getOwner(), 2, "MGF failed to set SO owner");

	mgf.runConsoleCommand("add so 1 -x 5 -y 4", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 4, "MGF failed to create SO");
	ASSERT_NOT_EQUAL(mgf._m_SO().size(), 4, "MGF failed to create SO");
	mgf.nthSO(3)->setOwner(3);
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getOwner(), 1, "MGF failed to setup SO owner");
	ASSERT_NOT_EQUAL(mgf.nthSO(1)->getOwner(), 2, "MGF failed to setup SO owner");
	ASSERT_NOT_EQUAL(mgf.nthSO(2)->getOwner(), 2, "MGF failed to setup SO owner");
	ASSERT_NOT_EQUAL(mgf.nthSO(3)->getOwner(), 3, "MGF failed to setup SO owner");

	// Trigger - delete the two middle SO out of the four
	mgf._deleteSO(mgf.nthSO(1));
	mgf._deleteSO(mgf.nthSO(1));

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 2, "MGF failed to delete SO");
	ASSERT_NOT_EQUAL(mgf._m_SO().size(), 2, "MGF failed to delete SO");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getOwner(), 1, "MGF failed to set SO owner");
	ASSERT_NOT_EQUAL(mgf.nthSO(1)->getOwner(), 3, "MGF failed to set SO owner");
}

void Project2Test::test_MGFramework_deleteAllSO()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF created with SO");
	mgf.init(16, 16, 32, 32);
	mgf._addSO(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 15, "MGF failed to create SO");

	// Trigger
	mgf.runConsoleCommand("delete all so", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF failed to delete all SO");

	// Delete all SO when no SO exist
	mgf.runConsoleCommand("delete all so", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 0, "MGF failed to delete all SO on empty SO list");
}

void Project2Test::test_MGFramework_setupSO()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);
	mgf._addSO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 1, "MGF failed to create SO");
	ASSERT_NOT_EQUAL(mgf._m_SO().size() == 0, false, "MGF failed to create SO");

	// Trigger
	mgf._setupSO(mgf.nthSO(0), 4, 5);

	// Verify
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileX(), 4, "MGF failed to setup SO x");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileY(), 5, "MGF failed to setup SO y");
}

void Project2Test::test_MGFramework_resetupSO()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);
	mgf._addSO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 1, "MGF failed to create SO");
	ASSERT_NOT_EQUAL(mgf._m_SO().size() == 0, false, "MGF failed to create SO");

	mgf._setupSO(mgf.nthSO(0), 4, 5);
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileX(), 4, "MGF failed to setup SO x");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileY(), 5, "MGF failed to setup SO y");

	// Trigger
	mgf._setupSO(mgf.nthSO(0), 8, 9);
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileX(), 8, "MGF failed to setup SO x");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileY(), 9, "MGF failed to setup SO y");

	mgf._deleteSO(mgf.nthSO(0));
	mgf._addSO(1);
	mgf._setupSO(mgf.nthSO(0), 4, 5);

	// Verify
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileX(), 4, "MGF failed to setup SO x");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileY(), 5, "MGF failed to setup SO y");
}


void Project2Test::test_MGFramework_createSOAtLocation()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);
	ASSERT_NOT_EQUAL(mgf.m_Map.occupant(10, 11), 0, "Target tile already occupied");

	// Trigger
	mgf.runConsoleCommand("add so 1 -x 10 -y 11", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 1, "MGF failed to create SO");
	ASSERT_NOT_EQUAL(mgf._m_SO().size(), 1, "MGF failed to create SO");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileX(), 10, "MGF failed to create SO at location");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileY(), 11, "MGF failed to create SO at location");
	ASSERT_NOT_EQUAL(mgf.m_Map.occupant(10, 11), mgf.nthSO(0)->getID(), "MGF failed to occupy tile");
}

void Project2Test::test_MGFramework_sortSOsWhenCreated()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);

	// Trigger
	mgf.runConsoleCommand("add so 1 -x 1 -y 4", &mgf, NULL);
	mgf.runConsoleCommand("add so 1 -x 1 -y 7", &mgf, NULL);
	mgf.runConsoleCommand("add so 1 -x 1 -y 1", &mgf, NULL);
	mgf.runConsoleCommand("add so 1 -x 1 -y 9", &mgf, NULL);
	mgf.runConsoleCommand("add so 1 -x 1 -y 3", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfSO(), 5, "MGF failed to create SO");
	ASSERT_NOT_EQUAL(mgf.nthSO(0)->getTileY(), 1, "MGF failed to sort SO on creation");
	ASSERT_NOT_EQUAL(mgf.nthSO(1)->getTileY(), 3, "MGF failed to sort SO on creation");
	ASSERT_NOT_EQUAL(mgf.nthSO(2)->getTileY(), 4, "MGF failed to sort SO on creation");
	ASSERT_NOT_EQUAL(mgf.nthSO(3)->getTileY(), 7, "MGF failed to sort SO on creation");
	ASSERT_NOT_EQUAL(mgf.nthSO(4)->getTileY(), 9, "MGF failed to sort SO on creation");
}