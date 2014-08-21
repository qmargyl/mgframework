#include "project2_server.h"
#include <iostream>
#include "mgframework/mgframework.h"


Project2Server::Project2Server()
{
	// Default parameters for demo application Project2...
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
		// Objcts such as the map are initialized here.
		m_Map.init(w, h, tw, th, m_Window.getWidth(), m_Window.getHeight()); // width (in number of tiles), height, tile width (in pixels), tile height, resolution x and y.


		// Setup game logics..

		runConsoleCommand("setfps 30", this, NULL); // Framework default is 20 FPS
		runConsoleCommand("delete all mo", this, NULL);
		runConsoleCommand("add mo 5", this, NULL);
		runConsoleCommand("open terminalserver", this, NULL);
		runConsoleCommand("delete all pe", this, NULL);
		runConsoleCommand("add pe 1", this, NULL);

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

void Project2Server::handleGameLogics()
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

void Project2Server::draw()
{
	// This function must be defined according to MGFramework class definition.
}
