#include "../mgclasstester.h"
#include "../stubs/mgcomponentimpl.h"

void MGClassTester::test_MGComponent()
{
	MGComponentImpl comp1;
	MGComponentImpl comp2;

	// Test that two consecutively created components have consecutive IDs. 
	ASSERT_NOT_EQUAL(comp1.getID() + 1, comp2.getID(), "MGComponent ID not increased properly");

}