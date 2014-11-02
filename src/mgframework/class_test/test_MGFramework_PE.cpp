#include "../../project2_test.h"
#include "../stubs/mgframeworkstub.h"


void Project2Test::test_MGFramework_createPE()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 0, "MGF created with PE");

	// Trigger/Verify
	mgf._addPE(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 0, "MGF failed to create zero PE");
	mgf._addPE(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 1, "MGF failed to create PE");
	mgf._addPE(3);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 4, "MGF failed to create PE");
	mgf._addPE(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 5, "MGF failed to create PE");
	mgf._addPE(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 20, "MGF failed to create PE");
	mgf._addPE(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 20, "MGF failed to create zero PE");
}

void Project2Test::test_MGFramework_deletePE()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 0, "MGF created with PE");
	mgf._addPE(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 15, "MGF failed to create PE");

	// Trigger/Verify

	// Delete first PE
	mgf._deletePE(mgf.nthPE(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 14, "MGF failed to delete PE");
	mgf._deletePE(mgf.nthPE(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 13, "MGF failed to delete PE");
	mgf._deletePE(mgf.nthPE(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 12, "MGF failed to delete PE");

	// Delete last PE
	mgf._deletePE(mgf.nthPE(11));
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 11, "MGF failed to delete PE");

	// Delete arbitrary PE
	mgf._deletePE(mgf.nthPE(7));
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 10, "MGF failed to delete PE");

	// Delete PE outside container
	mgf._deletePE(mgf.nthPE(17));
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 10, "MGF failed to delete zero PE");

	// Delete all PE
	mgf._deleteAllPE();
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 0, "MGF failed to delete PE");

	// Delete all PE when no PE exist
	mgf._deleteAllPE();
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 0, "MGF failed to run deleteAllPE");
}

void Project2Test::test_MGFramework_deleteFirstPE()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 0, "MGF created with PE");
	mgf._addPE(2);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 2, "MGF failed to create PE");

	std::list<MGPeriodicEvent>::iterator it = mgf.nthPE(0);
	mgf.nthPE(0)->setOwner(0);
	mgf.nthPE(1)->setOwner(1);

	// Trigger - delete first PE
	mgf._deletePE(it);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 1, "MGF failed to delete PE");
	ASSERT_NOT_EQUAL(mgf.nthPE(0)->getOwner(), 1, "MGF did not delete the right PE");

	// Setup - create another 3 PE
	it = mgf.nthPE(0);
	mgf._addPE(3);
	mgf.nthPE(0)->setOwner(0);
	mgf.nthPE(1)->setOwner(1);
	mgf.nthPE(2)->setOwner(2);
	mgf.nthPE(3)->setOwner(3);

	// Trigger - delete first PE again
	mgf._deletePE(it);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 3, "MGF failed to delete PE");
	ASSERT_NOT_EQUAL(mgf.nthPE(0)->getOwner(), 1, "MGF did not delete the right PE");
	ASSERT_NOT_EQUAL(mgf.nthPE(1)->getOwner(), 2, "MGF did not delete the right PE");
	ASSERT_NOT_EQUAL(mgf.nthPE(2)->getOwner(), 3, "MGF did not delete the right PE");
}

void Project2Test::test_MGFramework_deleteFewPE()
{
	// Setup
	MGFrameworkStub mgf;

	mgf.runConsoleCommand("add pe 4", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 4, "MGF failed to create PE");
	ASSERT_NOT_EQUAL(mgf._m_PE().size(), 4, "MGF failed to create PE");
	mgf.nthPE(0)->setOwner(1);
	mgf.nthPE(1)->setOwner(2);
	mgf.nthPE(2)->setOwner(2);
	mgf.nthPE(3)->setOwner(3);
	ASSERT_NOT_EQUAL(mgf.nthPE(0)->getOwner(), 1, "MGF failed to set PE owner");
	ASSERT_NOT_EQUAL(mgf.nthPE(1)->getOwner(), 2, "MGF failed to set PE owner");
	ASSERT_NOT_EQUAL(mgf.nthPE(2)->getOwner(), 2, "MGF failed to set PE owner");
	ASSERT_NOT_EQUAL(mgf.nthPE(3)->getOwner(), 3, "MGF failed to set PE owner");

	// Trigger - delete the two middle PE out of the four
	mgf._deletePE(mgf.nthPE(1));
	mgf._deletePE(mgf.nthPE(1));

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 2, "MGF failed to delete PE");
	ASSERT_NOT_EQUAL(mgf._m_PE().size(), 2, "MGF failed to delete PE");
	ASSERT_NOT_EQUAL(mgf.nthPE(0)->getOwner(), 1, "MGF failed to set PE owner");
	ASSERT_NOT_EQUAL(mgf.nthPE(1)->getOwner(), 3, "MGF failed to set PE owner");
}

void Project2Test::test_MGFramework_deleteAllPE()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 0, "MGF created with PE");
	mgf._addPE(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 15, "MGF failed to create PE");

	// Trigger
	mgf.runConsoleCommand("delete all pe", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 0, "MGF failed to delete all PE");

	// Delete all PE when no PE exist
	mgf.runConsoleCommand("delete all pe", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfPE(), 0, "MGF failed to delete all PE with empty PE list");
}