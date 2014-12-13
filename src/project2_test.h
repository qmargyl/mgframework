#ifndef _PROJECT_TEST_2_H
#define _PROJECT_TEST_2_H

#include "mgframework/mgclasstester.h"

class Project2Test : public MGClassTester
{
private:
	// test_MGPathItem
	static void test_MGPathItem_createPathItemXY();
	static void test_MGPathItem_createPathItemXYH();
	static void test_MGPathItem_setH();
	static void test_MGPathItem_equalCoordinate();

	// test_MGComponent
	static void test_MGComponent_generateID();
	static void test_MGComponent_enableDisableLogging();
	static void test_MGComponent_setOwner();
	static void test_MGComponent_toStringBool();
	static void test_MGComponent_toStringInt();
	static void test_MGComponent_toStringUInt();
	static void test_MGComponent_oneOf();
	static void test_MGComponent_symbols();
	static void test_MGComponent_randomNOfZeroAlwaysReturnsZero();
	static void test_MGComponent_randomNOfOneAlwaysReturnsZero();
	static void test_MGComponent_randomNOfFourAlwaysReturnsWithinLimits();
	
	// test_MGMovingObject
	static void test_MGMovingObject_initialize();
	static void test_MGMovingObject_mark();
	static void test_MGMovingObject_setBasicPathStartMoving();
	static void test_MGMovingObject_setAStarPathStartMoving();
	static void test_MGMovingObject_setAStarPathGetStuck();

	// test_MGMap
	static void test_MGMap_initialize();
	static void test_MGMap_calculateOneStepPath();

	// test_MGAstarNode
	static void test_MGAStarNode();
	
	// test_MGStationaryObject
	static void test_MGStationaryObject_setTileXY();
	static void test_MGStationaryObject_setType();
	static void test_MGStationaryObject_setTileXYWithoutOccupy();
	static void test_MGStationaryObject_setOneTexHandle();

	// test_MGSymbolTable
	static void test_MGSymbolTable_addSymbol();
	static void test_MGSymbolTable_setValue();
	static void test_MGSymbolTable_clear();
	static void test_MGSymbolTable_addSeveralSymbols();
	static void test_MGSymbolTable_symbolNames();

	// test_MGPathGenerator_calculatePathAStar
	static void test_MGPathGenerator_calculatePathAStar();
	static void test_MGPathGenerator_calculatePathAStarAroundObstacle();
	static void test_MGPathGenerator_calculatePathAStarUnreachable();

	// test_MGFramework_misc
	static void test_MGFramework_runOneFrame();
	static void test_MGFramework_fullscreen();
	static void test_MGFramework_toIntFromConstant();
	static void test_MGFramework_toIntFromSymbol();
	static void test_MGFramework_toBoolFromConstant();
	static void test_MGFramework_toBoolFromSymbol();
	static void test_MGFramework_unsetWindowProperties();
	static void test_MGFramework_setPort();
	static void test_MGFramework_enableFeatureMiniMap();
	static void test_MGFramework_countMark();
	static void test_MGFramework_updateFramingBasicCase();

	// test_MGFramework_MO
	static void test_MGFramework_createMO();
	static void test_MGFramework_deleteMO();
	static void test_MGFramework_deleteFirstMO();
	static void test_MGFramework_deleteFewMO();
	static void test_MGFramework_deleteAllMO();
	static void test_MGFramework_setupMO();
	static void test_MGFramework_resetupMO();
	static void test_MGFramework_setupDeleteSetupMO();
	static void test_MGFramework_deletePerOwnerOfMO();
	static void test_MGFramework_deleteFirstMOPerOwner();
	static void test_MGFramework_markMO();
	static void test_MGFramework_markMOIndexInSymbolTable();
	static void test_MGFramework_createMOAtLocation();
	static void test_MGFramework_oneMOStartsToTakeAStepRight();
	static void test_MGFramework_oneMOStartsToTakeAStepLeft();

