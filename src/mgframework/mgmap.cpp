
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
	//registerMemoryAllocation(sizeof(MGMap));
}

MGMap::~MGMap()
{
	//registerMemoryDeallocation(sizeof(MGMap));
	delete[] m_TileProperty;
	delete[] m_Occupied;
}



void MGMap::setScrollOffset(int px, int py)
{
	m_ScrollX = px;
	m_ScrollY = py;
}

void MGMap::init(int w, int h, int tw, int th, int windowWidth, int windowHeight)
{
	delete[] m_TileProperty;
	delete[] m_Occupied;
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


void MGMap::reInit(int w, int h, int tw, int th)
{
	init(w, h, tw, th, getWindowWidth(), getWindowHeight());
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
			int x1 = w->toInt(cmdvec[2]);
			int y1 = w->toInt(cmdvec[3]);
			int x2 = w->toInt(cmdvec[4]);
			int y2 = w->toInt(cmdvec[5]);

			MGFLOG_INFO("Calculating closest path from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ").");
			calculatePath(MGFBASICPATH1, x1, y1, x2, y2);
			return true;
		}

		case MGComponent_MAP_SETSIZE_INT_INT_INT_INT:
		{
			w->registerUsedCommand(MGComponent_MAP_SETSIZE_INT_INT_INT_INT);
			int x = w->toInt(cmdvec[2]);
			int y = w->toInt(cmdvec[3]);
			int tx = w->toInt(cmdvec[4]);
			int ty = w->toInt(cmdvec[5]);
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
		if(cmdvec[1]=="help")
		{
			return MGComponent_MAP_HELP;
		}
	}
	else if(cmdvec.size() == 3)
	{
		if(cmdvec[1]=="logging" && cmdvec[2]=="on")
		{
			return MGComponent_MAP_LOGGING_ON;
		}
		if(cmdvec[1]=="logging" && cmdvec[2]=="off")
		{
			return MGComponent_MAP_LOGGING_OFF;
		}
	}
	else if(cmdvec.size() == 6)
	{
		if(cmdvec[1]=="path")
		{
			return MGComponent_MAP_PATH_INT_INT_INT_INT;
		}
		else if(cmdvec[1]=="setsize")
		{
			return MGComponent_MAP_SETSIZE_INT_INT_INT_INT;
		}
	}

	return MGComponent_UNDEFINED;
}

