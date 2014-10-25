#ifndef _MG_SYMBOLTABLE_H
#define _MG_SYMBOLTABLE_H

#include "mgcomponent.h"
#include <deque>
#include <string>
#include <vector>

class MGSymbolTable :public MGComponent
{

protected:
	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) const;

public:

	class MGSymbolTablePair
	{
	public:
		MGSymbolTablePair(const std::string &s, int v)
		{
			symbol=s;
			value=v;
		}
		std::string symbol;
		int value;
	};

	MGSymbolTable();
	~MGSymbolTable();
	void addSymbol(const std::string &s, int v);
	bool hasValue(const std::string &s);
	void setValue(const std::string &s, int v);
	int getValue(const std::string &s);
	void clear();
	void printTable();

	bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);

	std::deque<MGSymbolTablePair> table; //TODO: Should this be private?
};

#endif