#include "mgframework.h"
#include "mgmovingobject.h"
#include "mgpathitem.h"
#include <iostream>
#include <cstring>

int MGMovingObject::m_TileSize = 0;
int MGMovingObject::m_MovingMOCounter = 0;

MGMovingObject::MGMovingObject()
{
	m_HistoryEnabled = false;
	setTimeOfLastUpdate(MGF_GetExecTimeMS());
	m_FinishingLastMove = false;
	m_Marked = false;
	m_TileX = 0;
	m_TileY = 0;
	m_DestTileX = 0;
	m_DestTileY = 0;
	m_NextTileX = 0;
	m_NextTileY = 0;
	m_X = 0.0;
	m_Y = 0.0;
	m_CurrentState = MOStateCreated;
	setOwner(MGF_NOPLAYER);
}

MGMovingObject::~MGMovingObject()
{
}

void MGMovingObject::initialize()
{
	setTimeOfLastUpdate(MGF_GetExecTimeMS());
	m_FinishingLastMove = false;
	m_Marked = false;
	m_TileX = 0;
	m_TileY = 0;
	m_DestTileX = 0;
	m_DestTileY = 0;
	m_NextTileX = 0;
	m_NextTileY = 0;
	m_X = 0.0;
	m_Y = 0.0;
	setOwner(MGF_NOPLAYER);
	if(!isCreated())
	{
		MGFLOG_ERROR("MGMovingObject::initialize was called in the wrong state: " << toString(getCurrentState())); 
	}
	changeState(MOStateIdle);
}

void MGMovingObject::setTileXY(int x, int y, MGFramework *world)
{
	world->m_Map.unOccupy(getTileX(), getTileY());
	if(world->isSelectiveTileRenderingActive()) world->m_Map.markForRendering(getTileX(), getTileY());
	m_TileX = x;
	m_TileY = y;
	m_X = 0.0;
	m_Y = 0.0;
	m_FinishingLastMove = false;
	if(getDestTileX() == getTileX() && getDestTileY() == getTileY() && isMoving() && m_Path.size() == 1)
	{
		// Final destination reached
		addToHistory(	(std::string("FinalDestination: ") + MGComponent::toString(getTileX()) + 
						 std::string(",") + MGComponent::toString(getTileY()) ).c_str());
		changeState(MOStateIdle);
	}
}

void MGMovingObject::setNextXY(int x, int y, MGFramework *world)
{
	if(world->m_Map.occupant(x, y) == getID())
	{

	}
	else if(MGFramework::distance(getTileX(), getTileY(), x, y) >= 1.5)
	{
		MGFLOG_ERROR("MGMovingObject::setNextXY detected an incorrect step size"); 
	}
	else if(world->m_Map.occupant(x, y) == 0)
	{
		if(	world->m_Map.occupant(m_NextTileX, m_NextTileY) == getID() ||
			world->m_Map.occupant(m_NextTileX, m_NextTileY) == 0)
		{
			world->m_Map.unOccupy(m_NextTileX, m_NextTileY);
			if(world->isSelectiveTileRenderingActive()) world->m_Map.markForRendering(m_NextTileX, m_NextTileY);
		}
		else
		{
			MGFLOG_WARNING("MGMovingObject::setNextXY tried to leave a tile not occupied by MO");
		}
		world->m_Map.occupy(x, y, getID());
		if(world->isSelectiveTileRenderingActive()) world->m_Map.markForRendering(x, y);
		m_NextTileX = x; 
		m_NextTileY = y;
	}
	else
	{
		MGFLOG_ERROR("MGMovingObject::setNextXY tried to occupy an occupied tile"); 
	}
}

void MGMovingObject::setDestTileXY(int x, int y)
{
	// If we are still finishing our last move, save current 
	// destination to temp variable and then set new destination
	if(m_X != 0.0 || m_Y != 0.0 || m_FinishingLastMove)
	{
		m_TempDestTileX = m_DestTileX;
		m_TempDestTileY = m_DestTileY;
		m_FinishingLastMove = true;
	}
	m_DestTileX = x;
	m_DestTileY = y;
}


void MGMovingObject::setPath(std::list<MGPathItem> p)
{
	m_Path = p;
	if(!m_Path.empty())
	{
		setDestTileXY(m_Path.front().getX(), m_Path.front().getY());
	}
}


double MGMovingObject::getDistance(int wx, int wy)
{ 
	return MGComponent::distance(getTileX(), getTileY(), wx, wy);
}