void MGMap::calculatePath(eMGFPathType pathType, int ax, int ay, int bx, int by)
{
	// Algorithm MGFSKYPATH:
	// 1) Step (x,y) towards target. Diagonally first and then straight ahead.

	// Algorithm MGFBASICPATH1:
	// 1) For all non-blocked adjacent tiles of (x,y) not already in the path, calculate estimated distance to target.
	// 2) Push the "best" alternative to the path. Back-track if necessary.
	// 3) Step (x,y) and goto 1)

	MGFLOG_INFO("MGMap::calculatePath will find a path (" << ax << ", " << ay << ") -> (" << bx << ", " << by << ")");
	std::list<PathItem> path;

	if(pathType==MGFBASICPATH1)
	{
		int x=ax;
		int y=ay;
		std::list<PathItem> neighbors;
		PathItem *n;

		while (x!=bx || y!=by)
		{
			if(x+1<getWidth() && y+1<getHeight() && !occupant(x+1,y+1))
			{
				n = new PathItem(x+1, y+1, MGFramework::distance(x+1, y+1, bx, by));
				bool skip = false;
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x-1>=0 && y-1>=0 && !occupant(x-1,y-1))
			{
				n = new PathItem(x-1, y-1, MGFramework::distance(x-1, y-1, bx, by));
				bool skip = false;
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x+1<getWidth() && y-1>=0 && !occupant(x+1,y-1))
			{
				n = new PathItem(x+1, y-1, MGFramework::distance(x+1, y-1, bx, by));
				bool skip = false;
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x-1>=0 && y+1<getHeight() && !occupant(x-1,y+1))
			{
				n = new PathItem(x-1, y+1, MGFramework::distance(x-1, y+1, bx, by));
				bool skip = false;
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(y+1<getHeight() && !occupant(x,y+1))
			{
				n = new PathItem(x, y+1, MGFramework::distance(x, y+1, bx, by));
				bool skip = false;
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(y-1>=0 && !occupant(x,y-1))
			{
				n = new PathItem(x, y-1, MGFramework::distance(x, y-1, bx, by));
				bool skip = false;
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x+1<getWidth() && !occupant(x+1,y))
			{
				n = new PathItem(x+1, y, MGFramework::distance(x+1, y, bx, by));
				bool skip = false;
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}
			if(x-1>=0 && !occupant(x-1,y))
			{
				n = new PathItem(x-1, y, MGFramework::distance(x-1, y, bx, by));
				bool skip = false;
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if(n->equalCoordinate(&(*it)))
					{
						skip = true;
					}
				}
				if(!skip) neighbors.push_front(*n);
			}

			if(!neighbors.empty())
			{
				//MGFLOG_INFO(	"MGMap::calculatePath has a neighbor list of " << neighbors.size() << 
				//				" elements and a path of " << path.size() << " elements");
				int bestx=0;
				int besty=0;
				double besth=MGFramework::distance(0, 0, getWidth(), getHeight());
				bool found=false;

				MGFLOG_INFO("MGMap::calculatePath will now evaluate " << neighbors.size() << " neighbors");

				// TODO: Set a limit for how many neighbors we will consider at the most, in each step. 
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if((*it).getH()<besth)
					{
						// First we check if the found tile is already in the path.
						bool alreadyInPath=false;
						for (std::list<PathItem>::iterator pit=path.begin(); pit != path.end(); ++pit)
						{
							if((*pit).equalCoordinate(&(*it)))
							{
								alreadyInPath = true;
							}
						}
						if(!alreadyInPath)
						{
							MGFLOG_INFO("MGMap::calculatePath found better tile: " << besth << " > " << (*it).getH());
							if(x<(*it).getX()-1 || x>(*it).getX()+1 || y<(*it).getY()-1 || y>(*it).getY()+1)
							{
								// The found tile is not a neighbor of {x,y}
								// TODO: Back tack somehow?
								// Set H to a large number to not try this neighbor again.
								(*it).setH(MGFramework::distance(0, 0, getWidth(), getHeight()));
								if(it != neighbors.begin())
								{
									--it;
									if(path.size()>0)
									{
										path.pop_back();
									}
								}
								else
								{
									MGFLOG_WARNING("MGMap::calculatePath was not able to find a path by back-tracking");
									break;
								}
								// XXX: Also back-track here, not only mark the neighbor tile as bad.

							}
							else
							{
								bestx=(*it).getX();
								besty=(*it).getY();
								besth=(*it).getH();
								found=true;
							}
						}
					}
				}

				if(found)
				{
					if(x<bestx-1 || x>bestx+1 || y<besty-1 || y>besty+1)
					{
						// The found {bestx,besty} is not a neighbor of {x,y} -> ERROR, since 
						// we have already handled this case above
						MGFLOG_ERROR("MGMap::calculatePath suggested a non-neighbor as next step in path");
						break;
					}
					PathItem *pI = new PathItem(bestx, besty, MGFramework::distance(bestx, besty, bx, by));
					path.push_back(*pI);
					x=bestx;
					y=besty;
				}
				else
				{
					// XXX: Should we really give up here? What about back tracking?
					//      Yes i think so. We have already tried to back-track...
					MGFLOG_WARNING("MGMap::calculatePath was not able to find a path");
					break;
				}
			}
			else
			{
				MGFLOG_WARNING("MGMap::calculatePath was not able to find a path as there are no available neighbor tiles to evaluate");
				break;
			}
			

			// Some additional checks..
			if(neighbors.size() > 1000)
			{
				// XXX: This is not necessarily an error but keep it like that for now so it stands out more...
				MGFLOG_ERROR("MGMap::calculatePath created a too long neighbor list, aborting");
				break;
			}
			if(path.size() > 1000)
			{
				// XXX: This is not necessarily an error but keep it like that for now so it stands out more...
				MGFLOG_ERROR("MGMap::calculatePath created a too long path, aborting");
				break;
			}
		}//while

	}

	else if(pathType==MGFSKYPATH)
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
				MGFLOG_ERROR("MGMap::calculatePath executed a case which should never happen");
				MGFLOG_ERROR("Data: x=" << x << ", y=" << y << ", ax=" << ax << ", ay=" << ay << ", bx=" << bx << ", by=" << by);
			}

			PathItem *pI = new PathItem(x, y);
			path.push_back(*pI);

		}//while



	}
	//delete [] path;

	// This will not look like this later... leave it for now.

	if(!path.empty())
	{
		MGFLOG_INFO("Path:");
		for (std::list<PathItem>::iterator it=path.begin(); it != path.end(); ++it)
		{
			MGFLOG_INFO("(" << (*it).getX() << ", " << (*it).getY() << ")");
		}
	}
	else
	{
		MGFLOG_WARNING("MGMap::calculatePath was not able to find a path");
	}

}