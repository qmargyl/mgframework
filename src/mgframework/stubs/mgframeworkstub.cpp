#include "mgframeworkstub.h"

MGFrameworkStub::MGFrameworkStub()
{

}

bool MGFrameworkStub::init(int w, int h, int tw, int th)
{
	width = w;
	height = h;
	tileWidth = tw;
	tileHeight = th;
	return true;
}