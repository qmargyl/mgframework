#include "botwars.h"
#include <iostream>
#include "../mgframework/mgframework.h"
#include "../mgframework/mgmovingobject.h"

BotWars::BotWars()
: m_NBots(0)
{
	// Default parameters for demo application Project2...
	unsetWindowProperties(); // Force setWindowProperties to be called before init.
	disableTyping();
	setInstanceType(MGFSINGLEPLAYERINSTANCE);
	disableLogging();
}


bool BotWars::processEvents()
{
	// Quit if it has been decided to do so.. also for server instances
	if(getQuitFlag()) return false;

	// No events for a server instance..
	if(getInstanceType() == MGFSERVERINSTANCE) return true;

	// Don't handle input if input is disabled..
	if(!isInputEnabled()) return true;

	// Event processing loop is started after all special cases.
	SDL_Event event;
	while (SDL_PollEvent(&event))//get all events
	{
		switch (event.type)
		{
			// Quit event
			case SDL_QUIT:
			{
				MGFLOG_INFO("SDL_QUIT")
				// Return false because we are quitting.
				return false;
			}

			case SDL_KEYDOWN:
			{
				if(!typingEnabled())
				{
					SDLKey sym = event.key.keysym.sym;
					if(sym == SDLK_ESCAPE) //Enable typing in the console if ESC is pressed..
					{
						activateConsole();
					}
					m_Keys[sym] = 1;
				}
				break;
			}

			case SDL_KEYUP:
			{
				if(!typingEnabled())
				{
					SDLKey sym = event.key.keysym.sym;
					MGFLOG_INFO("SDL_KEYUP" << std::endl << "  " << SDL_GetKeyName(sym))
					m_Keys[sym] = 0;
				}
				break;
			}

			// Default case for unknown events
			default:
			{
				break;
			}
		}
	}

	return true;
}

bool BotWars::init(int w, int h, int tw, int th)
{
	// The window is created.
	if (windowPropertiesSet())
	{
		if (!m_Window.createWindow()) 
		{
			return false;
		}

		// All graphics should be loaded here.
		m_Floor = loadBMPImage("tileset.bmp");
		m_MovingObject = loadBMPImage("movingobject.bmp");
		m_StationaryObject = loadBMPImage("stationaryobject.bmp");
		m_Mark = loadBMPImage("mark.bmp");
		SDL_SetColorKey(m_MovingObject, SDL_SRCCOLORKEY, 0);
		SDL_SetColorKey(m_StationaryObject, SDL_SRCCOLORKEY, 0);
		SDL_SetColorKey(m_Mark, SDL_SRCCOLORKEY, 0);


		// Objcts such as the map are initialized here.
		m_Map.init(w, h, tw, th, m_Window.getWidth(), m_Window.getHeight()); // width (in number of tiles), height, tile width (in pixels), tile height, resolution x and y.

		// Setup the edge around the screen to allow scrolling to see the entire map.
		m_Map.setTopEdge(0);
		m_Map.setBottomEdge(0);
		m_Map.setLeftEdge(0);
		m_Map.setRightEdge(0);

		// Setup application specific game logics..

		runConsoleCommand("logging on", this); // Turn on logging for the MGFramework class
		runConsoleCommand("map logging on", this); // Turn on logging for the MGMap class

		setDynamicFPSEnabled(false);
		runConsoleCommand("setfps 60", this);
		runConsoleCommand("open terminalserver", this);
		runConsoleCommand("minimap on", this);

		runConsoleCommand("create so 1000", this);

		addBots(10, 1, NULL);
		addBots(10, 2, NULL);
		addBots(10, 3, NULL);

		runConsoleCommand("logging off", this); // Turn on logging for the MGFramework class
		runConsoleCommand("map logging off", this); // Turn on logging for the MGMap class

		return true;
	}
	else
	{
		return false;
	}
}

void BotWars::handleGameLogics()
{
	// Application specific game logics are updated here..
}

