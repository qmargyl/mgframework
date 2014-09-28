#ifndef _MG_PATHGENERATOR_H
#define _MG_PATHGENERATOR_H

#include "mgcomponent.h"
#include "mgframework.h"
#include "mgastarnode.h"

#ifndef sqrt_2
#define sqrt_2 (1.414213562373095)
#endif


class MGPathGenerator : public MGComponent
{

public:

	// Calculate path from @(x1,y1) to @(x2,y2) on @map and store it in @path
	static void calculatePathAStar(int x1, int y1, int x2, int y2, MGMap &map, std::list<MGPathItem> &path);
	static void calculatePathBasic(int x1, int y1, int x2, int y2, MGMap &map, std::list<MGPathItem> &path);


	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec){ return MGComponent_UNDEFINED; }
	bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s){ return false; }

};





#endif