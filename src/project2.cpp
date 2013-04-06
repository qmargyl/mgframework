#include "project2.h"
#include <iostream>
#include "mgframework/mgframework.h"
#include "mgframework/mgmovingobject.h"

Project2::Project2()
{
	// Default parameters for demo application Project2...
	unsetWindowProperties(); // Force setWindowProperties to be called before init.
	disableLogging();
	disableTyping();
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
		m_MovingObject = loadBMPImage("movingobject.bmp");


		// Objcts such as the map are initialized here.
		m_Map.init(w, h, tw, th, m_Window.getWidth(), m_Window.getHeight()); // width (in number of tiles), height, tile width (in pixels), tile height, resolution x and y.


		// Setup game logics..

		// Local MG components..
		m_PE.setupTimer(4000);
		m_PE.activate();

		runConsoleCommand("fps 60"); // Framework default is 1 FPS
		runConsoleCommand("mo create 5");

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
		//std::cout << "PeriodicEvent m_PE triggered" << std::endl;
		for(int i=0;i<getNumberOfMO();i++)
		{
			m_MO[i].setDestTileXY(randomN(m_Map.getWidth()), randomN(m_Map.getHeight()));
		}
	}

	for(int i=0;i<getNumberOfMO();i++)
	{
		m_MO[i].update();
	}
}

void Project2::draw()
{
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
				if(m_Map.getTileProperty(x, y) == 0)
				{
					drawSprite(m_Brick, getSurface(), 0, 0, x * m_Map.getTileWidth() + m_Map.getScrollX(), y * m_Map.getTileHeight() + m_Map.getScrollY(), m_Map.getTileWidth(), m_Map.getTileHeight());
				}
				else if(m_Map.getTileProperty(x, y) == 1)
				{
					drawSprite(m_Floor, getSurface(), 0, 0, x * m_Map.getTileWidth() + m_Map.getScrollX(), y * m_Map.getTileHeight() + m_Map.getScrollY(), m_Map.getTileWidth(), m_Map.getTileHeight());
				}
			}
		}
	}

	// Draw all moving objects:
	for(int i=0;i<getNumberOfMO();i++)
	{
		drawSprite(m_MovingObject, getSurface(), 0, 0, m_MO[i].getTileX() * m_Map.getTileWidth() + m_Map.getScrollX()+m_MO[i].getXOffset(), m_MO[i].getTileY() * m_Map.getTileHeight() + m_Map.getScrollY() + m_MO[i].getYOffset(), m_Map.getTileWidth(), m_Map.getTileHeight());
	}


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
					if(m_Map.getTileProperty(x, y) == 0)
					{
						// Represent the tile on the mini map if enabled...
						putPixel32(getSurface(), x + m_Window.getWidth() - m_Map.getWidth() - 16, y + 16, 0x3F3F3F3F);
					}
					else if(m_Map.getTileProperty(x, y) == 1)
					{
						// Represent the tile on the mini map if enabled...
						putPixel32(getSurface(), x + m_Window.getWidth() - m_Map.getWidth() - 16, y + 16, 0xFFFFFFFF);
					}
				}
				else
				{
					// Represent the tile on the mini map if enabled...
					putPixel32(getSurface(), x + m_Window.getWidth() - m_Map.getWidth() - 16, y + 16, 0x00000000);
				}
			}
		}

		// Draw all moving objects..
		for(int i=0;i<getNumberOfMO();i++)
		{
			putPixel32(getSurface(), m_MO[i].getTileX() + m_Window.getWidth() - m_Map.getWidth() - 16, m_MO[i].getTileY() + 16, 0x00FF0000);
		}

	}

	// Example of how text can be printed on the surface.. Here FPS and time left between frames.
	drawText(getSurface(), MGFramework::toString(getLastFrameDelayTime()).c_str(), 16, m_Window.getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 50, 0, 0, 0, 0, 255, 0);
	drawText(getSurface(), MGFramework::toString(getFPS()).c_str(), 16, m_Window.getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 30, 0, 0, 0, 0, 255, 0);
}
