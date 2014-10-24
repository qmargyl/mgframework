#ifndef _MG_STATIONARYOBJECT_H
#define _MG_STATIONARYOBJECT_H
#include "mgcomponent.h"

class MGFramework;

class MGStationaryObject :public MGComponent
{
private:
	int m_TileX;
	int m_TileY;

public:

	MGStationaryObject();
	~MGStationaryObject();
	
	friend bool operator<(MGStationaryObject &lhs, MGStationaryObject &rhs);

	void initialize();
	void setTileXY(int x, int y, MGFramework *world);
	int getTileX(){ return m_TileX;}
	int getTileY(){ return m_TileY;}
	bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);
};



#endif