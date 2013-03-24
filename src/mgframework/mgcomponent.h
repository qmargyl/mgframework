#ifndef _MG_COMPONENT_H
#define _MG_COMPONENT_H

class MGComponent
{
public:
	virtual bool runConsoleCommand(const char *c) = 0;
};

#endif