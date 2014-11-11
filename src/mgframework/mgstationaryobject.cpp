#include "mgframework.h"
#include "mgstationaryobject.h"
#include <iostream>
#include <cstring>

MGStationaryObject::MGStationaryObject()
:	m_TileX(0),
	m_TileY(0),
	m_XOff(0),
	m_YOff(0),
	m_Type(0),
	m_texHandle(NULL)
{
	setOwner(MGF_NOPLAYER);
}

MGStationaryObject::~MGStationaryObject()
{

}

bool operator<(MGStationaryObject &lhs, MGStationaryObject &rhs)
{
	// This is used for sorting stationary objects only. They are
	// sorted by vertical location on the screen to be drawn in the
	// correct order.
	if(lhs.m_TileY < rhs.m_TileY)
	{
		return true;
	}
	else if(lhs.m_TileY == rhs.m_TileY)
	{
		return lhs.m_YOff < rhs.m_YOff;
	}
	return false;
}


void MGStationaryObject::setTileXY(int x, int y, MGFramework *world, bool occupyMap)
{
	if(occupyMap && world)
	{
		world->m_Map.unOccupy(getTileX(), getTileY());
		if(world->isSelectiveTileRenderingActive())
		{
			world->m_Map.markForRendering(getTileX(), getTileY());
		}
	}
	m_TileX = x;
	m_TileY = y;
	if(occupyMap && world)
	{
		world->m_Map.occupy(getTileX(), getTileY(), getID());
		if(world->isSelectiveTileRenderingActive())
		{
			world->m_Map.markForRendering(getTileX(), getTileY());
		}
	}
}


bool MGStationaryObject::runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s)
{
	char cmd[MGF_SCRIPTLINE_MAXLENGTH];
	strcpy(cmd, c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, " ");

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
			MGFLOG_ERROR("MGStationaryObject::runConsoleCommand received MGComponent_UNDEFINED from MGStationaryObject::detectMGComponentConsoleCommand"); 
			break;

		case MGComponent_SO_INT_GETLOCATION:
		{
			std::cout << "{" << getTileX() << "," << getTileY() << "}" << std::endl;
			return true;
		}

		case MGComponent_SO_INT_LOGGING_ON:
		{
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			return true;
		}

		case MGComponent_SO_INT_LOGGING_OFF:
		{
			MGFLOG_INFO("Logging disabled.");
			disableLogging();
			return true;
		}

		case MGComponent_SO_ALL_LOGGING_ON:
		{
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			return true;
		}

		case MGComponent_SO_ALL_LOGGING_OFF:
		{
			MGFLOG_INFO("Logging disabled.");
			disableLogging();
			return true;
		}


		default:
			MGFLOG_ERROR("MGStationaryObject::detectComponentConsoleCommand returned a bad value"); 
			return true;
	}

	std::cout << "Unknown command" << std::endl;
	return true;
}

eMGComponentConsoleCommand MGStationaryObject::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) const
{

	if(cmdvec.size() == 3)
	{
		if(cmdvec[2] == "getlocation")
		{
			return MGComponent_SO_INT_GETLOCATION;
		}
	}
	else if(cmdvec.size() == 4)
	{
		if(cmdvec[1] == "all" && cmdvec[2] == "logging" && cmdvec[3] == "on")
		{
			return MGComponent_SO_ALL_LOGGING_ON;
		}
		else if(cmdvec[2] == "logging" && cmdvec[3] == "on")
		{
			return MGComponent_SO_INT_LOGGING_ON;
		}	
		else if(cmdvec[1] == "all" && cmdvec[2] == "logging" && cmdvec[3] == "off")
		{
			return MGComponent_SO_ALL_LOGGING_OFF;
		}
		else if(cmdvec[2] == "logging" && cmdvec[3] == "off")
		{
			return MGComponent_SO_INT_LOGGING_OFF;
		}	
	}

	return MGComponent_UNDEFINED;
}
