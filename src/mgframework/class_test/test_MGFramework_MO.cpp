#include "../mgclasstester.h"
#include "../mgpathitem.h"
#include "../stubs/mgframeworkstub.h"


void MGClassTester::test_MGFramework_createMO()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF created with MOs");
	mgf._addMO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF created MOs without initializing the map");
	mgf.init(16, 16, 32, 32);

	// Trigger/Verify
	mgf._addMO(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to create zero MOs");
	mgf._addMO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 1, "MGF failed to create MOs");
	mgf._addMO(3);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 4, "MGF failed to create MOs");
	mgf._addMO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 5, "MGF failed to create MOs");
	mgf._addMO(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 20, "MGF failed to create MOs");
	mgf._addMO(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 20, "MGF failed to create zero MOs");
}

void MGClassTester::test_MGFramework_deleteMO()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF created with MOs");
	mgf.init(16, 16, 32, 32);
	mgf._addMO(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 15, "MGF failed to create MOs");

	// Trigger/Verify

	// Delete first MO
	mgf._deleteMO(mgf.nthMO(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 14, "MGF failed to delete MOs");
	mgf._deleteMO(mgf.nthMO(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 13, "MGF failed to delete MOs");
	mgf._deleteMO(mgf.nthMO(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 12, "MGF failed to delete MOs");

	// Delete last MO
	mgf._deleteMO(mgf.nthMO(11));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 11, "MGF failed to delete MOs");

	// Delete arbitrary MO
	mgf._deleteMO(mgf.nthMO(7));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 10, "MGF failed to delete MOs");

	// Delete MO outside container
	mgf._deleteMO(mgf.nthMO(17));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 10, "MGF failed to delete zero MOs");

	// Delete all MO
	mgf._deleteAllMO();
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to delete MOs");

	// Delete all MO when no MO exist
	mgf._deleteAllMO();
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to run deleteAllMO");
}

void MGClassTester::test_MGFramework_setupMO()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);
	mgf._addMO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size() == 0, false, "MGF failed to create MO");

	// Trigger
	mgf._setupMO(mgf.nthMO(0), 4, 5, 2, 3, 0, 0, 0, 0);

	// Verify
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileX(), 4, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileY(), 5, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 2, "MGF failed to setup MO owner");
}

void MGClassTester::test_MGFramework_resetupMO()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);
	mgf._addMO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size() == 0, false, "MGF failed to create MO");

	mgf._setupMO(mgf.nthMO(0), 4, 5, 2, 3, 0, 0, 0, 0);
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileX(), 4, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileY(), 5, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 2, "MGF failed to setup MO owner");

	// Trigger
	mgf._setupMO(mgf.nthMO(0), 8, 9, 10, 3, 0, 0, 0, 0);
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileX(), 8, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileY(), 9, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 10, "MGF failed to setup MO owner");

	mgf._deleteMO(mgf.nthMO(0));
	mgf._addMO(1);
	mgf._setupMO(mgf.nthMO(0), 4, 5, 2, 3, 0, 0, 0, 0);

	// Verify
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileX(), 4, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileY(), 5, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 2, "MGF failed to setup MO owner");
}

void MGClassTester::test_MGFramework_setupDeleteSetupMO()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);
	mgf._addMO(5);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 5, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size() == 0, false, "MGF failed to create MO");

	mgf._setupMO(mgf.nthMO(0), 1, 1, 1, 3, 0, 0, 0, 0);
	mgf._setupMO(mgf.nthMO(1), 2, 2, 2, 3, 0, 0, 0, 0);
	mgf._setupMO(mgf.nthMO(2), 3, 3, 3, 3, 0, 0, 0, 0);
	mgf._setupMO(mgf.nthMO(3), 4, 4, 4, 3, 0, 0, 0, 0);
	mgf._setupMO(mgf.nthMO(4), 5, 5, 5, 3, 0, 0, 0, 0);

	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileX(), 1, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileY(), 1, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");

	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getTileX(), 2, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getTileY(), 2, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 2, "MGF failed to setup MO owner");

	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getTileX(), 3, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getTileY(), 3, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getOwner(), 3, "MGF failed to setup MO owner");

	ASSERT_NOT_EQUAL(mgf.nthMO(3)->getTileX(), 4, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(3)->getTileY(), 4, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(3)->getOwner(), 4, "MGF failed to setup MO owner");

	ASSERT_NOT_EQUAL(mgf.nthMO(4)->getTileX(), 5, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(4)->getTileY(), 5, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(4)->getOwner(), 5, "MGF failed to setup MO owner");

	// Trigger
	mgf._deleteMO(mgf.nthMO(2));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 4, "MGF failed to delete MO");
	mgf._addMO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 5, "MGF failed to create MO");
	mgf._setupMO(mgf.nthMO(4), 7, 7, 7, 3, 0, 0, 0, 0);

	// Verify
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileX(), 1, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getTileY(), 1, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");

	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getTileX(), 2, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getTileY(), 2, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 2, "MGF failed to setup MO owner");

	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getTileX(), 4, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getTileY(), 4, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getOwner(), 4, "MGF failed to setup MO owner");

	ASSERT_NOT_EQUAL(mgf.nthMO(3)->getTileX(), 5, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(3)->getTileY(), 5, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(3)->getOwner(), 5, "MGF failed to setup MO owner");

	ASSERT_NOT_EQUAL(mgf.nthMO(4)->getTileX(), 7, "MGF failed to setup MO x");
	ASSERT_NOT_EQUAL(mgf.nthMO(4)->getTileY(), 7, "MGF failed to setup MO y");
	ASSERT_NOT_EQUAL(mgf.nthMO(4)->getOwner(), 7, "MGF failed to setup MO owner");
}