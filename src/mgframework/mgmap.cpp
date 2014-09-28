

#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <sstream>
#include "mgmap.h"
#include "mgframework.h"
#include "mgpathitem.h"
#include "mgpathgenerator.h"

unsigned long int MGMap::m_SC_CalculatedPaths = 0;
unsigned long int MGMap::m_SC_LongestCalculatedPath = 0;
unsigned long int MGMap::m_SC_FailedPathCalculations = 0;

MGMap::MGMap()
:	m_Width(0),
	m_Height(0),
	m_TileWidth(0),
	m_TileHeight(0),
	m_ScrollX(0),
	m_ScrollY(0),
	m_TileProperty(NULL),
	m_Occupied(NULL),
	m_MouseScrollingOngoing(false),
	m_MouseScrollingXClick(0),
	m_MouseScrollingYClick(0),
	m_TopEdge(0),
	m_BottomEdge(0),
	m_LeftEdge(0),
	m_RightEdge(0),	
	m_MarkedForRendering(NULL)
{

}

MGMap::~MGMap()
{
	delete[] m_TileProperty;
	delete[] m_Occupied;
	delete[] m_MarkedForRendering;
}

void MGMap::setScrollOffset(int px, int py)
{
	m_ScrollX = px;
	m_ScrollY = py;
}

void MGMap::init(int w, int h, int tw, int th, int windowWidth, int windowHeight)
{
	if(m_TileProperty) delete[] m_TileProperty;
	if(m_Occupied) delete[] m_Occupied;
	if(m_MarkedForRendering) delete[] m_MarkedForRendering;

	m_TileProperty = new unsigned int[w * h];
	m_Occupied = new int[w * h];
	m_MarkedForRendering = new bool[w * h];
	m_Width = w;
	m_Height = h;
	m_TileWidth = tw;
	m_TileHeight = th;
	setScrollOffset(0, 0);
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;

	// The proper way to populate the tile properties would be to 
	// for example read in a level file, or set hard coded values.
	// Here is just an example.

	for (int x = 0; x < getWidth(); x++)
	{
		for (int y = 0; y < getHeight(); y++)
		{
			setTileProperty(x, y, MGMAP_TP_PROPERTY_1 | MGMAP_TP_NOOBSTACLE);
			unOccupy(x, y);
			unmarkForRendering(x, y);
		}
	}
}

void MGMap::reInit(int w, int h, int tw, int th)
{
	init(w, h, tw, th, getWindowWidth(), getWindowHeight());
}

int MGMap::getTileIndex(int clickX, int clickY)
{
	MGFLOG_INFO("MGMap::getTileIndex(" << clickX << ", " << clickY << ")");
	if(	clickX > getLeftEdge() &&
		clickX < (getWindowWidth()-getRightEdge()) && 
		clickY > getTopEdge() && 
		clickY < (getWindowHeight()-getBottomEdge()))
	{
		int x = (clickX - getScrollX()) / getTileWidth();
		int y = (clickY - getScrollY()) / getTileHeight();
		if(x < getWidth() && y < getHeight())
		{
			return y * getWidth() + x;
		}
	}
	return -1; // Click outside of map
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

bool MGMap::mouseScrollingUpdate(int x, int y)
{
	if(m_MouseScrollingOngoing)
	{
		int setY = getScrollY() + m_MouseScrollingYClick - y;
		int setX = getScrollX() + m_MouseScrollingXClick - x;

		if(setY > getTopEdge())
		{
			setY = getTopEdge();
		}
		if(setY < getWindowHeight() - getHeight() * getTileHeight() - getBottomEdge())
		{
			setY = getWindowHeight() - getHeight() * getTileHeight() - getBottomEdge();
		}

		if(setX > getLeftEdge())
		{
			setX = getLeftEdge();
		}
		if(setX < getWindowWidth() - getWidth() * getTileWidth() - getRightEdge())
		{
			setX = getWindowWidth() - getWidth() * getTileWidth() - getRightEdge();
		}

		setScrollOffset(setX, setY);
	}
	return m_MouseScrollingOngoing;
}

bool MGMap::runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s)
{
	char cmd[MGF_SCRIPTLINE_MAXLENGTH];
	strcpy(cmd, c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, " ");

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
			MGFLOG_ERROR("MGMap::runConsoleCommand received MGComponent_UNDEFINED from MGMap::detectMGComponentConsoleCommand"); 
			break;

		case MGComponent_MAP_HELP:
		{
			w->registerUsedCommand(MGComponent_MAP_HELP);
			std::cout << "-----------------------------------------------------------------------------" << std::endl << std::endl;
			std::cout << "map help - Displays help information for console commands implemented in the" << std::endl;
			std::cout << "           map object." << std::endl;
			return true;
		}

		case MGComponent_MAP_PATH_INT_INT_INT_INT:
		{
			w->registerUsedCommand(MGComponent_MAP_PATH_INT_INT_INT_INT);
			int x1 = w->toInt(cmdvec[2], s);
			int y1 = w->toInt(cmdvec[3], s);
			int x2 = w->toInt(cmdvec[4], s);
			int y2 = w->toInt(cmdvec[5], s);
			MGFLOG_INFO("Calculating closest path from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ").");
			calculatePath(MGFBASICPATH1, x1, y1, x2, y2);
			return true;
		}

		case MGComponent_MAP_SETSIZE_INT_INT_INT_INT:
		{
			w->registerUsedCommand(MGComponent_MAP_SETSIZE_INT_INT_INT_INT);
			int x = w->toInt(cmdvec[2], s);
			int y = w->toInt(cmdvec[3], s);
			int tx = w->toInt(cmdvec[4], s);
			int ty = w->toInt(cmdvec[5], s);
			reInit(x,y,tx,ty);
			return true;
		}

		case MGComponent_MAP_LOGGING_ON:
		{
			w->registerUsedCommand(MGComponent_MAP_LOGGING_ON);
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			return true;
		}

		case MGComponent_MAP_LOGGING_OFF:
		{
			w->registerUsedCommand(MGComponent_MAP_LOGGING_OFF);
			disableLogging();
			MGFLOG_INFO("Logging disabled.");
			return true;
		}

		default:
			MGFLOG_ERROR("MGMap::detectComponentConsoleCommand returned a bad value"); 
			return true;
	}

	std::cout << "Unknown command" << std::endl;
	return true;
}

