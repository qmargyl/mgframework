#include "mgframework.h"
#include "mgstationaryobject.h"
#include <iostream>

int MGStationaryObject::m_TileSize = 0;

MGStationaryObject::MGStationaryObject()
{
	m_TileX=0;
	m_TileY=0;
	setOwner(MGF_NOPLAYER);
}

MGStationaryObject::~MGStationaryObject()
{

}

void MGStationaryObject::initialize()
{
	m_TileX=0;
	m_TileY=0;
	setOwner(MGF_NOPLAYER);
}

void MGStationaryObject::setTileXY(int x, int y, MGFramework *world)
{
	world->m_Map.unOccupy(getTileX(), getTileY());
	m_TileX=x;
	m_TileY=y;
	world->m_Map.occupy(getTileX(), getTileY(), getID());
}

double MGStationaryObject::getDistance(int wx, int wy)
{ 
	return MGFramework::distance(getTileX(), getTileY(), wx, wy);
}



void MGStationaryObject::copy(MGStationaryObject *src)
{
	//Copy ALL class variables!
	m_TileX = src->m_TileX;
	m_TileY = src->m_TileY;
	m_ID = src->m_ID;
	m_Owner = src->m_Owner;
	m_LoggingEnabled = src->m_LoggingEnabled;
}

bool MGStationaryObject::runConsoleCommand(const char *c, MGFramework *w)
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
			w->registerUsedCommand(MGComponent_SO_INT_GETLOCATION);
			std::cout << "{" << getTileX() << "," << getTileY() << "}" << std::endl;
			return true;
		}

		case MGComponent_SO_INT_HELP:
		{
			w->registerUsedCommand(MGComponent_SO_INT_HELP);
			return true;
		}

		case MGComponent_SO_INT_LOGGING_ON:
		{
			w->registerUsedCommand(MGComponent_SO_INT_LOGGING_ON);
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			return true;
		}

		case MGComponent_SO_INT_LOGGING_OFF:
		{
			w->registerUsedCommand(MGComponent_SO_INT_LOGGING_OFF);
			MGFLOG_INFO("Logging disabled.");
			disableLogging();
			return true;
		}

		case MGComponent_SO_ALL_LOGGING_ON:
		{
			w->registerUsedCommand(MGComponent_SO_ALL_LOGGING_ON);
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			return true;
		}

		case MGComponent_SO_ALL_LOGGING_OFF:
		{
			w->registerUsedCommand(MGComponent_SO_ALL_LOGGING_OFF);
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

eMGComponentConsoleCommand MGStationaryObject::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{

	if(cmdvec.size() == 3)
	{
		if(cmdvec[2]=="getlocation")
		{
			return MGComponent_SO_INT_GETLOCATION;
		}
		if(cmdvec[2]=="help")
		{
			return MGComponent_SO_INT_HELP;
		}
	}
	else if(cmdvec.size() == 4)
	{
		if(cmdvec[1]=="all" && cmdvec[2]=="logging" && cmdvec[3]=="on")
		{
			return MGComponent_SO_ALL_LOGGING_ON;
		}
		else if(cmdvec[2]=="logging" && cmdvec[3]=="on")
		{
			return MGComponent_SO_INT_LOGGING_ON;
		}	
		else if(cmdvec[1]=="all" && cmdvec[2]=="logging" && cmdvec[3]=="off")
		{
			return MGComponent_SO_ALL_LOGGING_OFF;
		}
		else if(cmdvec[2]=="logging" && cmdvec[3]=="off")
		{
			return MGComponent_SO_INT_LOGGING_OFF;
		}	
	}

	return MGComponent_UNDEFINED;
}


