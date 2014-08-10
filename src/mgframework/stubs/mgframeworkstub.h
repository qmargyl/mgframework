#ifndef _MGFRAMEWORKSTUB_H
#define _MGFRAMEWORKSTUB_H

#include "../mgframework.h"

class MGFrameworkStub : public MGFramework
{
	private:
		void handleGameLogics(){}
		void draw(){}

	public:
		MGFrameworkStub();
		bool init(int w, int h, int tw, int th);

		int width;
		int height;
		int tileWidth;
		int tileHeight;
};

#endif