eMGComponentConsoleCommand MGMap::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	if(cmdvec.size() == 2)
	{
		if(cmdvec[1] == "help")
		{
			return MGComponent_MAP_HELP;
		}
	}
	else if(cmdvec.size() == 3)
	{
		if(cmdvec[1] == "logging" && cmdvec[2] == "on")
		{
			return MGComponent_MAP_LOGGING_ON;
		}
		if(cmdvec[1] == "logging" && cmdvec[2] == "off")
		{
			return MGComponent_MAP_LOGGING_OFF;
		}
	}
	else if(cmdvec.size() == 6)
	{
		if(cmdvec[1] == "path")
		{
			return MGComponent_MAP_PATH_INT_INT_INT_INT;
		}
		else if(cmdvec[1] == "setsize")
		{
			return MGComponent_MAP_SETSIZE_INT_INT_INT_INT;
		}
	}

	return MGComponent_UNDEFINED;
}

std::list<MGPathItem> MGMap::calculatePath(eMGFPathType pathType, int ax, int ay, int bx, int by)
{
	// Algorithm MGFSKYPATH:
	// 1) Step (x,y) towards target. Diagonally first and then straight ahead.

	// Algorithm MGFBASICPATH1:
	// 1) For all non-blocked adjacent tiles of (x,y) not already in the path, calculate estimated distance to target.
	// 2) Push the "best" alternative to the path. Back-track if necessary.
	// 3) Step (x,y) and goto 1)

	MGFLOG_INFO("MGMap::calculatePath will find a path (" << ax << ", " << ay << ") -> (" << bx << ", " << by << ")");
	std::list<MGPathItem> path;

	if(pathType == MGFASTARLIST)
	{
		MGPathGenerator::calculatePathAStar(ax, ay, bx, by, *this, path);
	}

	else if(pathType == MGFBASICPATH1)
	{
		MGPathGenerator::calculatePathBasic(ax, ay, bx, by, *this, path);
	}

	else if(pathType == MGFSKYPATH)
	{
		int x = ax;
		int y = ay;

		while (x != bx || y != by)
		{
			if(x > bx && y > by)
			{
				x--;
				y--;
			}
			else if(x < bx && y < by)
			{
				x++;
				y++;
			}
			else if(x > bx && y < by)
			{
				x--;
				y++;
			}
			else if(x < bx && y > by)
			{
				x++;
				y--;
			}
			else if(x < bx)
			{
				x++;
			}
			else if(x > bx)
			{
				x--;
			}
			else if(y < by)
			{
				y++;
			}
			else //if(y > by)
			{
				y--;
			}

			MGPathItem *pI = new MGPathItem(x, y);
			path.push_back(*pI); //TODO: Call constructor without new as argument. 

		}//while

	}

	// XXX: Perhaps come up with a better way to print the path?
	if(!path.empty())
	{
		if(loggingEnabled())
		{
			MGFLOG_INFO("Path:");
			for (std::list<MGPathItem>::iterator it=path.begin(); it != path.end(); ++it)
			{
				MGFLOG_INFO("(" << (*it).getX() << ", " << (*it).getY() << ")");
			}
		}
	}
	else
	{
		// XXX: What is this caused by? It actually happens sometimes.
		MGFLOG_WARNING("MGMap::calculatePath produced an empty path");
	}

	return path;
}

void MGMap::printStatisticsCounters()
{

}