#ifndef _MG_PATHITEM_H
#define _MG_PATHITEM_H

enum eMGFPathType{
	MGFSTRAIGHTLINE = 0,
	MGFSKYPATH = 1,
	MGFGROUNDPATH = 2,
	MGFBASICPATH1 = 3,
	MGFASTARLIST = 4
};

class MGPathItem
{
private:
	MGPathItem(){}
	int m_X;
	int m_Y;
	double m_Heuristic;

public:
	
	MGPathItem(int x, int y);
	MGPathItem(int x, int y, double h);

	~MGPathItem(){}
	int getX() const { return m_X; }
	int getY() const { return m_Y; }
	double getH() const { return m_Heuristic; }
	void setH(double h){ m_Heuristic = h; }
	bool equalCoordinate(MGPathItem* pi) const { return (getX() == pi->getX()) && (getY() == pi->getY()); }
};

#endif