void MGMovingObject::setSpeed(double s, int tileSize)
{
	if(s > (double)0 && tileSize > 0)
	{
		m_Speed = (double)tileSize / s;
		m_TileSize = tileSize;
	}
	else
	{
		m_Speed = (double)0;
		m_TileSize = 0;
	}
}

void MGMovingObject::update(MGFramework *w)
{
	if(m_Path.empty())
	{
		changeState(MOStateIdle);
		return;
	}
	else
	{
		int timeSinceLastUpdate = MGF_GetExecTimeMS() - getTimeOfLastUpdate();
		double d = m_Speed * (timeSinceLastUpdate / 1000.0);

		MGPathItem pathI = m_Path.front();
		int x = pathI.getX();
		int y = pathI.getY();

		// Something has moved into the MO's path. A new path needs to be calculated.
		if(w->m_Map.occupant(x, y) != getID() && w->m_Map.occupant(x, y) != 0)
		{
			// Try to find a new path to the last element in the path.
			if(!isStuck()/* || (isStuck() && timeSinceLastUpdate > 5000)*/)
			{
				//MGFLOG_WARNING("MGMovingObject::update concluded that the path is blocked. Will try to find a new Path...");
				setPath(w->m_Map.calculatePath(MGFBASICPATH1, getTileX(), getTileY(), m_Path.back().getX(), m_Path.back().getY()));
				setTimeOfLastUpdate(MGF_GetExecTimeMS());
				changeState(MOStateStuck);
				return;
			}
			else if(isStuck() && timeSinceLastUpdate > 10000)
			{
				setPath(w->m_Map.calculatePath(MGFBASICPATH1, getTileX(), getTileY(), m_Path.back().getX(), m_Path.back().getY()));
				setTimeOfLastUpdate(MGF_GetExecTimeMS());
				addToHistory("Stuck for too long");
				MGFLOG_WARNING("MGMovingObject::update: MO stuck for too long");
				changeState(MOStateIdle);
				m_Path.clear();
				return;
			}

		}
		else
		{
			setDestTileXY(x, y);
			setTimeOfLastUpdate(MGF_GetExecTimeMS());
			if(isStuck())
			{
				changeState(MOStateMoving);
			}
		}

		// Not stuck and not at destination tile
		if(!isStuck() && (getDestTileX() != getTileX() || getDestTileY() != getTileY()))
		{
			if(!isMoving())
			{
				changeState(MOStateMoving);
			}

			if(getDestTileX() > getTileX() && getDestTileY() > getTileY())
			{
				if(MGFramework::oneOf(w->m_Map.occupant(getTileX() + 1, getTileY() + 1), 0, getID()))
				{
					setNextXY(getTileX() + 1, getTileY() + 1, w);
					m_X += d;
					m_Y += d;
				}
			}
			else if(getDestTileX() < getTileX() && getDestTileY() < getTileY())
			{
				if(MGFramework::oneOf(w->m_Map.occupant(getTileX() - 1, getTileY() - 1), 0, getID()))
				{
					setNextXY(getTileX() - 1, getTileY() - 1, w);
					m_X -= d;
					m_Y -= d;
				}
			}
			else if(getDestTileX() > getTileX() && getDestTileY() < getTileY())
			{
				if(MGFramework::oneOf(w->m_Map.occupant(getTileX() + 1, getTileY() - 1), 0, getID()))
				{
					setNextXY(getTileX() + 1, getTileY() - 1, w);
					m_X += d;
					m_Y -= d;
				}
			}
			else if(getDestTileX() < getTileX() && getDestTileY() > getTileY())
			{
				if(MGFramework::oneOf(w->m_Map.occupant(getTileX() - 1, getTileY() + 1), 0, getID()))
				{
					setNextXY(getTileX() - 1, getTileY() + 1, w);
					m_X -= d;
					m_Y += d;
				}
			}
			else if(getDestTileX() > getTileX())
			{
				if(MGFramework::oneOf(w->m_Map.occupant(getTileX() + 1, getTileY()), 0, getID()))
				{
					setNextXY(getTileX() + 1, getTileY(), w);
					m_X += d * 1.414;
				}
			}
			else if(getDestTileX() < getTileX())
			{
				if(MGFramework::oneOf(w->m_Map.occupant(getTileX() - 1, getTileY()), 0, getID()))
				{
					setNextXY(getTileX() - 1, getTileY(), w);
					m_X -= d * 1.414;
				}
			}
			else if(getDestTileY() > getTileY())
			{
				if(MGFramework::oneOf(w->m_Map.occupant(getTileX(), getTileY() + 1), 0, getID()))
				{
					setNextXY(getTileX(), getTileY() + 1, w);
					m_Y += d * 1.414;
				}
			}
			else if(getDestTileY() < getTileY())
			{
				if(MGFramework::oneOf(w->m_Map.occupant(getTileX(), getTileY() - 1), 0, getID()))
				{
					setNextXY(getTileX(), getTileY() - 1, w);
					m_Y -= d * 1.414;
				}
			}
			else
			{
				MGFLOG_ERROR("MGMovingObject::update executed a case which should never happen");
				setTileXY(getTileX(), getTileY(), w);
			}

			if(m_X >= getTileSize() && m_Y >= getTileSize())
			{
				setTileXY(getTileX() + 1, getTileY() + 1, w);
			}
			else if(m_X >= getTileSize() && m_Y == 0)
			{
				setTileXY(getTileX() + 1, getTileY(), w);
			}
			else if(-m_X >= getTileSize() && -m_Y >= getTileSize())
			{
				setTileXY(getTileX() - 1, getTileY() - 1, w);
			}
			else if(-m_X >= getTileSize() && m_Y == 0)
			{
				setTileXY(getTileX() - 1, getTileY(), w);
			}
			else if(m_X >= getTileSize() && -m_Y >= getTileSize())
			{
				setTileXY(getTileX() + 1, getTileY() - 1, w);
			}
			else if(m_Y >= getTileSize() && m_X == 0)
			{
				setTileXY(getTileX(), getTileY() + 1, w);
			}
			else if(-m_X >= getTileSize() && m_Y >= getTileSize())
			{
				setTileXY(getTileX() - 1, getTileY() + 1, w);
			}
			else if(-m_Y >= getTileSize() && m_X == 0)
			{
				setTileXY(getTileX(), getTileY() - 1, w);
			}

			setTimeOfLastUpdate(MGF_GetExecTimeMS());
		}
		else if(!isStuck())
		{
			// Arrived at destination tile, set a new one from the path
			if(!m_Path.empty())
			{
				m_Path.pop_front();
			}
		}
		else
		{
			// Do nothing?
		}
	}
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
	m_Path = src->m_Path;
}

