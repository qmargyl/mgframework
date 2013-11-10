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
		runConsoleCommand("create pe 1", this);

		if(getNumberOfPE()>0)
		{
			m_PE[0].setupTimer(4000);
			m_PE[0].activate();
		}

		return true;
	}
	else
	{
		return false;
	}
}

void BotWarsServer::handleGameLogics()
{
	// Update periodic event to trigger rare events
	if(getNumberOfPE()>0)
	{
		if(m_PE[0].update())
		{
			// Set all moving objects destination coordinate.
			for(int i=0;i<getNumberOfMO();i++)
			{
				m_MO[i].setDestTileXY(randomN(m_Map.getWidth()), randomN(m_Map.getHeight()));
			}
		}
	}

	// Update all moving objects
	for(int i=0;i<getNumberOfMO();i++)
	{
		m_MO[i].update(this);
	}

}

void BotWarsServer::draw()
{
	// This function must be defined according to MGFramework class definition.
}
