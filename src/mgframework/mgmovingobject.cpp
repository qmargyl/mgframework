#include "mgframework.h"
#include "mgmovingobject.h"
#include <iostream>

int MGMovingObject::m_TileSize = 0;

MGMovingObject::MGMovingObject()
{
	setTimeOfLastUpdate(SDL_GetTicks());
	m_FinishingLastMove=false;
	m_Marked=false;
	m_TileX=0;
	m_TileY=0;
	m_NextTileX=0;
	m_NextTileY=0;
	m_CurrentState=MOStateCreated;
	setOwner(MGF_NOPLAYER);
	registerMemoryAllocation(sizeof(MGMovingObject));
}

MGMovingObject::~MGMovingObject()
{
	registerMemoryDeallocation(sizeof(MGMovingObject));
}

void MGMovingObject::initialize()
{
	setTimeOfLastUpdate(SDL_GetTicks());
	m_FinishingLastMove=false;
	m_Marked=false;
	m_TileX=0;
	m_TileY=0;
	m_NextTileX=0;
	m_NextTileY=0;
	setOwner(MGF_NOPLAYER);
	//enableLogging();
	registerMemoryAllocation(sizeof(MGMovingObject));
	if(!isCreated())
	{
		MGFLOG_ERROR("MGMovingObject::initialize was called in the wrong state: " << toString(getCurrentState())); 
	}
	changeState(MOStateIdle);
}

void MGMovingObject::setTileXY(int x, int y, MGFramework *world)
{
	world->m_Map.unOccupy(getTileX(), getTileY());
	m_TileX=x;
	m_TileY=y;
	m_X=0.0;
	m_Y=0.0;
	m_FinishingLastMove=false;
	if(getDestTileX()==getTileX() && getDestTileY()==getTileY() && isMoving())
	{
		changeState(MOStateIdle);
	}
}

void MGMovingObject::setNextXY(int x, int y, MGFramework *world)
{
	world->m_Map.occupy(x, y, getID());
	m_NextTileX = x; 
	m_NextTileY = y;
}

