#ifndef _MG_CLASSTESTER_H
#define _MG_CLASSTESTER_H

#include <iostream>
#include <string>

#define MGCLASSTESTER_LOGLINE_MAXLENGTH 1024

#define ASSERT_NOT_EQUAL(x, y, msg)	do{ \
		if((x) != (y)){ \
			std::cout << "[ASSERT] " << __FILE__ << ":" << __LINE__ << " MSG: " << msg << ", " << (x) << ", " << (y) << std::endl; \
			return; \
		} \
	} while(0)

#ifndef UNITTEST_LINUX
  #define RUNTEST(f) do{ \
		std::cout << "[TC] " << #f << std::endl; \
		f ## (); \
	} while(0)
#else
  #define RUNTEST(f) do{ \
		std::cout << "[TC] " << #f << std::endl; \
		f(); \
	} while(0)
#endif

#define FINISH_TESTSUITE() do{ \
		std::cout << "Exiting application..." << std::endl; \
	} while(0)



class MGClassTester
{
private:
	// test_MGComponent
	static void test_MGComponent();

	// test_MGMap_initialize
	static void test_MGMap_initialize();

	// test_MGAstarNode
	static void test_MGAStarNode();

	// test_MGSymbolTable
	static void test_MGSymbolTable_addSymbol();
	static void test_MGSymbolTable_setValue();
	static void test_MGSymbolTable_clear();
	static void test_MGSymbolTable_addSeveralSymbols();
	static void test_MGSymbolTable_symbolNames();

	// test_MGPathGenerator_calculatePathAStar
	static void test_MGPathGenerator_calculatePathAStar();

	// test_MGFramework_misc
	static void test_MGFramework_runOneFrame();

	// test_MGFramework_MO
	static void test_MGFramework_createMO();
	static void test_MGFramework_deleteMO();
	static void test_MGFramework_deleteMOVerifyIterator();
	static void test_MGFramework_deleteFewMO();
	static void test_MGFramework_deleteAllMO();
	static void test_MGFramework_setupMO();
	static void test_MGFramework_resetupMO();
	static void test_MGFramework_setupDeleteSetupMO();
	static void test_MGFramework_deletePerOwnerOfMO();
	static void test_MGFramework_deleteFirstMOPerOwner();
	static void test_MGFramework_markMO();
	static void test_MGFramework_markMOIndexInSymbolTable();

	// test_system_test_002_basic_MO
	static void test_002_basic_MO_create_delete();
	static void test_002_basic_MO_create_delete_with_owner();
	static void test_002_basic_MO_create_with_area_square();
	static void test_002_basic_MO_create_with_area_square_single_location();
	static void test_002_basic_MO_mark_unmark();
	static void test_002_basic_MO_verify_location();
	static void test_002_basic_MO_ordering();
	static void test_002_basic_MO_random_mo();
	static void test_002_basic_MO_stressing();

public:
	static void runAll()
	{
		// test_MGComponent
		RUNTEST(test_MGComponent);

		// test_MGMap_initialize
		RUNTEST(test_MGMap_initialize);

		// test_MGAstarNode
		RUNTEST(test_MGAStarNode);

		// test_MGSymbolTable
		RUNTEST(test_MGSymbolTable_addSymbol);
		RUNTEST(test_MGSymbolTable_setValue);
		RUNTEST(test_MGSymbolTable_clear);
		RUNTEST(test_MGSymbolTable_addSeveralSymbols);
		RUNTEST(test_MGSymbolTable_symbolNames);

		// test_MGPathGenerator_calculatePathAStar
		RUNTEST(test_MGPathGenerator_calculatePathAStar);

		// test_MGFramework_misc
		RUNTEST(test_MGFramework_runOneFrame);

		// test_MGFramework_MO
		RUNTEST(test_MGFramework_createMO);
		RUNTEST(test_MGFramework_deleteMO);
		RUNTEST(test_MGFramework_deleteMOVerifyIterator);
		RUNTEST(test_MGFramework_deleteFewMO);
		RUNTEST(test_MGFramework_deleteAllMO);
		RUNTEST(test_MGFramework_setupMO);
		RUNTEST(test_MGFramework_resetupMO);
		RUNTEST(test_MGFramework_setupDeleteSetupMO);
		RUNTEST(test_MGFramework_deletePerOwnerOfMO);
		RUNTEST(test_MGFramework_deleteFirstMOPerOwner);
		RUNTEST(test_MGFramework_markMO);
		RUNTEST(test_MGFramework_markMOIndexInSymbolTable);

		// test_system_test_002_basic_MO
		RUNTEST(test_002_basic_MO_create_delete);
		RUNTEST(test_002_basic_MO_create_delete_with_owner);
		RUNTEST(test_002_basic_MO_create_with_area_square);
		RUNTEST(test_002_basic_MO_create_with_area_square_single_location);
		RUNTEST(test_002_basic_MO_mark_unmark);
		RUNTEST(test_002_basic_MO_verify_location);
		RUNTEST(test_002_basic_MO_ordering);
		RUNTEST(test_002_basic_MO_random_mo);
		RUNTEST(test_002_basic_MO_stressing);

		FINISH_TESTSUITE();
	}

	// Log filtering/comparing/evaluation
	static void logEval(std::string logFileName, bool negativeTest);
	static std::string filterLine(const char* line);
	static void logFilter(std::string logFileName);
	static void logCompare(std::string logFileName1, std::string logFileName2);
};

#endif
