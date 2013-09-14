
#include "mgmap.h"
#include <stdlib.h>
#include "mgframework.h"
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <list>


MGMap::MGMap()
{
	m_TileProperty = NULL;
	m_MouseScrollingOngoing = false;
	m_MouseScrollingXClick=0;
	m_MouseScrollingYClick=0;
}

MGMap::~MGMap()
{
	if(m_TileProperty != NULL)
	{
		delete[] m_TileProperty;
	}
}



void MGMap::setScrollOffset(int px, int py)
{
	m_ScrollX = px;
	m_ScrollY = py;
}

void MGMap::init(int w, int h, int tw, int th, int windowWidth, int windowHeight)
{
	m_TileProperty = new Uint32[w*h];
	m_Occupied = new int[w*h];
	m_Width = w;
	m_Height = h;
	m_TileWidth = tw;
	m_TileHeight = th;
	setScrollOffset(0, 0);
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;

	// The proper way to populate the tile properties would be to 
	// for example read in a level file, or set hard coded values.
	// Here is just an example so we have something before the
	// framework can load a file.. Obviously the hardcoded tile
	// properties should not be part of the framework.

	for (int x=0; x < getWidth(); x++)
	{
		for (int y=0; y < getHeight(); y++)
		{
			setTileProperty(x, y, MGMAP_TP_PROPERTY_1 | MGMAP_TP_NOOBSTACLE);
			unOccupy(x, y);
		}
	}

	// Just an example...
	setTileProperty(3, 3, MGMAP_TP_PROPERTY_2 | MGMAP_TP_NOOBSTACLE);
	setTileProperty(4, 4, MGMAP_TP_PROPERTY_2 | MGMAP_TP_NOOBSTACLE);
	setTileProperty(5, 5, MGMAP_TP_PROPERTY_2 | MGMAP_TP_NOOBSTACLE);
	setTileProperty(6, 4, MGMAP_TP_PROPERTY_2 | MGMAP_TP_NOOBSTACLE);
	setTileProperty(7, 3, MGMAP_TP_PROPERTY_2 | MGMAP_TP_NOOBSTACLE);
	setTileProperty(8, 2, MGMAP_TP_PROPERTY_2 | MGMAP_TP_NOOBSTACLE);

}


/***************************************************
 * int MGMap::getTileIndex(int clickX, int clickY)
 * Returns tile index of clicked tile. If click
 * was outside of the map then -1 will be returned.
 ***************************************************/
int MGMap::getTileIndex(int clickX, int clickY)
{
	int x = (clickX - getScrollX()) / getTileWidth();
	int y = (clickY - getScrollY()) / getTileHeight();
	if((x < getWidth()) && (y < getHeight()))
	{
		return y * getWidth() + x;
	}
	else
	{
		return -1;
	}
}



void MGMap::save()
{
	// Not implemented yet
	return ;
}

void MGMap::mouseScrollingRelease(int x, int y)
{
	m_MouseScrollingOngoing = false;
}

void MGMap::mouseScrollingClick(int x, int y)
{
	m_MouseScrollingOngoing = true;
	m_MouseScrollingXClick=x;
	m_MouseScrollingYClick=y;
}

void MGMap::mouseScrollingUpdate(int x, int y)
{
	// Possible optimization by re-using x and y variables and making the method inline.
	if(m_MouseScrollingOngoing)
	{
		int setY = getScrollY() + m_MouseScrollingYClick - y;
		int setX = getScrollX() + m_MouseScrollingXClick - x;

		if(setY > 0)
		{
			setY = 0;
		}
		if(setY < getWindowHeight() - getHeight() * getTileHeight())
		{
			setY = getWindowHeight() - getHeight() * getTileHeight();
		}

		if(setX > 0)
		{
			setX = 0;
		}
		if(setX < getWindowWidth() - getWidth() * getTileWidth())
		{
			setX = getWindowWidth() - getWidth() * getTileWidth();
		}

		setScrollOffset(setX, setY);
	}
}

bool MGMap::runConsoleCommand(const char *c, MGFramework *w)
{
	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
			MGFLOG_ERROR(std::cout << "MGMap::runConsoleCommand received MGComponent_UNDEFINED from MGMap::detectMGComponentConsoleCommand" << std::endl;); 
			break;

		case MGComponent_MAP_HELP:
		{
			std::cout << "-----------------------------------------------------------------------------" << std::endl << std::endl;
			std::cout << "map help - Displays help information for console commands implemented in the" << std::endl;
			std::cout << "           map object." << std::endl;
			return true;
		}

		case MGComponent_MAP_PATH_INT_INT_INT_INT:
		{
			int x1 = MGFramework::toInt(cmdvec[2]);
			int y1 = MGFramework::toInt(cmdvec[3]);
			int x2 = MGFramework::toInt(cmdvec[4]);
			int y2 = MGFramework::toInt(cmdvec[5]);

			MGFLOG_INFO(std::cout << "Calculating closest path from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")." << std::endl;);
			calculatePath(MGFSKYPATH, x1, y1, x2, y2);
			return true;
		}

		default:
			MGFLOG_ERROR(std::cout << "MGMap::detectComponentConsoleCommand returned a bad value" << std::endl;); 
			return true;
	}

	std::cout << "Unknown command" << std::endl;
	return true;
}

eMGComponentConsoleCommand MGMap::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	if(cmdvec.size() == 2)
	{
		if(cmdvec[1]=="help")
		{
			return MGComponent_MAP_HELP;
		}
	}
	else if(cmdvec.size() == 6)
	{
		if(cmdvec[1]=="path")
		{
			return MGComponent_MAP_PATH_INT_INT_INT_INT;
		}
	}

	return MGComponent_UNDEFINED;
}

void MGMap::calculatePath(eMGFPathType pathType, int ax, int ay, int bx, int by)
{
	std::list<PathItem> path;

	if(pathType==MGFSKYPATH)
	{
		int x=ax;
		int y=ay;

		while (x!=bx || y!=by)
		{
			if(x>bx && y>by)
			{
				x--;
				y--;
			}
			else if(x<bx && y<by)
			{
				x++;
				y++;
			}
			else if(x>bx && y<by)
			{
				x--;
				y++;
			}
			else if(x<bx && y>by)
			{
				x++;
				y--;
			}
			else if(x<bx)
			{
				x++;
			}
			else if(x>bx)
			{
				x--;
			}
			else if(y<by)
			{
				y++;
			}
			else if(y>by)
			{
				y--;
			}
			else
			{
				MGFLOG_ERROR(std::cout << "WARNING: MGMap::calculatePath() executed a case which should never happen" << std::endl;);
				MGFLOG_ERROR(std::cout << "Data: x=" << x << ", y=" << y << ", ax=" << ax << ", ay=" << ay << ", bx=" << bx << ", by=" << by << std::endl;);
			}

			PathItem *pI = new PathItem(x, y);
			path.push_back(*pI);
			//std::cout << "Add to path: " << pI->getX() << ", " << pI->getY() << std::endl;

		}

		// This will not look like this later... leave it for now.
		if(!path.empty())
		{
			std::cout << "Path: ";
			for (std::list<PathItem>::iterator it=path.begin(); it != path.end(); ++it)
			{
				std::cout << "(" << (*it).getX() << ", " << (*it).getY() << ") ";
			}
			std::cout << std::endl;
		}

	}
	//delete [] path;
}