#include "mgframework.h"
#include "mgmovingobject.h"
#include <iostream>

MGMovingObject::MGMovingObject()
{
	m_TimeOfLastUpdate = SDL_GetTicks();
	m_FinishingLastMove=false;
}

void MGMovingObject::setTileXY(int x, int y)
{
	m_TileX=x;
	m_TileY=y;
	m_X=0.0;
	m_Y=0.0;
	m_FinishingLastMove=false;
	std::cout << "MO: Setting XY (" << x << "," << y << ")" << std::endl;
}

void MGMovingObject::setDestTileXY(int x, int y)
{
	if(m_X!=0.0 || m_Y!=0.0 || m_FinishingLastMove)
	{
		m_TempDestTileX=m_DestTileX;
		m_TempDestTileY=m_DestTileY;
		m_FinishingLastMove=true;
		std::cout << "MO: Setting next destination XY (" << x << "," << y << ")" << std::endl;
	}
	else
	{
		std::cout << "MO: Setting destination XY (" << x << "," << y << ")" << std::endl;
	}
	m_DestTileX=x;
	m_DestTileY=y;
}

double MGMovingObject::getDistance(int wx, int wy)
{ 
	return MGFramework::distance(getTileX(), getTileY(), wx, wy);
}

void MGMovingObject::setSpeed(double s, int tileSize)
{
	m_Speed=(double)tileSize / s;
	m_tileSize = tileSize;
}

void MGMovingObject::update()
{
	int timeSinceLastUpdate = SDL_GetTicks() - m_TimeOfLastUpdate;
	double d = m_Speed * (timeSinceLastUpdate / 1000.0);

	if(getDestTileX()!=getTileX() || getDestTileY()!=getTileY())
	{
		if(getDestTileX()>getTileX() && getDestTileY()>getTileY())
		{
			m_X += d;
			m_Y += d;
		}
		else if(getDestTileX()<getTileX() && getDestTileY()<getTileY())
		{
			m_X -= d;
			m_Y -= d;
		}
		else if(getDestTileX()>getTileX() && getDestTileY()<getTileY())
		{
			m_X += d;
			m_Y -= d;
		}
		else if(getDestTileX()<getTileX() && getDestTileY()>getTileY())
		{
			m_X -= d;
			m_Y += d;
		}
		else if(getDestTileX()>getTileX())
		{
			m_X += d*1.414;
		}
		else if(getDestTileX()<getTileX())
		{
			m_X -= d*1.414;
		}
		else if(getDestTileY()>getTileY())
		{
			m_Y += d*1.414;
		}
		else if(getDestTileY()<getTileY())
		{
			m_Y -= d*1.414;
		}
		else
		{
			std::cout << "WARNING: MGMovingObject::update() executed a case which should never happen" << std::endl;
			setTileXY(getTileX(), getTileY());
		}


		if(m_X>=m_tileSize && m_Y>=m_tileSize)
		{
			setTileXY(getTileX()+1, getTileY()+1);
		}
		else if(m_X>=m_tileSize && m_Y==0)
		{
			setTileXY(getTileX()+1, getTileY());
		}
		else if(-m_X>=m_tileSize && -m_Y>=m_tileSize)
		{
			setTileXY(getTileX()-1, getTileY()-1);
		}
		else if(-m_X>=m_tileSize && m_Y==0)
		{
			setTileXY(getTileX()-1, getTileY());
		}
		if(m_X>=m_tileSize && -m_Y>=m_tileSize)
		{
			setTileXY(getTileX()+1, getTileY()-1);
		}
		else if(m_Y>=m_tileSize && m_X==0)
		{
			setTileXY(getTileX(), getTileY()+1);
		}
		else if(-m_X>=m_tileSize && m_Y>=m_tileSize)
		{
			setTileXY(getTileX()-1, getTileY()+1);
		}
		else if(-m_Y>=m_tileSize && m_X==0)
		{
			setTileXY(getTileX(), getTileY()-1);
		}
	}

	m_TimeOfLastUpdate = SDL_GetTicks();
}

bool MGMovingObject::runConsoleCommand(const char *c)
{
	std::cout << "MGMovingObject::runConsoleCommand(" << c << ")" << std::endl;

	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');
	return true;
}