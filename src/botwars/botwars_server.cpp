#include "botwars_server.h"
#include <iostream>
#include "../mgframework/mgframework.h"
#include "../mgframework/mgmovingobject.h"


BotWarsServer::BotWarsServer()
{
	unsetWindowProperties(); // Force setWindowProperties to be called before init.
	disableTyping();
	setInstanceType(MGFSERVERINSTANCE);
	disableMiniMap();
	enableLogging();

	for(int i = 0; i < MAXNUMBEROFCLIENTS; ++i)
	{
		clients[i].active=false;
		sprintf(clients[i].ip, "");
		clients[i].port=0;
	}
}

bool BotWarsServer::init(int w, int h, int tw, int th)
{
	// The window is created.
	if (windowPropertiesSet())
	{
		// Objcts such as the map are initialized here.
		m_Map.init(w, h, tw, th, m_Window.getWidth(), m_Window.getHeight()); // width (in number of tiles), height, tile width (in pixels), tile height, resolution x and y.


		// Setup game logics..

		runConsoleCommand("setfps 30", this); // Framework default is 20 FPS
		runConsoleCommand("create mo 5", this);
		runConsoleCommand("open terminalserver", this);

		return true;
	}
	else
	{
		return false;
	}
}

void BotWarsServer::handleGameLogics()
{
	for(int i=0; i<MAXNUMBEROFCLIENTS; ++i)
	{
		if(clients[i].active)
		{
			
		}
	}

	// Update periodic event to trigger rare events
	if(getNumberOfPE()>0)
	{
		if(m_PE[0].update())
		{

		}
	}

	// Update all moving objects
	for(int i=0; i<getNumberOfMO(); ++i)
	{
		m_MO[i].update(this);
	}

}

void BotWarsServer::draw()
{
	// This function must be defined according to MGFramework class definition.
}
