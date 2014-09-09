#include "../mgclasstester.h"

#include "../mgsymboltable.h"

void MGClassTester::test_MGSymbolTable_addSymbol()
{
	// Setup
	MGSymbolTable s;
	ASSERT_NOT_EQUAL(s.hasValue("s1"), false, "MGSymbolTable contains test symbol before adding it");

	// Trigger
	s.addSymbol("s1", 17);

	// Verify
	ASSERT_NOT_EQUAL(s.hasValue("s1"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.getValue("s1"), 17, "MGSymbolTable failed to give symbol correct value");
}

void MGClassTester::test_MGSymbolTable_setValue()
{
	// Setup
	MGSymbolTable s;
	ASSERT_NOT_EQUAL(s.hasValue("s1"), false, "MGSymbolTable contains test symbol before adding it");
	s.addSymbol("s1", 17);
	ASSERT_NOT_EQUAL(s.hasValue("s1"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.getValue("s1"), 17, "MGSymbolTable failed to give symbol correct value");
	
	// Trigger
	s.setValue("s1", 18);
	
	// Verify
	ASSERT_NOT_EQUAL(s.hasValue("s1"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.getValue("s1"), 18, "MGSymbolTable failed to give symbol correct value");
}

void MGClassTester::test_MGSymbolTable_clear()
{
	// Setup
	MGSymbolTable s;
	ASSERT_NOT_EQUAL(s.hasValue("s1"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s2"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s3"), false, "MGSymbolTable contains test symbol before adding it");
	s.addSymbol("s1", 17);
	s.addSymbol("s2", 18);
	s.addSymbol("s3", 19);
	ASSERT_NOT_EQUAL(s.hasValue("s1"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s2"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s3"), true, "MGSymbolTable failed to add symbol");

	// Trigger
	s.clear();
	
	// Verify
	ASSERT_NOT_EQUAL(s.hasValue("s1"), false, "MGSymbolTable failed to remove symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s2"), false, "MGSymbolTable failed to remove symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s3"), false, "MGSymbolTable failed to remove symbol");
}

void MGClassTester::test_MGSymbolTable_addManySymbols()
{
	// Setup
	MGSymbolTable s;
	ASSERT_NOT_EQUAL(s.hasValue("s1"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s2"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s3"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s4"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s5"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s6"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s7"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s8"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s9"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s10"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s11"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s12"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s13"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s14"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s15"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s16"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s17"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s18"), false, "MGSymbolTable contains test symbol before adding it");
	ASSERT_NOT_EQUAL(s.hasValue("s19"), false, "MGSymbolTable contains test symbol before adding it");

	s.addSymbol("s1", 1);
	s.addSymbol("s2", 2);	
	s.addSymbol("s3", 3);	
	s.addSymbol("s4", 4);	
	s.addSymbol("s5", 5);
	s.addSymbol("s6", 6);
	s.addSymbol("s7", 7);
	s.addSymbol("s8", 8);
	s.addSymbol("s9", 9);
	s.addSymbol("s10", 20);
	s.addSymbol("s11", 21);
	s.addSymbol("s12", 22);
	s.addSymbol("s13", 23);
	s.addSymbol("s14", 24);
	s.addSymbol("s15", 25);
	s.addSymbol("s16", 26);
	s.addSymbol("s17", 27);
	s.addSymbol("s18", 28);
	s.addSymbol("s19", 29);

	// Verify
	ASSERT_NOT_EQUAL(s.hasValue("s1"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s2"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s3"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s4"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s5"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s6"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s7"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s8"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s9"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s10"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s11"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s12"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s13"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s14"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s15"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s16"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s17"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s18"), true, "MGSymbolTable failed to add symbol");
	ASSERT_NOT_EQUAL(s.hasValue("s19"), true, "MGSymbolTable failed to add symbol");

	ASSERT_NOT_EQUAL(s.getValue("s1"), 1, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s2"), 2, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s3"), 3, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s4"), 4, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s5"), 5, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s6"), 6, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s7"), 7, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s8"), 8, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s9"), 9, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s10"), 20, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s11"), 21, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s12"), 22, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s13"), 23, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s14"), 24, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s15"), 25, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s16"), 26, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s17"), 27, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s18"), 28, "MGSymbolTable failed to set correct value");
	ASSERT_NOT_EQUAL(s.getValue("s19"), 29, "MGSymbolTable failed to set correct value");
}