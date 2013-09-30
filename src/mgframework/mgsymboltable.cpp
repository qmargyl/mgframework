
#include "mgsymboltable.h"
#include "mgframework.h"

MGSymbolTable::MGSymbolTable()
{

}
	
MGSymbolTable::~MGSymbolTable()
{

}

void MGSymbolTable::addSymbol(const std::string &s, int v)
{

}

int MGSymbolTable::lookup(const std::string &s)
{
	return 0;
}

void MGSymbolTable::delSymbol(const std::string &s)
{

}

void MGSymbolTable::printTable()
{

}

eMGComponentConsoleCommand MGSymbolTable::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	return MGComponent_UNDEFINED;
}

bool MGSymbolTable::runConsoleCommand(const char *c, MGFramework *w)
{
	return true;
}