#include "mgframeworkstub.h"
#include "../mgpathitem.h"

bool MGFrameworkStub::init(int w, int h, int tw, int th)
{
	m_Map.init(w, h, tw, th, 1024, 768);
	return true;
}