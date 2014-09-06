#include "../mgclasstester.h"

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
	mgf._deleteMO(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 14, "MGF failed to delete MOs");
	mgf._deleteMO(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 13, "MGF failed to delete MOs");
	mgf._deleteMO(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 12, "MGF failed to delete MOs");

	// Delete last MO
	mgf._deleteMO(11);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 11, "MGF failed to delete MOs");

	// Delete arbitrary MO
	mgf._deleteMO(7);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 10, "MGF failed to delete MOs");

	// Delete MO outside container
	mgf._deleteMO(17);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 10, "MGF failed to delete zero MOs");

	// Delete all MO
	mgf._deleteAllMO();
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to delete MOs");

	// Delete all MO when no MO exist
	mgf._deleteAllMO();
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to run deleteAllMO");
}