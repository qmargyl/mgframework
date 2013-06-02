#include "project2.h"
#include <iostream>
#include "mgframework/mgframework.h"
#include "mgframework/mgmovingobject.h"

Project2::Project2()
{
	// Default parameters for demo application Project2...
	unsetWindowProperties(); // Force setWindowProperties to be called before init.
	disableTyping();
	setProgramVersion("0.2");
	setInstanceType(MGFSINGLEPLAYERINSTANCE);
	disableLogging();
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
		m_Floor = loadBMPImage("tileset.bmp");
		m_MovingObject = loadBMPImage("movingobject.bmp");
		m_Mark = loadBMPImage("mark.bmp");
		SDL_SetColorKey(m_MovingObject, SDL_SRCCOLORKEY, 0);
		SDL_SetColorKey(m_Mark, SDL_SRCCOLORKEY, 0);


		// Objcts such as the map are initialized here.
		m_Map.init(w, h, tw, th, m_Window.getWidth(), m_Window.getHeight()); // width (in number of tiles), height, tile width (in pixels), tile height, resolution x and y.


		// Setup game logics..

		runConsoleCommand("setfps 60", this); // Framework default is 20 FPS
		runConsoleCommand("create mo 50", this);
		runConsoleCommand("open terminalserver", this);
		runConsoleCommand("mo 0 mark", this);
		runConsoleCommand("create pe 1", this);
		runConsoleCommand("pe 0 setuptimer 4000", this);
		runConsoleCommand("pe 0 activate", this);
		runConsoleCommand("minimap on", this);

		return true;
	}
	else
	{
		return false;
	}
}

void Project2::handleGameLogics()
{
	// Update periodic event to trigger rare events
	if(getNumberOfPE()>0)
	{
		if(m_PE[0].update())
		{
			std::cout << "PE0: update" << std::endl;
		}
	}

	// Update all moving objects
	for(int i=0;i<getNumberOfMO();i++)
	{
		m_MO[i].update(this);
	}

	// Example of how FPS can be controlled dynamically
	if(getLastFrameDelayTime()>10) setDesiredFPS(getFPS()+1);
	if(getLastFrameDelayTime()<7) setDesiredFPS(getFPS()-1);
}

void Project2::draw()
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



	// Draw frame if available
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
