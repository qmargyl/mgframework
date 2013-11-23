#ifndef _MG_HTMLLOG_H
#define _MG_HTMLLOG_H

#include "mgcomponent.h"


class MGHTMLLog :public MGComponent
{
	private:
			
	public:

		MGHTMLLog();
		~MGHTMLLog();

		bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s){ return true; }
		eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec){ return MGComponent_UNDEFINED; }

};


#endif

