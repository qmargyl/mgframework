#include "project2.h"
#include <iostream>
#include "mgframework/mgframework.h"

Project2::Project2()
{
	// Default parameters for demo application Project2...
	unsetWindowProperties(); // Force setWindowProperties to be called before init.
	disableLogging();
	disableTyping();
	//setProgramName("Demo App Project2");
	setProgramVersion("0.2");
}

bool Project2::init(int w, int h, int tw, int th)
{
	// The window is created.
	if (windowPropertiesSet())
	{
		if (!m_Window.createWindow()) 
		{
			return false;
		}

		// All graphics should be loaded here.
		m_Brick = loadBMPImage("brick1_blue_32x32.bmp");
		m_Floor = loadBMPImage("floor1_blue_32x32.bmp");


		// Objcts such as the map are initialized here.
		map.init(w, h, tw, th, m_Window.getWidth(), m_Window.getHeight()); // width (in number of tiles), height, tile width (in pixels), tile height, resolution x and y.


		// Setup game logic..
		m_PE.setupTimer(2000);
		m_PE.activate();
		enableMiniMap();

		return true;
	}
	else
	{
		return false;
	}
}

void Project2::handleGameLogics()
{
	if(m_PE.update())
	{
		std::cout << "PeriodicEvent m_PE triggered" << std::endl;
	}
}

void Project2::draw()
{
	for (int x=0; x < map.getWidth(); x++)
	{
		for ( int y=0; y < map.getHeight(); y++)
		{
			// Only draw the tiles actually visible (+1 to draw partly visible tiles) in the window...
			if(  ((x * map.getTileWidth() + map.getScrollX()) <= m_Window.getWidth() + map.getTileWidth()) &&
				 ((x * map.getTileWidth() + map.getScrollX()) >= 0 - map.getTileWidth()) &&
				 ((y * map.getTileHeight() + map.getScrollY()) <= m_Window.getHeight() - map.getTileHeight()) &&
				 ((y * map.getTileHeight() + map.getScrollY()) >= 0 - map.getTileHeight())  )
			{
				if(map.getTileProperty(x, y) == 0)
				{
					drawSprite(m_Brick, getSurface(), 0, 0, x * map.getTileWidth() + map.getScrollX(), y * map.getTileHeight() + map.getScrollY(), map.getTileWidth(), map.getTileHeight());
				}
				else if(map.getTileProperty(x, y) == 1)
				{
					drawSprite(m_Floor, getSurface(), 0, 0, x * map.getTileWidth() + map.getScrollX(), y * map.getTileHeight() + map.getScrollY(), map.getTileWidth(), map.getTileHeight());
				}
			}
		}
	}


	if(miniMapEnabled())
	{
		for (int x=0; x < map.getWidth(); x++)
		{
			for ( int y=0; y < map.getHeight(); y++)
			{
				// Only draw the tiles actually visible in the window...
				if(  ((x * map.getTileWidth() + map.getScrollX()) <= m_Window.getWidth()) &&
					 ((x * map.getTileWidth() + map.getScrollX()) >= 0) &&
					 ((y * map.getTileHeight() + map.getScrollY()) <= m_Window.getHeight()) &&
					 ((y * map.getTileHeight() + map.getScrollY()) >= 0)  )
				{
					if(map.getTileProperty(x, y) == 0)
					{
						// Represent the tile on the mini map if enabled...
						putPixel32(getSurface(), x + m_Window.getWidth() - map.getWidth() - 16, y + 16, 0x3F3F3F3F);
					}
					else if(map.getTileProperty(x, y) == 1)
					{
						// Represent the tile on the mini map if enabled...
						putPixel32(getSurface(), x + m_Window.getWidth() - map.getWidth() - 16, y + 16, 0xFFFFFFFF);
					}
				}
				else
				{
					// Represent the tile on the mini map if enabled...
					putPixel32(getSurface(), x + m_Window.getWidth() - map.getWidth() - 16, y + 16, 0x00000000);
				}
			}
		}
	}

	// Example of how text can be printed on the surface.. Here FPS and time left between frames.
	drawText(getSurface(), MGFramework::toString(getLastFrameDelayTime()).c_str(), 16, m_Window.getWidth() - map.getWidth() - 16, map.getHeight() + 50, 0, 0, 0, 0, 255, 0);
	drawText(getSurface(), MGFramework::toString(getFPS()).c_str(), 16, m_Window.getWidth() - map.getWidth() - 16, map.getHeight() + 30, 0, 0, 0, 0, 255, 0);
}
