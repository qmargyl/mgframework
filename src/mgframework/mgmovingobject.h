#ifndef _MG_MOVINGOBJECT_H
#define _MG_MOVINGOBJECT_H
#include "mgcomponent.h"


class MGMovingObject :public MGComponent
{
private:
	double m_Speed; // pixels per second
	int m_TimeOfLastUpdate;
	int m_TileX;
	int m_TileY;
	int m_DestTileX;
	int m_DestTileY;
	double getDistance(int wx, int wy);
	double m_X;
	double m_Y;
	int m_tileSize;

public:

	MGMovingObject();
	void setTileXY(int x, int y);
	void setDestTileXY(int x, int y);
	int getTileX(){ return m_TileX;}
	int getTileY(){ return m_TileY;}
	int getDestTileX(){ return m_DestTileX;}
	int getDestTileY(){ return m_DestTileY;}
	void setSpeed(double s, int tileSize); // Seconds, Distance to next tile (pixels)
	void update();

	int getXOffset(){ return (int)(m_X+0.5);}
	int getYOffset(){ return (int)(m_Y+0.5);}

	bool runConsoleCommand(const char *c);

};



#endif