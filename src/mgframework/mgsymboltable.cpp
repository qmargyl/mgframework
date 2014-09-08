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
	table.push_back(MGSymbolTablePair(s, v));
}

bool MGSymbolTable::hasValue(const std::string &s)
{
	for (std::deque<MGSymbolTablePair>::iterator it=table.begin(); it != table.end(); ++it)
	{
		if((*it).symbol == s)
		{
			return true;
		}
	}
	return false;
}

void MGSymbolTable::setValue(const std::string &s, int v)
{
	for (std::deque<MGSymbolTablePair>::iterator it=table.begin(); it != table.end(); ++it)
	{
		if((*it).symbol == s)
		{
			(*it).value = v;
			return;
		}
	}
	MGFLOG_ERROR("Cannot find symbol '" << s.c_str() << "'");
}

int MGSymbolTable::getValue(const std::string &s)
{
	for (std::deque<MGSymbolTablePair>::iterator it=table.begin(); it != table.end(); ++it)
	{
		if((*it).symbol == s)
		{
			return (*it).value;
		}
	}
	MGFLOG_ERROR("Cannot find symbol '" << s.c_str() << "'");
	return 0;
}

void MGSymbolTable::clear()
{
	table.clear();
}

void MGSymbolTable::printTable()
{
	std::cout << "Symbol Table" << std::endl;
	for (std::deque<MGSymbolTablePair>::iterator it=table.begin(); it != table.end(); ++it)
	{
		std::cout << "'" << ((*it).symbol).c_str() << "' : " << (*it).value << std::endl;
	}
}

eMGComponentConsoleCommand MGSymbolTable::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	return MGComponent_UNDEFINED;
}

bool MGSymbolTable::runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s)
{
	return true;
}