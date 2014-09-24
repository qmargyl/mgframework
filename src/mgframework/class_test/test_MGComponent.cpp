#include "../../project2_test.h"
#include "../stubs/mgcomponentimpl.h"

void Project2Test::test_MGComponent()
{
	MGComponentImpl comp1;
	MGComponentImpl comp2;

	// Test that two consecutively created components have consecutive IDs. 
	ASSERT_NOT_EQUAL(comp1.getID() + 1, comp2.getID(), "MGComponent ID not increased properly");
	
	// Logging ON/OFF
	ASSERT_NOT_EQUAL(comp1.loggingEnabled(), false, "MGComponent not created with disabled logging");
	comp1.enableLogging();
	ASSERT_NOT_EQUAL(comp1.loggingEnabled(), true, "MGComponent not able to enable logging");
	comp1.disableLogging();
	ASSERT_NOT_EQUAL(comp1.loggingEnabled(), false, "MGComponent not able to disable logging");
	comp1.enableLogging();
	ASSERT_NOT_EQUAL(comp1.loggingEnabled(), true, "MGComponent not able to enable logging");
	comp1.disableLogging();
	ASSERT_NOT_EQUAL(comp1.loggingEnabled(), false, "MGComponent not able to disable logging");

	// Owner
	ASSERT_NOT_EQUAL(comp1.isOwned(), false, "MGComponent not created without owner");
	comp1.setOwner(4);
	ASSERT_NOT_EQUAL(comp1.isOwned(), true, "MGComponent not able to set owner");
	ASSERT_NOT_EQUAL(comp1.getOwner(), 4, "MGComponent not able to set owner");
	comp1.setOwner(0);
	ASSERT_NOT_EQUAL(comp1.isOwned(), false, "MGComponent not able to unset owner");
	ASSERT_NOT_EQUAL(comp1.getOwner(), 0, "MGComponent not able to unset owner");
	
	// toString(bool)
	ASSERT_NOT_EQUAL(MGComponent::toString(true), std::string("true"), "toString(bool) returned incorrectly");
	ASSERT_NOT_EQUAL(MGComponent::toString(false), std::string("false"), "toString(bool) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString(true), std::string("true"), "toString(bool) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString(false), std::string("false"), "toString(bool) returned incorrectly");
	
	// toString(int)
	ASSERT_NOT_EQUAL(MGComponent::toString(17), std::string("17"), "toString(int) returned incorrectly");
	ASSERT_NOT_EQUAL(MGComponent::toString(-17), std::string("-17"), "toString(int) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString(17), std::string("17"), "toString(int) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString(-17), std::string("-17"), "toString(int) returned incorrectly");
}