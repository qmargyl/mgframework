#include "project2_server.h"
#include <iostream>
#include "mgframework/mgframework.h"


Project2Server::Project2Server()
{
	unsetWindowProperties(); // Force setWindowProperties to be called before init.
	disableTyping();
	setInstanceType(MGFSERVERINSTANCE);
	disableFeatureMiniMap();
	enableLogging();
}

bool Project2Server::init(int w, int h, int tw, int th)
{
	// The window is created.
	if (windowPropertiesSet())
	{
		// Objects such as the map are initialized here.
		m_Map.init(w, h, tw, th, getWindow()->getWidth(), getWindow()->getHeight()); // width (in number of tiles), height, tile width (in pixels), tile height, resolution x and y.

		// Open terminal server for incoming connection requests
		setPort(666);
		runConsoleCommand("open terminalserver", this, NULL);
		
		// Print a status update to the console periodically
		runConsoleCommand("add pe 1", this, NULL);
		m_PE.begin()->setupTimer(10000);
		m_PE.begin()->activate();

		return true;
	}
	else
	{
		return false;
	}
}

void Project2Server::handleGameLogics()
{
	// Update periodic event to trigger rare events
	if(getNumberOfPE() > 0)
	{
		if(m_PE.begin()->update())
		{
			std::cout << "Status: nMO=" << getNumberOfMO() << std::endl;
		}
	}

	// Update all moving objects
	for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
	{
		it->update(this);
	}

}

void Project2Server::draw()
{
	// This function must be defined according to MGFramework class definition.
}
