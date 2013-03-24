#ifndef _MG_MOVINGOBJECT_H
#define _MG_MOVINGOBJECT_H
#include "mgcomponent.h"

class MGMovingObject :public MGComponent
{
private:
	double m_Speed;
	int m_TileX;
	int m_TileY;
	int m_DestTileX;
	int m_DestTileY;
	double getDistance(int wx, int wy);

public:
	MGMovingObject();
	void setTileXY(int x, int y);
	void setDestTileXY(int x, int y);
	int getTileX();
	int getTileY();
	int getDestTileX();
	int getDestTileY();
	void setSpeed(double s);

	bool runConsoleCommand(const char *c);

};



#endif