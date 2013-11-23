#ifndef _MG_STATIONARYOBJECT_H
#define _MG_STATIONARYOBJECT_H
#include "mgcomponent.h"

class MGFramework;

class MGStationaryObject :public MGComponent
{
private:
	int m_TileX;
	int m_TileY;
	static int m_TileSize;

	double getDistance(int wx, int wy);
	int getTileSize(){ return m_TileSize;}

public:

	MGStationaryObject();
	~MGStationaryObject();

	void initialize();

	void setTileXY(int x, int y, MGFramework *world);

	void copy(MGStationaryObject *src);

	int getTileX(){ return m_TileX;}
	int getTileY(){ return m_TileY;}

	int getCenterX(){ return getTileX()*getTileSize() + (int)(0.5 *getTileSize());}
	int getCenterY(){ return getTileY()*getTileSize() + (int)(0.5 *getTileSize());}
	bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);

};



#endif