bool MGMovingObject::runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s)
{
	char cmd[MGF_SCRIPTLINE_MAXLENGTH];
	strcpy(cmd, c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, " ");

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

		case MGComponent_MO_INT_GETLOCATION:
		{
			w->registerUsedCommand(MGComponent_MO_INT_GETLOCATION);
			MGFLOG_INFO("{" << getTileX() << "," << getTileY() << "}");
			return true;
		}

		case MGComponent_MO_INT_EXPECT_GETLOCATION_INT_INT:
		{
			w->registerUsedCommand(MGComponent_MO_INT_EXPECT_GETLOCATION_INT_INT);
			int lx = w->toInt(cmdvec[4], s);
			int ly = w->toInt(cmdvec[5], s);
			if(lx == getTileX() && ly == getTileY())
			{
				MGFLOG_INFO("Expected location confirmed: (" << lx << ", " << ly << ")");
			}
			else
			{
				MGFLOG_ERROR("Expected location not confirmed: (" << lx << ", " << ly << "), MO at (" << getTileX() << ", " << getTileY() << ")");
			}
		}

		case MGComponent_MO_INT_GETDESTINATION:
		{
			// XXX: This is not really relevant anymore since paths are used now instead..
			w->registerUsedCommand(MGComponent_MO_INT_GETDESTINATION);
			MGFLOG_INFO("{" << getDestTileX() << "," << getDestTileY() << "}");
			return true;
		}

		case MGComponent_MO_INT_GETSPEED:
		{
			w->registerUsedCommand(MGComponent_MO_INT_GETSPEED, (int)getSpeed());
			std::cout << "" << (int)getSpeed() << std::endl;
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
			int dx = w->toInt(cmdvec[3], s);
			int dy = w->toInt(cmdvec[4], s);
			addToHistory(	(std::string("CalculatePath: ") + MGComponent::toString(getTileX()) + 
							 std::string(",") + MGComponent::toString(getTileY()) + std::string(" -> ") +
							 MGComponent::toString(dx) + std::string(",") + MGComponent::toString(dy)).c_str());
			setPath(w->m_Map.calculatePath(MGFBASICPATH1, getTileX(), getTileY(), dx, dy));
			MGFLOG_INFO("Path length: " << m_Path.size());
			return true;
		}

		case MGComponent_MO_INT_LOGGING_ON:
		{
			w->registerUsedCommand(MGComponent_MO_INT_LOGGING_ON);
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			addToHistory("Logging enabled.");
			return true;
		}

		case MGComponent_MO_INT_LOGGING_OFF:
		{
			w->registerUsedCommand(MGComponent_MO_INT_LOGGING_OFF);
			MGFLOG_INFO("Logging disabled.");
			addToHistory("Logging disabled.");
			disableLogging();
			return true;
		}

		case MGComponent_MO_INT_HISTORY_BOOL:
		{
			w->registerUsedCommand(MGComponent_MO_INT_HISTORY_BOOL);
			int inputOn = w->toInt(cmdvec[3], s);
			if(inputOn == MGF_TRUE)
			{
				enableHistory();
			}
			else
			{
				printHistory();
				disableHistory();
			}
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
		if(cmdvec[2] == "help")
		{
			return MGComponent_MO_INT_HELP;
		}
		else if(cmdvec[2] == "getspeed")
		{
			return MGComponent_MO_INT_GETSPEED;
		}
		else if(cmdvec[2] == "getdestination")
		{
			return MGComponent_MO_INT_GETDESTINATION;
		}
		else if(cmdvec[2] == "getlocation")
		{
			return MGComponent_MO_INT_GETLOCATION;
		}
		else if(cmdvec[2] == "mark")
		{
			return MGComponent_MO_INT_MARK;
		}
		else if(cmdvec[2] == "unmark")
		{
			return MGComponent_MO_INT_UNMARK;
		}
	}
	else if(cmdvec.size() == 4)
	{
		if(cmdvec[0] == "mo" && cmdvec[2] == "logging" && cmdvec[3] == "on")
		{
			return MGComponent_MO_INT_LOGGING_ON;
		}
		else if(cmdvec[0] == "mo" && cmdvec[2] == "logging" && cmdvec[3] == "off")
		{
			return MGComponent_MO_INT_LOGGING_OFF;
		}
		else if(cmdvec[0] == "mo" && cmdvec[2] == "history")
		{
			return MGComponent_MO_INT_HISTORY_BOOL;
		}
	}
	else if(cmdvec.size() == 5)
	{
		if(cmdvec[2] == "setdestination")
		{
			return MGComponent_MO_INT_SETDESTINATION_INT_INT;
		}
	}
	else if(cmdvec.size() == 6)
	{
		if(cmdvec[2] == "expect" && cmdvec[3] == "getlocation")
		{
			return MGComponent_MO_INT_EXPECT_GETLOCATION_INT_INT;
		}
	}

	return MGComponent_UNDEFINED;
}


