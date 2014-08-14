#ifndef _MG_PATHITEM_H
#define _MG_PATHITEM_H

//class MGAStarNode;
//#include "mgpathgenerator.h"

class MGPathItem
{
private:
	int m_X;
	int m_Y;
	double m_Heuristic;

public:
	MGPathItem(){}
	MGPathItem(int x, int y);
	MGPathItem(int x, int y, double h);
/*
	MGPathItem(MGAStarNode node);
*/
	~MGPathItem(){}
	int getX(){ return m_X; }
	int getY(){ return m_Y; }
	double getH(){ return m_Heuristic; }
	void setH(double h){ m_Heuristic = h; }
	void setPI(int x, int y, int h){ m_X = x; m_Y = y; m_Heuristic = h; }
	bool equalCoordinate(MGPathItem* pi){ return (getX() == pi->getX()) && (getY() == pi->getY()); }
};

#endif