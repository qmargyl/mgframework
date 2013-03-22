#include "mgframework.h"
#include "mgmovingobject.h"
#include <iostream>

MGMovingObject::MGMovingObject()
{

}

void MGMovingObject::setTileXY(int x, int y)
{
	m_TileX=x;
	m_TileY=y;
	std::cout << "MO: Setting XY (" << x << "," << y << ")" << std::endl;
}

void MGMovingObject::setDestTileXY(int x, int y)
{
	m_DestTileX=x;
	m_DestTileY=y;
	std::cout << "MO: Setting destination XY (" << x << "," << y << ")" << std::endl;
}

int MGMovingObject::getTileX()
{
	return m_TileX;
}

int MGMovingObject::getTileY()
{
	return m_TileY;
}

int MGMovingObject::getDestTileX()
{
	return m_DestTileX;
}

int MGMovingObject::getDestTileY()
{
	return m_DestTileY;
}

double MGMovingObject::getDistance(int wx, int wy)
{
	return MGFramework::distance(getTileX(), getTileY(), wx, wy);
}

void MGMovingObject::setSpeed(double s)
{
	m_Speed=s;
}

bool MGMovingObject::runConsoleCommand(const char *c)
{
	std::cout << "MGMovingObject::runConsoleCommand(" << c << ")" << std::endl;

	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');
	return true;
}