void BotWars::draw()
{
	// Draw all tiles visible in the window...
	for (int x=0; x < m_Map.getWidth(); x++)
	{
		for ( int y=0; y < m_Map.getHeight(); y++)
		{
			// Only draw the tiles actually visible (+1 to draw partly visible tiles) in the window...
			if(  ((x * m_Map.getTileWidth() + m_Map.getScrollX()) <= m_Window.getWidth() + m_Map.getTileWidth()) &&
				 ((x * m_Map.getTileWidth() + m_Map.getScrollX()) >= 0 - m_Map.getTileWidth()) &&
				 ((y * m_Map.getTileHeight() + m_Map.getScrollY()) <= m_Window.getHeight() + m_Map.getTileHeight()) &&
				 ((y * m_Map.getTileHeight() + m_Map.getScrollY()) >= 0 - m_Map.getTileHeight())  )
			{
				if(m_Map.getTileProperty(x, y) & MGMAP_TP_PROPERTY_1)
				{
					drawSprite(m_Floor, getSurface(), 0, 0, x * m_Map.getTileWidth() + m_Map.getScrollX(), y * m_Map.getTileHeight() + m_Map.getScrollY(), m_Map.getTileWidth(), m_Map.getTileHeight());
				}
				else if(m_Map.getTileProperty(x, y) & MGMAP_TP_PROPERTY_2)
				{
					drawSprite(m_Floor, getSurface(), 32, 64, x * m_Map.getTileWidth() + m_Map.getScrollX(), y * m_Map.getTileHeight() + m_Map.getScrollY(), m_Map.getTileWidth(), m_Map.getTileHeight());
				}
			}
		}
	}

	// Draw all moving objects...
	int oX,oY;
	for(int i=0;i<getNumberOfMO();i++)
	{
		if(m_MO != NULL)
		{
			oX=m_MO[i].getTileX() * m_Map.getTileWidth() + m_Map.getScrollX()+m_MO[i].getXOffset();
			oY=m_MO[i].getTileY() * m_Map.getTileHeight() + m_Map.getScrollY() + m_MO[i].getYOffset();
			// Only draw visible moving objects...
			if(detectCollisionRectangle(oX, oY, oX+m_Map.getTileWidth(), oY+m_Map.getTileHeight(), 0, 0, m_Window.getWidth(), m_Window.getHeight()))
			{
				drawSprite(m_MovingObject, getSurface(), 0, 0, oX, oY, m_Map.getTileWidth(), m_Map.getTileHeight());
				if(m_MO[i].isMarked())
				{
					drawSprite(m_Mark, getSurface(), 0, 0, oX, oY, m_Map.getTileWidth(), m_Map.getTileHeight());
				}
			}
		}
	}

	// Draw all stationary objects...
	int sX,sY;
	for(int i=0;i<getNumberOfSO();i++)
	{
		if(m_SO != NULL)
		{
			sX=m_SO[i].getTileX() * m_Map.getTileWidth() + m_Map.getScrollX();
			sY=m_SO[i].getTileY() * m_Map.getTileHeight() + m_Map.getScrollY()-16;
			// Only draw visible stationary objects...
			if(detectCollisionRectangle(sX, sY, sX+m_Map.getTileWidth(), sY+m_Map.getTileHeight(), 0, 0, m_Window.getWidth(), m_Window.getHeight()))
			{
				drawSprite(m_StationaryObject, getSurface(), 0, 0, sX, sY, m_Map.getTileWidth(), m_Map.getTileHeight()+16);
			}
		}
	}

	// Draw the mini map if enabled. Also draw all objects on it...
	if(miniMapEnabled())
	{
		for (int x=0; x < m_Map.getWidth(); x++)
		{
			for ( int y=0; y < m_Map.getHeight(); y++)
			{
				// Only draw the tiles actually visible in the window...
				if(  ((x * m_Map.getTileWidth() + m_Map.getScrollX()) <= m_Window.getWidth()) &&
					 ((x * m_Map.getTileWidth() + m_Map.getScrollX()) >= 0) &&
					 ((y * m_Map.getTileHeight() + m_Map.getScrollY()) <= m_Window.getHeight()) &&
					 ((y * m_Map.getTileHeight() + m_Map.getScrollY()) >= 0)  )
				{
					// Different color for different tile property of each tile...
					if(m_Map.getTileProperty(x, y)  & MGMAP_TP_PROPERTY_1)
					{
						putPixel32(getSurface(), x + m_Window.getWidth() - m_Map.getWidth() - 16, y + 16, 0x3F3F3F3F);
					}
					else if(m_Map.getTileProperty(x, y)  & MGMAP_TP_PROPERTY_2)
					{
						putPixel32(getSurface(), x + m_Window.getWidth() - m_Map.getWidth() - 16, y + 16, 0xFFFFFFFF);
					}
				}
				else
				{
					// Draw it black..
					putPixel32(getSurface(), x + m_Window.getWidth() - m_Map.getWidth() - 16, y + 16, 0x00000000);
				}
			}
		}
		// Draw all moving objects on the mini map..
		for(int i=0;i<getNumberOfMO();i++)
		{
			putPixel32(getSurface(), m_MO[i].getTileX() + m_Window.getWidth() - m_Map.getWidth() - 16, m_MO[i].getTileY() + 16, 0x00FF0000);
		}
	}

	// Example of how text can be printed on the surface.. Here FPS and time left between frames.
#ifndef MGF_DEBUGGING_ENABLED
	drawText(getSurface(), (string("MOs: ") + MGFramework::toString((int)getNumberOfMO())).c_str(), 16, m_Window.getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 30, 0, 0, 0, 0, 255, 0);
	drawText(getSurface(), (string("FD : ") + MGFramework::toString((int)getLastFrameDelayTime())).c_str(), 16, m_Window.getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 50, 0, 0, 0, 0, 255, 0);
	drawText(getSurface(), (string("FPS: ") + MGFramework::toString((int)getFPS())).c_str(), 16, m_Window.getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 70, 0, 0, 0, 0, 255, 0);
#endif


	// Draw marking frame if marking is ongoing
	if(isFramingOngoing())
	{
		int uLX=smallest(getFrameStartX(), getFrameEndX());
		int uLY=smallest(getFrameStartY(), getFrameEndY());
		int xL=abs(getFrameStartX() - getFrameEndX());
		int yL=abs(getFrameStartY() - getFrameEndY());
		hLine32(getSurface(), uLX, uLY, xL, 0x00FF0000);
		hLine32(getSurface(), uLX, uLY+yL, xL, 0x00FF0000);
		vLine32(getSurface(), uLX, uLY, yL, 0x00FF0000);
		vLine32(getSurface(), uLX+xL, uLY, yL, 0x00FF0000);
	}


}


void BotWars::addBots(int n, unsigned int team, const char* script)
{	
	if(getNumberOfMO()==getNumberOfPE())
	{
		int first = addMO(n);
		addPE(n);
		for(int i=first; i<first+n; ++i)
		{
			setupMO(i, -1, -1, team, 3);
			m_PE[i].setOwner(team);
			m_PE[i].setFileName1(script);
			m_PE[i].setupTimer(3000);
			m_PE[i].activate();
		}
	}
	else
	{
		MGFLOG_ERROR("Number of MO and PE do not match! MO:" << getNumberOfMO() << ", PE:" << getNumberOfPE());
	}

}