void MGMovingObject::setDestTileXY(int x, int y)
{
	// If we are still finishing our last move, save current 
	// destination to temp variable and then set new destination
	if(m_X!=0.0 || m_Y!=0.0 || m_FinishingLastMove)
	{
		m_TempDestTileX=m_DestTileX;
		m_TempDestTileY=m_DestTileY;
		m_FinishingLastMove=true;
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

void MGMovingObject::update(MGFramework *w)
{
	int timeSinceLastUpdate = SDL_GetTicks() - getTimeOfLastUpdate();
	double d = m_Speed * (timeSinceLastUpdate / 1000.0);

	if(getDestTileX()!=getTileX() || getDestTileY()!=getTileY())
	{
		if(!isStuck())
		{
			changeState(MOStateMoving);
		}

		if(getDestTileX()>getTileX() && getDestTileY()>getTileY())
		{
			if(MGFramework::oneOf(w->m_Map.occupant(getTileX()+1, getTileY()+1), 0, getID()))
			{
				setNextXY(getTileX()+1, getTileY()+1, w);
				m_X += d;
				m_Y += d;
			}
		}
		else if(getDestTileX()<getTileX() && getDestTileY()<getTileY())
		{
			if(MGFramework::oneOf(w->m_Map.occupant(getTileX()-1, getTileY()-1), 0, getID()))
			{
				setNextXY(getTileX()-1, getTileY()-1, w);
				m_X -= d;
				m_Y -= d;
			}
		}
		else if(getDestTileX()>getTileX() && getDestTileY()<getTileY())
		{
			if(MGFramework::oneOf(w->m_Map.occupant(getTileX()+1, getTileY()-1), 0, getID()))
			{
				setNextXY(getTileX()+1, getTileY()-1, w);
				m_X += d;
				m_Y -= d;
			}
		}
		else if(getDestTileX()<getTileX() && getDestTileY()>getTileY())
		{
			if(MGFramework::oneOf(w->m_Map.occupant(getTileX()-1, getTileY()+1), 0, getID()))
			{
				setNextXY(getTileX()-1, getTileY()+1, w);
				m_X -= d;
				m_Y += d;
			}
		}
		else if(getDestTileX()>getTileX())
		{
			if(MGFramework::oneOf(w->m_Map.occupant(getTileX()+1, getTileY()), 0, getID()))
			{
				setNextXY(getTileX()+1, getTileY(), w);
				m_X += d*1.414;
			}
		}
		else if(getDestTileX()<getTileX())
		{
			if(MGFramework::oneOf(w->m_Map.occupant(getTileX()-1, getTileY()), 0, getID()))
			{
				setNextXY(getTileX()-1, getTileY(), w);
				m_X -= d*1.414;
			}
		}
		else if(getDestTileY()>getTileY())
		{
			if(MGFramework::oneOf(w->m_Map.occupant(getTileX(), getTileY()+1), 0, getID()))
			{
				setNextXY(getTileX(), getTileY()+1, w);
				m_Y += d*1.414;
			}
		}
		else if(getDestTileY()<getTileY())
		{
			if(MGFramework::oneOf(w->m_Map.occupant(getTileX(), getTileY()-1), 0, getID()))
			{
				setNextXY(getTileX(), getTileY()-1, w);
				m_Y -= d*1.414;
			}
		}
		else
		{
			MGFLOG_ERROR("MGMovingObject::update executed a case which should never happen");
			setTileXY(getTileX(), getTileY(), w);
		}

		if(m_X>=getTileSize() && m_Y>=getTileSize())
		{
			//std::cout << "A" << std::endl;
			setTileXY(getTileX()+1, getTileY()+1, w);
		}
		else if(m_X>=getTileSize() && m_Y==0)
		{
			//std::cout << "B" << std::endl;
			setTileXY(getTileX()+1, getTileY(), w);
		}
		else if(-m_X>=getTileSize() && -m_Y>=getTileSize())
		{
			//std::cout << "C" << std::endl;
			setTileXY(getTileX()-1, getTileY()-1, w);
		}
		else if(-m_X>=getTileSize() && m_Y==0)
		{
			//std::cout << "D" << std::endl;
			setTileXY(getTileX()-1, getTileY(), w);
		}
		else if(m_X>=getTileSize() && -m_Y>=getTileSize())
		{
			//std::cout << "E" << std::endl;
			setTileXY(getTileX()+1, getTileY()-1, w);
		}
		else if(m_Y>=getTileSize() && m_X==0)
		{
			//std::cout << "F" << std::endl;
			setTileXY(getTileX(), getTileY()+1, w);
		}
		else if(-m_X>=getTileSize() && m_Y>=getTileSize())
		{
			//std::cout << "G" << std::endl;
			setTileXY(getTileX()-1, getTileY()+1, w);
		}
		else if(-m_Y>=getTileSize() && m_X==0)
		{
			//std::cout << "H" << std::endl;
			setTileXY(getTileX(), getTileY()-1, w);
		}
	}

	setTimeOfLastUpdate(SDL_GetTicks());
}


void MGMovingObject::copy(MGMovingObject *src)
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
	//m_TileSize = src->m_TileSize;
	m_FinishingLastMove = src->m_FinishingLastMove;
	m_TempDestTileX = src->m_TempDestTileX;
	m_TempDestTileY = src->m_TempDestTileY;
	m_Marked = src->m_Marked;
	m_NextTileX = src->m_NextTileX;
	m_NextTileY = src->m_NextTileY;
	m_ID = src->m_ID;
	m_Owner = src->m_Owner;
	m_LoggingEnabled = src->m_LoggingEnabled;
	m_CurrentState = src->m_CurrentState;
}

bool MGMovingObject::runConsoleCommand(const char *c, MGFramework *w)
{
	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
			MGFLOG_ERROR("MGMovingObject::runConsoleCommand received MGComponent_UNDEFINED from MGMovingObject::detectMGComponentConsoleCommand"); 
			break;

		case MGComponent_MO_INT_MARK:
		{
			w->registerUsedCommand(MGComponent_MO_INT_MARK);
			if(!isMarked())
			{
				mark();
				w->countMark();
			}
			return true;
		}

		case MGComponent_MO_ALL_MARK:
		{
			w->registerUsedCommand(MGComponent_MO_ALL_MARK);
			if(!isMarked())
			{
				mark();
				w->countMark();
			}
			return true;
		}

		case MGComponent_MO_MARKED_UNMARK:
		{
			w->registerUsedCommand(MGComponent_MO_MARKED_UNMARK);
			if(isMarked())
			{
				unMark();
				w->countUnMark();
			}
			return true;
		}

		case MGComponent_MO_INT_UNMARK:
		{
			w->registerUsedCommand(MGComponent_MO_INT_UNMARK);
			if(isMarked())
			{
				unMark();
				w->countUnMark();
			}
			return true;
		}

		case MGComponent_MO_ALL_UNMARK:
		{
			w->registerUsedCommand(MGComponent_MO_ALL_UNMARK);
			if(isMarked())
			{
				unMark();
				w->countUnMark();
			}
			return true;
		}

		case MGComponent_MO_INT_GETDISTANCE:
		{
			w->registerUsedCommand(MGComponent_MO_INT_GETDISTANCE);
			std::cout << getDistance(getDestTileX(), getDestTileY()) << std::endl;
			return true;
		}

		case MGComponent_MO_INT_GETLOCATION:
		{
			w->registerUsedCommand(MGComponent_MO_INT_GETLOCATION);
			std::cout << "{" << getTileX() << "," << getTileY() << "}" << std::endl;
			return true;
		}

		case MGComponent_MO_INT_GETDESTINATION:
		{
			w->registerUsedCommand(MGComponent_MO_INT_GETDESTINATION);
			std::cout << "{" << getDestTileX() << "," << getDestTileY() << "}" << std::endl;
			return true;
		}

		case MGComponent_MO_INT_GETSPEED:
		{
			w->registerUsedCommand(MGComponent_MO_INT_GETSPEED);
			std::cout << "" << getSpeed() << std::endl;
			return true;
		}

		case MGComponent_MO_INT_HELP:
		{
			w->registerUsedCommand(MGComponent_MO_INT_HELP);
			std::cout << std::endl << "mo <i> help - Displays help information for console commands implemented" << std::endl;
			std::cout << "          in MGMovingObject. <i> is the ID of the MO." << std::endl;
			std::cout << "mo <i> getspeed - Returns the speed of the MO with ID <i>, in pixels per" << std::endl;
			std::cout << "          second." << std::endl;
			std::cout << "mo <i> getdestination - Returns the destination of the MO with ID <i>." << std::endl;
			std::cout << "mo <i> getlocation - Returns the location of the MO with ID <i>." << std::endl;
			std::cout << "mo <i> getdistance - Returns the distance to the destination of the MO with ID <i>." << std::endl;

			return true;
		}

		case MGComponent_MO_INT_SETDESTINATION_INT_INT:
		{
			w->registerUsedCommand(MGComponent_MO_INT_SETDESTINATION_INT_INT);
			int dx=w->toInt(cmdvec[3]);
			int dy=w->toInt(cmdvec[4]);
			setDestTileXY(dx, dy);
			w->m_Map.calculatePath(MGFBASICPATH1, getTileX(), getTileY(), getDestTileX(), getDestTileY());
			return true;
		}

		case MGComponent_MO_ALL_SETDESTINATION_INT_INT:
		{
			w->registerUsedCommand(MGComponent_MO_ALL_SETDESTINATION_INT_INT);
			int dx=w->toInt(cmdvec[3]);
			int dy=w->toInt(cmdvec[4]);
			setDestTileXY(dx, dy);
			w->m_Map.calculatePath(MGFSKYPATH, getTileX(), getTileY(), getDestTileX(), getDestTileY());
			return true;
		}

		case MGComponent_MO_MARKED_SETDESTINATION_INT_INT:
		{
			w->registerUsedCommand(MGComponent_MO_MARKED_SETDESTINATION_INT_INT);
			int dx=w->toInt(cmdvec[3]);
			int dy=w->toInt(cmdvec[4]);
			setDestTileXY(dx, dy);
			w->m_Map.calculatePath(MGFSKYPATH, getTileX(), getTileY(), getDestTileX(), getDestTileY());
			return true;
		}

		case MGComponent_MO_INT_LOGGING_ON:
		{
			w->registerUsedCommand(MGComponent_MO_INT_LOGGING_ON);
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			return true;
		}

		case MGComponent_MO_INT_LOGGING_OFF:
		{
			w->registerUsedCommand(MGComponent_MO_INT_LOGGING_OFF);
			MGFLOG_INFO("Logging disabled.");
			disableLogging();
			return true;
		}

		case MGComponent_MO_ALL_LOGGING_ON:
		{
			w->registerUsedCommand(MGComponent_MO_ALL_LOGGING_ON);
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			return true;
		}

		case MGComponent_MO_ALL_LOGGING_OFF:
		{
			w->registerUsedCommand(MGComponent_MO_ALL_LOGGING_OFF);
			MGFLOG_INFO("Logging disabled.");
			disableLogging();
			return true;
		}


		default:
			MGFLOG_ERROR("MGMovingObject::detectComponentConsoleCommand returned a bad value"); 
			return true;
	}

	std::cout << "Unknown command" << std::endl;
	return true;
}

