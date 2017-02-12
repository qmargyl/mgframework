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
	setTimeOfLastUpdate((unsigned int)(1000.0 * (((float)clock()) / CLOCKS_PER_SEC)));
	m_FinishingMove = false;
	m_Marked = false;
	m_TileX = 0;
	m_TileY = 0;
	m_LastX = 0;
	m_LastY = 0;
	m_NextTileX = 0;
	m_NextTileY = 0;
	m_X = 0.0;
	m_Y = 0.0;
	m_VPixelsPerSecond = 0.0;
	m_CurrentState = MOStateCreated;
	m_PathFindingAlgorithm = MGFBASICPATH1;
	setOwner(MGF_NOPLAYER);
}

MGMovingObject::~MGMovingObject()
{
	if(isMoving())
	{
		m_MovingMOCounter--;
	}
}

void MGMovingObject::initialize()
{
	setTimeOfLastUpdate((unsigned int)(1000.0 * (((float)clock()) / CLOCKS_PER_SEC)));
	m_FinishingMove = false;
	m_Marked = false;
	m_TileX = 0;
	m_TileY = 0;
	m_NextTileX = 0;
	m_NextTileY = 0;
	m_X = 0.0;
	m_Y = 0.0;
	m_PathFindingAlgorithm = MGFBASICPATH1;
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
	m_FinishingMove = false;
	if(getNextTileX() == getTileX() && getNextTileY() == getTileY() && isMoving() && m_Path.size() == 1)
	{
		// Final destination reached
		addToHistory(	(std::string("FinalDestination: ") + MGComponent::toString(getTileX()) + 
						 std::string(",") + MGComponent::toString(getTileY()) ).c_str());
		changeState(MOStateIdle);
	}
}

void MGMovingObject::setNextTileXY(int x, int y, MGFramework *world)
{
	if(world->m_Map.occupant(x, y) != getID() && world->m_Map.occupant(x, y) != 0)
	{
		// Something in the way
	}
	else
	{
		if(m_X != 0.0 || m_Y != 0.0 || m_FinishingMove)
		{
			// Handle an ongoing step from one tile to another
			m_TempNextTileX = m_NextTileX;
			m_TempNextTileY = m_NextTileY;
			m_FinishingMove = true;
		}
		world->m_Map.unOccupy(m_NextTileX, m_NextTileY);
		m_NextTileX = x;
		m_NextTileY = y;
		world->m_Map.occupy(m_NextTileX, m_NextTileY, getID());
	}
}


void MGMovingObject::setPath(std::list<MGPathItem> p)
{
	m_Path = p;
}


double MGMovingObject::getDistance(int wx, int wy)
{ 
	return MGComponent::distance(getTileX(), getTileY(), wx, wy);
}