void MGMovingObject::changeState(MOState toState)
{
	// XXX: This function should also update statistics counters?
	if(toState == MOStateCreated)
	{
		MGFLOG_ERROR("MGMovingObject::changeState " << toString(getCurrentState()) << "->" << toString(toState) << ", MOStateCreated should not be re-entered");
	}
	else if(getCurrentState() == MOStateCreated && toState == MOStateMoving)
	{
		MGFLOG_ERROR("MGMovingObject::changeState " << toString(getCurrentState()) << "->" << toString(toState) << ", expected MOStateIdle");
	}
	else if(getCurrentState() != toState)
	{
		MGFLOG_INFO("MGMovingObject::changeState " << toString(getCurrentState()) << "->" << toString(toState));

		if(toState == MOStateMoving)
		{
			++m_MovingMOCounter;
		}
		else if(m_CurrentState == MOStateMoving)
		{
			--m_MovingMOCounter;
		}

		addToHistory((std::string("ChangeState: ") + toString(getCurrentState()) + std::string(" -> ") + toString(toState)).c_str());
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

void MGMovingObject::printHistory()
{
	if(m_HistoryEnabled)
	{
		std::cout << "MGMovingObject ID " << getID() << " History" << std::endl;
		//TODO: Loop from size to zero instead, and print the history reversed.
		for(unsigned int i = 0; i < m_History.size(); ++i)
		{
			std::cout << i << "\t" << m_History.at(i).c_str() << std::endl;
		}
	}
}

void MGMovingObject::addToHistory(const char *str)
{
	if(m_HistoryEnabled)
	{
		m_History.push_back(MGComponent::toString((int)0) + std::string(": ") + std::string(str));
	}
}