eMGComponentConsoleCommand MGMovingObject::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	if(cmdvec.size() == 3)
	{
		if(cmdvec[2]=="help")
		{
			return MGComponent_MO_INT_HELP;
		}
		else if(cmdvec[2]=="getspeed")
		{
			return MGComponent_MO_INT_GETSPEED;
		}
		else if(cmdvec[2]=="getdestination")
		{
			return MGComponent_MO_INT_GETDESTINATION;
		}
		else if(cmdvec[2]=="getlocation")
		{
			return MGComponent_MO_INT_GETLOCATION;
		}
		else if(cmdvec[2]=="getdistance")
		{
			return MGComponent_MO_INT_GETDISTANCE;
		}
		else if(cmdvec[1]=="all" && cmdvec[2]=="mark")
		{
			return MGComponent_MO_ALL_MARK;
		}
		else if(cmdvec[2]=="mark")
		{
			return MGComponent_MO_INT_MARK;
		}
		else if(cmdvec[1]=="all" && cmdvec[2]=="unmark")
		{
			return MGComponent_MO_ALL_UNMARK;
		}
		else if(cmdvec[1]=="marked" && cmdvec[2]=="unmark")
		{
			return MGComponent_MO_MARKED_UNMARK;
		}
		else if(cmdvec[2]=="unmark")
		{
			return MGComponent_MO_INT_UNMARK;
		}
	}
	else if(cmdvec.size() == 4)
	{
		if(cmdvec[0]=="mo" && cmdvec[1]=="all" &&cmdvec[2]=="logging" && cmdvec[3]=="on")
		{
			return MGComponent_MO_ALL_LOGGING_ON;
		}
		else if(cmdvec[0]=="mo" && cmdvec[1]=="all" &&cmdvec[2]=="logging" && cmdvec[3]=="off")
		{
			return MGComponent_MO_ALL_LOGGING_OFF;
		}
		if(cmdvec[0]=="mo" && cmdvec[2]=="logging" && cmdvec[3]=="on")
		{
			return MGComponent_MO_INT_LOGGING_ON;
		}
		else if(cmdvec[0]=="mo" && cmdvec[2]=="logging" && cmdvec[3]=="off")
		{
			return MGComponent_MO_INT_LOGGING_OFF;
		}
	}
	else if(cmdvec.size() == 5)
	{
		if(cmdvec[1]=="all" && cmdvec[2]=="setdestination")
		{
			return MGComponent_MO_ALL_SETDESTINATION_INT_INT;
		}
		else if(cmdvec[1]=="marked" && cmdvec[2]=="setdestination")
		{
			return MGComponent_MO_MARKED_SETDESTINATION_INT_INT;
		}
		else if(cmdvec[2]=="setdestination")
		{
			return MGComponent_MO_INT_SETDESTINATION_INT_INT;
		}
	}

	return MGComponent_UNDEFINED;
}


void MGMovingObject::changeState(MOState toState)
{
	if(toState==MOStateCreated)
	{
		MGFLOG_ERROR("MGMovingObject::changeState " << toString(getCurrentState()) << "->" << toString(toState) << ", MOStateCreated should not be re-entered");
	}
	else if(getCurrentState()==MOStateCreated && toState==MOStateMoving)
	{
		MGFLOG_ERROR("MGMovingObject::changeState " << toString(getCurrentState()) << "->" << toString(toState) << ", expected MOStateIdle");
	}
	else if(getCurrentState() != toState)
	{
		MGFLOG_INFO("MGMovingObject::changeState " << toString(getCurrentState()) << "->" << toString(toState));
		m_CurrentState = toState;
	}
	else
	{
		// Keep current state...
	}
}

const char* MGMovingObject::toString(MOState s)
{
	switch(s)
	{
		case MOStateCreated:
		{
			return "MOStateCreated";
		}
		case MOStateIdle:
		{
			return "MOStateIdle";
		}
		case MOStateMoving:
		{
			return "MOStateMoving";
		}
		case MOStateStuck:
		{
			return "MOStateStuck";
		}
		default:
		{
			return "ERROR";
		}
	}
}