void MGMovingObject::setSpeed(double s, int tileSize)
{
	if(s > 0.0 && tileSize > 0)
	{
		m_VPixelsPerSecond = static_cast<double>(tileSize) / s;
		m_TileSize = tileSize;
	}
	else
	{
		m_VPixelsPerSecond = 0.0;
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
		unsigned int currentTimeSinceLastUpdate = w->getWindow()->getExecTimeMS() - getTimeOfLastUpdate();

		int x = 0;
		int y = 0;
		if (m_X == 0 && m_Y == 0)
		{
			MGPathItem pathI = m_Path.front();
			x = pathI.getX();
			y = pathI.getY();
			m_LastX = x;
			m_LastY = y;
		}
		else
		{
			x = m_LastX;
			y = m_LastY;
		}

		// Something has moved into the MO's path. A new path needs to be calculated.
		if(w->m_Map.occupant(x, y) != getID() && w->m_Map.occupant(x, y) != 0)
		{
			// Try to find a new path to the last element in the path.
			if(!isStuck()/* || (isStuck() && timeSinceLastUpdate > 5000)*/)
			{
				//MGFLOG_WARNING("MGMovingObject::update concluded that the path is blocked. Will try to find a new Path...");
				setPath(w->m_Map.calculatePath(m_PathFindingAlgorithm, getTileX(), getTileY(), m_Path.back().getX(), m_Path.back().getY()));
				setTimeOfLastUpdate(w->getWindow()->getExecTimeMS());
				changeState(MOStateStuck);
				return;
			}
			else if(isStuck() && currentTimeSinceLastUpdate > 10000)
			{
				addToHistory("Stuck for too long");
				MGFLOG_WARNING("MGMovingObject::update: MO stuck for too long");
				changeState(MOStateIdle);
				m_Path.clear();
				return;
			}
		}
		else
		{
			setNextTileXY(x, y, w);
			setTimeOfLastUpdate(w->getWindow()->getExecTimeMS());
			if(isStuck())
			{
				changeState(MOStateMoving);
			}
		}

		// Not stuck and not at destination tile
		if(!isStuck() && (getNextTileX() != getTileX() || getNextTileY() != getTileY()))
		{
			if(!isMoving())
			{
				changeState(MOStateMoving);
			}

			int dx = 0;
			int dy = 0;

			if(getNextTileX() > getTileX())
			{
				dx = 1;
			}
			else if(getNextTileX() < getTileX())
			{
				dx = -1;
			}
			if(getNextTileY() > getTileY())
			{
				dy = 1;
			}
			else if(getNextTileY() < getTileY())
			{
				dy = -1;
			}
			if(dx != 0 || dy != 0)
			{
				if(MGFramework::oneOf(w->m_Map.occupant(getNextTileX(), getNextTileY()), 0, getID()))
				{
					double d = getSpeed() * (currentTimeSinceLastUpdate / 1000.0);
					if(dx != 0 && dy != 0)
					{
						m_X += (d * (double)dx);
						m_Y += (d * (double)dy);
					}
					else
					{
						m_X += (1.414 * d * (double)dx);
						m_Y += (1.414 * d * (double)dy);
					}
				}
			}

			dx = 0;
			dy = 0;

			if(m_X >= getTileSize())
			{
				dx = 1;
			}
			else if(-m_X >= getTileSize())
			{
				dx = -1;
			}
			if(m_Y >= getTileSize())
			{
				dy = 1;
			}
			else if(-m_Y >= getTileSize())
			{
				dy = -1;
			}
			if((dx != 0 && dy != 0) || (dy != 0 && m_X == 0) || (dx != 0 && m_Y == 0))
			{
				setTileXY(getNextTileX(), getNextTileY(), w);
			}
			else if(dx != 0)
			{
				m_X = dx * getTileSize();
			}
			else if(dy != 0)
			{
				m_Y = dy * getTileSize();
			}
			setTimeOfLastUpdate(w->getWindow()->getExecTimeMS());
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
			if(!isMarked())
			{
				mark();
				w->countMark();
			}
			return true;
		}

		case MGComponent_MO_INT_UNMARK:
		{
			if(isMarked())
			{
				unMark();
				w->countUnMark();
			}
			return true;
		}

		case MGComponent_MO_INT_GETLOCATION:
		{
			MGFLOG_INFO("{" << getTileX() << "," << getTileY() << "}");
			return true;
		}

		case MGComponent_MO_INT_EXPECT_GETLOCATION_INT_INT:
		{
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

		case MGComponent_MO_INT_GETSPEED:
		{
			w->setCommandReturnValue((int)getSpeed());
			std::cout << "" << (int)getSpeed() << std::endl;
			return true;
		}

		case MGComponent_MO_INT_HELP:
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

		case MGComponent_MO_INT_SETDESTINATION_INT_INT_PARAMLIST:
		{
			int dx = w->toInt(cmdvec[3], s);
			int dy = w->toInt(cmdvec[4], s);

			for(unsigned int i = 5; i < cmdvec.size(); ++i)
			{
				MGFLOG_INFO("Set destination (param): " << cmdvec[i] );
				if(cmdvec[i] == "-astar")
				{
					m_PathFindingAlgorithm = MGFASTARLIST;
				}
				else if(cmdvec[i] == "-basicpath")
				{
					m_PathFindingAlgorithm = MGFBASICPATH1;
				}
				else
				{
					MGFLOG_ERROR("Error in command (mo <int> setdestination <int> <int>), bad parameter list");
				}
			}

			addToHistory(	(std::string("CalculatePath: ") + MGComponent::toString(getTileX()) + 
							 std::string(",") + MGComponent::toString(getTileY()) + std::string(" -> ") +
							 MGComponent::toString(dx) + std::string(",") + MGComponent::toString(dy)).c_str());
			// Starting from NextTile allows finishing the ongoing step before starting to follow a new path.
			setPath(w->m_Map.calculatePath(m_PathFindingAlgorithm, getNextTileX(), getNextTileY(), dx, dy));
			MGFLOG_INFO("Path length: " << m_Path.size());
			return true;
		}

		case MGComponent_MO_INT_LOGGING_ON:
		{
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			addToHistory("Logging enabled.");
			return true;
		}

		case MGComponent_MO_INT_LOGGING_OFF:
		{
			MGFLOG_INFO("Logging disabled.");
			addToHistory("Logging disabled.");
			disableLogging();
			return true;
		}

		case MGComponent_MO_INT_HISTORY_BOOL:
		{
			bool inputOn = w->toBool(cmdvec[3], s);
			if(inputOn)
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

eMGComponentConsoleCommand MGMovingObject::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) const
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
	else if(cmdvec.size() >= 5)
	{
		if(cmdvec[2] == "setdestination")
		{
			return MGComponent_MO_INT_SETDESTINATION_INT_INT_PARAMLIST;
		}
		else if(cmdvec[2] == "expect" && cmdvec[3] == "getlocation")
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

const char* MGMovingObject::toString(MOState s) const
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