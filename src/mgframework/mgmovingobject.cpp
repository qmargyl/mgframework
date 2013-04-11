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
	//std::cout << "MGMovingObject::setTileXY: m_X=" << m_X << ", m_Y=" << m_Y << std::endl;
	//std::cout << "MO: Setting XY (" << x << "," << y << ") m_X=" << m_X << ", m_Y=" << m_Y << std::endl;
	m_TileX=x;
	m_TileY=y;
	m_X=0.0;
	m_Y=0.0;
	m_FinishingLastMove=false;
}

void MGMovingObject::setDestTileXY(int x, int y)
{
	//std::cout << "MGMovingObject::setDestTileXY (" << x << "," << y << ")" << std::endl;
	if(m_X!=0.0 || m_Y!=0.0 || m_FinishingLastMove)
	{
		m_TempDestTileX=m_DestTileX;
		m_TempDestTileY=m_DestTileY;
		m_FinishingLastMove=true;
		//std::cout << "MGMovingObject::setDestTileXY, When finished: m_X=" << m_X << ", m_Y=" << m_Y << std::endl;
	}
	else
	{
		//std::cout << "MGMovingObject::setDestTileXY, Now: m_X=" << m_X << ", m_Y=" << m_Y << std::endl;
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
	m_TileSize = tileSize;
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

		if(m_X>=getTileSize() && m_Y>=getTileSize())
		{
			//std::cout << "A" << std::endl;
			setTileXY(getTileX()+1, getTileY()+1);
		}
		else if(m_X>=getTileSize() && m_Y==0)
		{
			//std::cout << "B" << std::endl;
			setTileXY(getTileX()+1, getTileY());
		}
		else if(-m_X>=getTileSize() && -m_Y>=getTileSize())
		{
			//std::cout << "C" << std::endl;
			setTileXY(getTileX()-1, getTileY()-1);
		}
		else if(-m_X>=getTileSize() && m_Y==0)
		{
			//std::cout << "D" << std::endl;
			setTileXY(getTileX()-1, getTileY());
		}
		if(m_X>=getTileSize() && -m_Y>=getTileSize())
		{
			//std::cout << "E" << std::endl;
			setTileXY(getTileX()+1, getTileY()-1);
		}
		else if(m_Y>=getTileSize() && m_X==0)
		{
			//std::cout << "F" << std::endl;
			setTileXY(getTileX(), getTileY()+1);
		}
		else if(-m_X>=getTileSize() && m_Y>=getTileSize())
		{
			//std::cout << "G" << std::endl;
			setTileXY(getTileX()-1, getTileY()+1);
		}
		else if(-m_Y>=getTileSize() && m_X==0)
		{
			//std::cout << "H" << std::endl;
			setTileXY(getTileX(), getTileY()-1);
		}
	}

	m_TimeOfLastUpdate = SDL_GetTicks();
}


void MGMovingObject::copy(const MGMovingObject *src)
{
	//Copy ALL class variables!
	m_Speed = src->m_Speed;
	m_TimeOfLastUpdate = src->m_TimeOfLastUpdate;
	m_TileX = src->m_TileX;
	m_TileY = src->m_TileY;
	m_DestTileX = src->m_DestTileX;
	m_DestTileY = src->m_DestTileY;
	m_X = src->m_X;
	m_Y = src->m_Y;
	m_TileSize = src->m_TileSize;
	m_FinishingLastMove = src->m_FinishingLastMove;
	m_TempDestTileX = src->m_TempDestTileX;
	m_TempDestTileY = src->m_TempDestTileY;
}

bool MGMovingObject::runConsoleCommand(const char *c)
{
	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');

	if(cmdvec.size() == 3)
	{
		if(cmdvec[2]=="help")
		{
			std::cout << std::endl << "mo <i> help - Displays help information for console commands implemented" << std::endl;
			std::cout << "          in MGMovingObject. <i> is the ID of the MO." << std::endl;
			std::cout << "mo <i> getspeed - Returns the speed of the MO with ID <i>, in pixels per" << std::endl;
			std::cout << "          second." << std::endl;
			std::cout << "mo <i> getdestination - Returns the destination of the MO with ID <i>." << std::endl;
			std::cout << "mo <i> getlocation - Returns the location of the MO with ID <i>." << std::endl;
			std::cout << "mo <i> getdistance - Returns the distance to the destination of the MO with ID <i>." << std::endl;

			return true;
		}
		else if(cmdvec[2]=="getspeed")
		{
			std::cout << "" << getSpeed() << std::endl;
			return true;
		}
		else if(cmdvec[2]=="getdestination")
		{
			std::cout << "{" << getDestTileX() << "," << getDestTileY() << "}" << std::endl;
			return true;
		}
		else if(cmdvec[2]=="getlocation")
		{
			std::cout << "{" << getTileX() << "," << getTileY() << "}" << std::endl;
			return true;
		}
		else if(cmdvec[2]=="getdistance")
		{
			std::cout << getDistance(getDestTileX(), getDestTileY()) << std::endl;
			return true;
		}

	}

	std::cout << "Error in command (mo ...)" << std::endl;
	return true;
}