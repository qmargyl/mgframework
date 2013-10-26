

#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "mgmap.h"
#include "mgframework.h"


MGMap::MGMap()
: 	m_TopEdge(0),
	m_BottomEdge(0),
	m_LeftEdge(0),
	m_RightEdge(0),
	m_TileProperty(NULL),
	m_MouseScrollingOngoing(false),
	m_MouseScrollingXClick(0),
	m_MouseScrollingYClick(0)
{

}

MGMap::~MGMap()
{
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
	return -1;
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

std::list<PathItem> MGMap::calculatePath(eMGFPathType pathType, int ax, int ay, int bx, int by)
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
		std::list<PathItem> evaluated;
		PathItem *n;
		PathItem *e;

		PathItem *now = new PathItem(ax, ay, MGFramework::distance(ax, ay, bx, by));
		path.push_back(*now);

		while (x!=bx || y!=by)
		{
			MGFLOG_INFO("MGMap::calculatePath has current (x,y) = (" << x << ", " << y << ")");
			if(x+1<getWidth() && y+1<getHeight() && !occupant(x+1,y+1))
			{
				n = new PathItem(x+1, y+1, distance(x+1, y+1, bx, by));
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
				n = new PathItem(x-1, y-1, distance(x-1, y-1, bx, by));
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
				n = new PathItem(x+1, y-1, distance(x+1, y-1, bx, by));
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
				n = new PathItem(x-1, y+1, distance(x-1, y+1, bx, by));
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
				n = new PathItem(x, y+1, distance(x, y+1, bx, by));
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
				n = new PathItem(x, y-1, distance(x, y-1, bx, by));
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
				n = new PathItem(x+1, y, distance(x+1, y, bx, by));
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
				n = new PathItem(x-1, y, distance(x-1, y, bx, by));
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
				int bestx=0;
				int besty=0;
				double besth=distance(0, 0, getWidth(), getHeight());
				bool found=false;

				MGFLOG_INFO("MGMap::calculatePath will now evaluate " << neighbors.size() << " neighbors");

				// XXX: Set a limit for how many neighbors we will consider at the most, in each step. 
				for (std::list<PathItem>::iterator it=neighbors.begin(); it != neighbors.end(); ++it)
				{
					if((*it).getH()<besth)
					{
						// First we check if the found tile has already been evaluated.
						bool alreadyEvaluated = false;
						for (std::list<PathItem>::iterator ae = evaluated.begin(); ae != evaluated.end(); ++ae)
						{
							if((*ae).equalCoordinate(&(*it)))
							{
								alreadyEvaluated = true;
							}
						}

						if(!alreadyEvaluated)
						{
							// Add the current (x,y) to list of evaluated coordinates..
							e = new PathItem(x, y, MGFramework::distance(x, y, bx, by));
							evaluated.push_back(*e);

							MGFLOG_INFO("MGMap::calculatePath found better candidate: " << besth << " > " << (*it).getH() << ", (" << (*it).getX() << "," << (*it).getY() << ")");
							while(x<(*it).getX()-1 || x>(*it).getX()+1 || y<(*it).getY()-1 || y>(*it).getY()+1)
							{
								// The found tile is not a neighbor of {x,y}
								// Set H to a large number to not try this neighbor again.
								// Back-track.
								MGFLOG_INFO("MGMap::calculatePath marking bad neighbor: " << (*it).getX() << "," << (*it).getY());
								(*it).setH(getWidth() + getHeight());

								if(it != neighbors.begin())
								{
									MGFLOG_INFO("MGMap::calculatePath back-tracking");
									--it;
									if(path.size()>0)
									{
										path.pop_back();

										if(!path.empty())
										{
											x=path.back().getX();
											y=path.back().getY();
										}
										else
										{
											break;
										}
									}
								}
								else
								{
									MGFLOG_INFO("MGMap::calculatePath stopping back-tracking");
									break;
								}
							}

							bestx=(*it).getX();
							besty=(*it).getY();
							besth=(*it).getH();
							found = !(x < bestx-1 || x > bestx+1 || y < besty-1 || y > besty+1);
						}
					}
				}

				if(found)
				{
					if(x < bestx-1 || x > bestx+1 || y < besty-1 || y > besty+1)
					{
						// XXX: Is this really necessary?
						MGFLOG_ERROR("MGMap::calculatePath suggested a non-neighbor as next step in path");
						path.clear();
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

					// XXX: Why is this only ok one tile away from target? What if a cluster of tiles is occupied?
					if(MGFramework::distance(x, y, bx, by) < 2 && occupant(bx,by) != 0)
					{
						// The tile we want to go to is occopied and we are one tile away

					}
					else
					{
						MGFLOG_WARNING("MGMap::calculatePath was not able to find a path");

					}
					break;
				}
			}
			else
			{
				MGFLOG_WARNING("MGMap::calculatePath was not able to find a path as there are no available neighbor tiles to evaluate");
				break;
			}
			

			// Some additional checks..
			if(neighbors.size() > 200)
			{
				MGFLOG_WARNING("MGMap::calculatePath created a too long neighbor list, purging half...");
				size_t nToPurge = neighbors.size() / 2;
				for(int i=0; i < (int)nToPurge; ++i)
				{
					neighbors.pop_back();
				}
			}
			if(path.size() > 1000)
			{
				// XXX: This is not necessarily an error but keep it like that for now so it stands out more...
				MGFLOG_ERROR("MGMap::calculatePath created a too long path, aborting");
				path.clear();
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
				path.clear();
			}

			PathItem *pI = new PathItem(x, y);
			path.push_back(*pI);

		}//while



	}

	// XXX: Perhaps come up with a better way to print the path?
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
		// XXX: What is this caused by? It actually happens sometimes.
		MGFLOG_WARNING("MGMap::calculatePath produced an empty path");
	}

	return path;
}