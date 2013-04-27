#ifndef _MG_COMPONENT_H
#define _MG_COMPONENT_H

class MGFramework;

class MGComponent
{
public:
	virtual bool runConsoleCommand(const char *c, MGFramework *w) = 0;
};

#endif