#ifndef _MGCOMPONENTIMPL_H
#define _MGCOMPONENTIMPL_H

#include "../mgcomponent.h"
#include "../mgframework.h"

class MGComponentImpl : public MGComponent
{
	private:

		eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) const
		{
			return MGComponent_UNDEFINED;
		}

	public:

		bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s){ return false; }
};

#endif
