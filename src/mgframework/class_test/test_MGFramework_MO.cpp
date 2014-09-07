#include "../mgclasstester.h"
#include "../mgpathitem.h"
#include "../mgsymboltable.h"
#include "../stubs/mgframeworkstub.h"


void MGClassTester::test_MGFramework_createMO()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF created with MO");
	mgf._addMO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF created MOs without initializing the map");
	mgf.init(16, 16, 32, 32);

	// Trigger/Verify
	mgf._addMO(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to create zero MO");
	mgf._addMO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 1, "MGF failed to create MO");
	mgf._addMO(3);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 4, "MGF failed to create MO");
	mgf._addMO(1);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 5, "MGF failed to create MO");
	mgf._addMO(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 20, "MGF failed to create MO");
	mgf._addMO(0);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 20, "MGF failed to create zero MO");
}

void MGClassTester::test_MGFramework_deleteMO()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF created with MO");
	mgf.init(16, 16, 32, 32);
	mgf._addMO(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 15, "MGF failed to create MO");

	// Trigger/Verify

	// Delete first MO
	mgf._deleteMO(mgf.nthMO(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 14, "MGF failed to delete MO");
	mgf._deleteMO(mgf.nthMO(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 13, "MGF failed to delete MO");
	mgf._deleteMO(mgf.nthMO(0));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 12, "MGF failed to delete MO");

	// Delete last MO
	mgf._deleteMO(mgf.nthMO(11));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 11, "MGF failed to delete MO");

	// Delete arbitrary MO
	mgf._deleteMO(mgf.nthMO(7));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 10, "MGF failed to delete MO");

	// Delete MO outside container
	mgf._deleteMO(mgf.nthMO(17));
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 10, "MGF failed to delete zero MO");

	// Delete all MO
	mgf._deleteAllMO();
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to delete MO");

	// Delete all MO when no MO exist
	mgf._deleteAllMO();
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to run deleteAllMO");
}

void MGClassTester::test_MGFramework_deleteMOVerifyIterator()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF created with MO");
	mgf.init(16, 16, 32, 32);
	mgf._addMO(2);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 2, "MGF failed to create MO");

	std::list<MGMovingObject>::iterator it = mgf.nthMO(0);

	// Trigger - delete first MO
	mgf._deleteMO(it);

	// Verify
	ASSERT_NOT_EQUAL(mgf.nthMO(0) == it, false, "MGF failed to set iterator correctly when deleting MO");
//	it--;
	ASSERT_NOT_EQUAL(mgf.nthMO(0) == it, true, "MGF failed to set iterator correctly when deleting MO");

	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 1, "MGF failed to delete MO");
}

void MGClassTester::test_MGFramework_deleteFewMO()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);

	mgf.runConsoleCommand("add mo 1 -owner 1", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");

	mgf.runConsoleCommand("add mo 2 -owner 2", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 3, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 3, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 2, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getOwner(), 2, "MGF failed to setup MO owner");

	mgf.runConsoleCommand("add mo 1 -owner 3", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 4, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 4, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 2, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getOwner(), 2, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(3)->getOwner(), 3, "MGF failed to setup MO owner");

	// Trigger - delete the two middle MO out of the four
	mgf._deleteMO(mgf.nthMO(1));
	mgf._deleteMO(mgf.nthMO(1));

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 2, "MGF failed to delete MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 2, "MGF failed to delete MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 3, "MGF failed to setup MO owner");
}

void MGClassTester::test_MGFramework_deleteAllMO()
{
	// Setup
	MGFrameworkStub mgf;
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF created with MO");
	mgf.init(16, 16, 32, 32);
	mgf._addMO(15);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 15, "MGF failed to create MO");

	// Trigger
	mgf.runConsoleCommand("delete all mo", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to delete all MO");

	// Delete all MO when no MO exist
	mgf.runConsoleCommand("delete all mo", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 0, "MGF failed to delete all MO on empty MO list");
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

void MGClassTester::test_MGFramework_deletePerOwnerOfMO()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);

	mgf.runConsoleCommand("add mo 1 -owner 1", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");

	mgf.runConsoleCommand("add mo 2 -owner 2", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 3, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 3, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 2, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getOwner(), 2, "MGF failed to setup MO owner");

	mgf.runConsoleCommand("add mo 1 -owner 3", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 4, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 4, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 2, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getOwner(), 2, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(3)->getOwner(), 3, "MGF failed to setup MO owner");

	// Trigger
	mgf.runConsoleCommand("delete all mo -owner 2", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 2, "MGF failed to delete MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 2, "MGF failed to delete MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 3, "MGF failed to setup MO owner");
}

void MGClassTester::test_MGFramework_deleteFirstMOPerOwner()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);

	mgf.runConsoleCommand("add mo 1 -owner 1", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");

	mgf.runConsoleCommand("add mo 2 -owner 2", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 3, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 3, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 1, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 2, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(2)->getOwner(), 2, "MGF failed to setup MO owner");

	// Trigger
	mgf.runConsoleCommand("delete all mo -owner 1", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 2, "MGF failed to delete MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 2, "MGF failed to delete MO");
	ASSERT_NOT_EQUAL(mgf.nthMO(0)->getOwner(), 2, "MGF failed to setup MO owner");
	ASSERT_NOT_EQUAL(mgf.nthMO(1)->getOwner(), 2, "MGF failed to setup MO owner");
}

void MGClassTester::test_MGFramework_markMO()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);
	mgf.runConsoleCommand("add mo 1", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 1, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.getNumberOfMarkedMO(), 0, "MGF created marked MO");

	// Trigger
	mgf.runConsoleCommand("mo 0 mark", &mgf, NULL);

	// Verify
	ASSERT_NOT_EQUAL(mgf.getNumberOfMarkedMO(), 1, "MGF failed to mark MO");
}

void MGClassTester::test_MGFramework_markMOIndexInSymbolTable()
{
	// Setup
	MGFrameworkStub mgf;
	mgf.init(16, 16, 32, 32);
	mgf.runConsoleCommand("add mo 2", &mgf, NULL);
	ASSERT_NOT_EQUAL(mgf._getNumberOfMO(), 2, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf._m_MO().size(), 2, "MGF failed to create MO");
	ASSERT_NOT_EQUAL(mgf.getNumberOfMarkedMO(), 0, "MGF created marked MO");

	MGSymbolTable s;
	s.addSymbol("moIndex", 1);
	ASSERT_NOT_EQUAL(s.hasValue("moIndex"), true, "MGF failed to create symbol");
	ASSERT_NOT_EQUAL(s.getValue("moIndex"), 1, "MGF failed to create symbol");

	// Trigger
	mgf.runConsoleCommand("mo moIndex mark", &mgf, &s);

	// Verify
	ASSERT_NOT_EQUAL(mgf.getNumberOfMarkedMO(), 1, "MGF failed to mark MO");
}