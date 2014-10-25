#include "mgpathitem.h"
#include "mgastarnode.h"



MGPathItem::MGPathItem(int x, int y)
{
	m_X = x;
	m_Y = y;
	m_Heuristic = 1;
}

MGPathItem::MGPathItem(int x, int y, double h)
{
	m_X = x;
	m_Y = y;
	m_Heuristic = h;
}