	// test_MGFramework_SO
	static void test_MGFramework_createSO();
	static void test_MGFramework_deleteSO();
	static void test_MGFramework_deleteFirstSO();
	static void test_MGFramework_deleteFewSO();
	static void test_MGFramework_deleteAllSO();
	static void test_MGFramework_setupSO();
	static void test_MGFramework_resetupSO();
	static void test_MGFramework_createSOAtLocation();
	static void test_MGFramework_sortSOsWhenCreated();
	static void test_MGFramework_setDefaultSOType();
	static void test_MGFramework_setSpecifiedSOType();
	static void test_MGFramework_setSpecifiedSOTypeBasedOnSymbol();
	static void test_MGFramework_fillInSOClusterOf4SO();
	static void test_MGFramework_increaseDensityOfSO();

	// test_MGFramework_PE
	static void test_MGFramework_createPE();
	static void test_MGFramework_deletePE();
	static void test_MGFramework_deleteFirstPE();
	static void test_MGFramework_deleteFewPE();
	static void test_MGFramework_deleteAllPE();

	// test_MGSC
	static void test_MGSC_startServer();

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
	void runAll() const
	{
		// test_MGPathItem
		RUNTEST(test_MGPathItem_createPathItemXY);
		RUNTEST(test_MGPathItem_createPathItemXYH);
		RUNTEST(test_MGPathItem_setH);
		RUNTEST(test_MGPathItem_equalCoordinate);

		// test_MGComponent
		RUNTEST(test_MGComponent_generateID);
		RUNTEST(test_MGComponent_enableDisableLogging);
		RUNTEST(test_MGComponent_setOwner);
		RUNTEST(test_MGComponent_toStringBool);
		RUNTEST(test_MGComponent_toStringInt);
		RUNTEST(test_MGComponent_toStringUInt);
		RUNTEST(test_MGComponent_oneOf);
		RUNTEST(test_MGComponent_symbols);
		RUNTEST(test_MGComponent_randomNOfZeroAlwaysReturnsZero);
		RUNTEST(test_MGComponent_randomNOfOneAlwaysReturnsZero);
		RUNTEST(test_MGComponent_randomNOfFourAlwaysReturnsWithinLimits);

		// test_MGMovingObject
		RUNTEST(test_MGMovingObject_initialize);
		RUNTEST(test_MGMovingObject_mark);
		RUNTEST(test_MGMovingObject_setBasicPathStartMoving);
		RUNTEST(test_MGMovingObject_setAStarPathStartMoving);
		RUNTEST(test_MGMovingObject_setAStarPathGetStuck);
	
		// test_MGMap
		RUNTEST(test_MGMap_initialize);
		RUNTEST(test_MGMap_calculateOneStepPath);

		// test_MGAstarNode
		RUNTEST(test_MGAStarNode);
		
		// test_MGStationaryObject
		RUNTEST(test_MGStationaryObject_setTileXY);
		RUNTEST(test_MGStationaryObject_setType);
		RUNTEST(test_MGStationaryObject_setTileXYWithoutOccupy);
		RUNTEST(test_MGStationaryObject_setOneTexHandle);

		// test_MGSymbolTable
		RUNTEST(test_MGSymbolTable_addSymbol);
		RUNTEST(test_MGSymbolTable_setValue);
		RUNTEST(test_MGSymbolTable_clear);
		RUNTEST(test_MGSymbolTable_addSeveralSymbols);
		RUNTEST(test_MGSymbolTable_symbolNames);

		// test_MGPathGenerator_calculatePathAStar
		RUNTEST(test_MGPathGenerator_calculatePathAStar);
		RUNTEST(test_MGPathGenerator_calculatePathAStarAroundObstacle);
		RUNTEST(test_MGPathGenerator_calculatePathAStarUnreachable);

		// test_MGFramework_misc
		RUNTEST(test_MGFramework_runOneFrame);
		RUNTEST(test_MGFramework_fullscreen);
		RUNTEST(test_MGFramework_toIntFromConstant);
		RUNTEST(test_MGFramework_toIntFromSymbol);
		RUNTEST(test_MGFramework_toBoolFromConstant);
		RUNTEST(test_MGFramework_toBoolFromSymbol);
		RUNTEST(test_MGFramework_unsetWindowProperties);
		RUNTEST(test_MGFramework_setPort);
		RUNTEST(test_MGFramework_enableFeatureMiniMap);
		RUNTEST(test_MGFramework_countMark);
		RUNTEST(test_MGFramework_updateFramingBasicCase);

		// test_MGFramework_MO
		RUNTEST(test_MGFramework_createMO);
		RUNTEST(test_MGFramework_deleteMO);
		RUNTEST(test_MGFramework_deleteFirstMO);
		RUNTEST(test_MGFramework_deleteFewMO);
		RUNTEST(test_MGFramework_deleteAllMO);
		RUNTEST(test_MGFramework_setupMO);
		RUNTEST(test_MGFramework_resetupMO);
		RUNTEST(test_MGFramework_setupDeleteSetupMO);
#ifndef UNITTEST_LINUX
		// TODO: Investigate segmentation fault in Linux
		RUNTEST(test_MGFramework_deletePerOwnerOfMO);
#endif
		RUNTEST(test_MGFramework_deleteFirstMOPerOwner);
		RUNTEST(test_MGFramework_markMO);
		RUNTEST(test_MGFramework_markMOIndexInSymbolTable);
		RUNTEST(test_MGFramework_createMOAtLocation);
		RUNTEST(test_MGFramework_oneMOStartsToTakeAStepRight);
		RUNTEST(test_MGFramework_oneMOStartsToTakeAStepLeft);


		// test_MGFramework_SO
		RUNTEST(test_MGFramework_createSO);
		RUNTEST(test_MGFramework_deleteSO);
		RUNTEST(test_MGFramework_deleteFirstSO);
		RUNTEST(test_MGFramework_deleteFewSO);
		RUNTEST(test_MGFramework_deleteAllSO);
		RUNTEST(test_MGFramework_setupSO);
		RUNTEST(test_MGFramework_resetupSO);
		RUNTEST(test_MGFramework_createSOAtLocation);
		RUNTEST(test_MGFramework_sortSOsWhenCreated);
		RUNTEST(test_MGFramework_setDefaultSOType);
		RUNTEST(test_MGFramework_setSpecifiedSOType);
		RUNTEST(test_MGFramework_setSpecifiedSOTypeBasedOnSymbol);
		RUNTEST(test_MGFramework_fillInSOClusterOf4SO);
		RUNTEST(test_MGFramework_increaseDensityOfSO);

		// test_MGFramework_PE
		RUNTEST(test_MGFramework_createPE);
		RUNTEST(test_MGFramework_deletePE);
		RUNTEST(test_MGFramework_deleteFirstPE);
		RUNTEST(test_MGFramework_deleteFewPE);
		RUNTEST(test_MGFramework_deleteAllPE);

		// test_MGSC
		RUNTEST(test_MGSC_startServer);

		// test_system_test_002_basic_MO
		RUNTEST(test_002_basic_MO_create_delete);
		RUNTEST(test_002_basic_MO_create_delete_with_owner);
		RUNTEST(test_002_basic_MO_create_with_area_square);
		RUNTEST(test_002_basic_MO_create_with_area_square_single_location);
		RUNTEST(test_002_basic_MO_mark_unmark);
		RUNTEST(test_002_basic_MO_verify_location);
		RUNTEST(test_002_basic_MO_ordering);
		RUNTEST(test_002_basic_MO_random_mo);
#ifndef UNITTEST_LINUX
		// TODO: Investigate segmentation fault in Linux
		RUNTEST(test_002_basic_MO_stressing);
#endif

		FINISH_TESTSUITE();
	}
};

#endif