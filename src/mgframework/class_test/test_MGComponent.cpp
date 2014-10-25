#include "../../project2_test.h"
#include "../stubs/mgcomponentimpl.h"

void Project2Test::test_MGComponent_generateID()
{
	MGComponentImpl comp1;
	MGComponentImpl comp2;

	// Test that two consecutively created components have consecutive IDs. 
	ASSERT_NOT_EQUAL(comp1.getID() + 1, comp2.getID(), "MGComponent ID not increased properly");
}

void Project2Test::test_MGComponent_enableDisableLogging()
{
	MGComponentImpl comp1;

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
}

void Project2Test::test_MGComponent_setOwner()
{
	MGComponentImpl comp1;

	// Owner
	ASSERT_NOT_EQUAL(comp1.isOwned(), false, "MGComponent not created without owner");
	comp1.setOwner(4);
	ASSERT_NOT_EQUAL(comp1.isOwned(), true, "MGComponent not able to set owner");
	ASSERT_NOT_EQUAL(comp1.getOwner(), 4, "MGComponent not able to set owner");
	comp1.setOwner(0);
	ASSERT_NOT_EQUAL(comp1.isOwned(), false, "MGComponent not able to unset owner");
	ASSERT_NOT_EQUAL(comp1.getOwner(), 0, "MGComponent not able to unset owner");
}

void Project2Test::test_MGComponent_toStringBool()
{
	MGComponentImpl comp1;

	// toString(bool)
	ASSERT_NOT_EQUAL(MGComponent::toString(true), std::string("true"), "toString(bool) returned incorrectly");
	ASSERT_NOT_EQUAL(MGComponent::toString(false), std::string("false"), "toString(bool) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString(true), std::string("true"), "toString(bool) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString(false), std::string("false"), "toString(bool) returned incorrectly");
}

void Project2Test::test_MGComponent_toStringInt()
{
	MGComponentImpl comp1;

	// toString(int)
	ASSERT_NOT_EQUAL(MGComponent::toString((int)17), std::string("17"), "toString(int) returned incorrectly");
	ASSERT_NOT_EQUAL(MGComponent::toString((int)-17), std::string("-17"), "toString(int) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString((int)17), std::string("17"), "toString(int) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString((int)-17), std::string("-17"), "toString(int) returned incorrectly");
}

void Project2Test::test_MGComponent_toStringUInt()
{
	MGComponentImpl comp1;

	// toString(int)
	ASSERT_NOT_EQUAL(MGComponent::toString((unsigned int)17), std::string("17"), "toString(unsigned int) returned incorrectly");
	ASSERT_NOT_EQUAL(MGComponent::toString((unsigned int)666), std::string("666"), "toString(unsigned int) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString((unsigned int)17), std::string("17"), "toString(unsigned int) returned incorrectly");
	ASSERT_NOT_EQUAL(comp1.toString((unsigned int)666), std::string("666"), "toString(unsigned int) returned incorrectly");
}

void Project2Test::test_MGComponent_oneOf()
{
	ASSERT_NOT_EQUAL(MGComponent::oneOf(1, 1, 2), true, "oneOf returned incorrectly");
	ASSERT_NOT_EQUAL(MGComponent::oneOf(1, 2, 1), true, "oneOf returned incorrectly");
	ASSERT_NOT_EQUAL(MGComponent::oneOf(1, 1, 1), true, "oneOf returned incorrectly");
	ASSERT_NOT_EQUAL(MGComponent::oneOf(1, 2, 2), false, "oneOf returned incorrectly");
}

void Project2Test::test_MGComponent_symbols()
{
	MGComponentImpl c;

	ASSERT_NOT_EQUAL(c.symbols("").size(), 0, "Wrong number of symbols");
	ASSERT_NOT_EQUAL(c.symbols("1+2").size(), 3, "Wrong number of symbols");
	ASSERT_NOT_EQUAL(c.symbols("a-b").size(), 3, "Wrong number of symbols");
	ASSERT_NOT_EQUAL(c.symbols("x=f(x)").size(), 6, "Wrong number of symbols");
	ASSERT_NOT_EQUAL(c.symbols("q==1").size(), 3, "Wrong number of symbols");
	ASSERT_NOT_EQUAL(c.symbols("(u)").size(), 3, "Wrong number of symbols");
}