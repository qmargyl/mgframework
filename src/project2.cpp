#include "project2.h"
#include <iostream>
#include <algorithm> 
#include "mgframework/mgframework.h"

Project2::Project2()
{
	// Default parameters for demo application Project2...
	unsetWindowProperties(); // Force setWindowProperties to be called before init.
	disableTyping();
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
		m_Floor = m_Window.loadBMPImage("tileset.bmp");
		m_MovingObject = m_Window.loadBMPImage("movingobject.bmp");
		m_StationaryObject = m_Window.loadBMPImage("stationaryobject.bmp");
		m_Mark = m_Window.loadBMPImage("mark.bmp");
		SDL_SetColorKey(m_MovingObject, SDL_SRCCOLORKEY, 0);
		SDL_SetColorKey(m_StationaryObject, SDL_SRCCOLORKEY, 0);
		SDL_SetColorKey(m_Mark, SDL_SRCCOLORKEY, 0);


		// Objcts such as the map are initialized here.
		m_Map.init(w, h, tw, th, m_Window.getWidth(), m_Window.getHeight()); // width (in number of tiles), height, tile width (in pixels), tile height, resolution x and y.

		// Setup the edge around the screen to allow scrolling to see the entire map.
		m_Map.setTopEdge(8);
		m_Map.setBottomEdge(64);
		m_Map.setLeftEdge(8);
		m_Map.setRightEdge(64);

		// Activate/deactivate framework features
		enableFeatureSelectiveTileRendering();
		enableFeatureMouseScrolling();
		disableFeatureCenterOnMO();
		//disableFeatureMouseScrolling();
		//enableFeatureCenterOnMO(0);

		// Setup application specific game logics..

		runConsoleCommand("open terminalserver", this, NULL);
		runConsoleCommand("logging off", this, NULL); // Turn on logging for the MGFramework class
		runConsoleCommand("map logging off", this, NULL); // Turn on logging for the MGMap class
		runConsoleCommand("minimap on", this, NULL);

		return true;
	}
	else
	{
		return false;
	}
}

void Project2::handleGameLogics()
{
	// Application specific game logics are updated here..
}

void Project2::draw()
{
	bool noRenderingNeeded = isSelectiveTileRenderingActive() && !MGMovingObject::anyMovingMO() && !isFramingOngoing() && !renderAllTiles();

	if(!noRenderingNeeded)
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
					 ((y * m_Map.getTileHeight() + m_Map.getScrollY()) >= 0 - m_Map.getTileHeight()) &&
					 (!isSelectiveTileRenderingActive() || renderAllTiles() || m_Map.isMarkedForRendering(x, y)) )
				{
					if(m_Map.getTileProperty(x, y) & MGMAP_TP_PROPERTY_1)
					{
						drawTile(m_Floor, 0, 0, x * m_Map.getTileWidth() + m_Map.getScrollX(), y * m_Map.getTileHeight() + m_Map.getScrollY());
					}
					else if(m_Map.getTileProperty(x, y) & MGMAP_TP_PROPERTY_2)
					{
						drawTile(m_Floor, 32, 64, x * m_Map.getTileWidth() + m_Map.getScrollX(), y * m_Map.getTileHeight() + m_Map.getScrollY());
					}
					m_Map.unmarkForRendering(x, y);
				}
			}
		}

		// Draw all moving objects...
		int oX,oY;
		for(int i = getNumberOfMO(); i--;)//for(int i=0;i<getNumberOfMO();i++)
		{
			if(m_MO != NULL)
			{
				oX=m_MO[i].getTileX() * m_Map.getTileWidth() + m_Map.getScrollX()+m_MO[i].getXOffset();
				oY=m_MO[i].getTileY() * m_Map.getTileHeight() + m_Map.getScrollY() + m_MO[i].getYOffset();
				// Only draw visible moving objects...
				if(detectCollisionRectangle(oX, oY, oX+m_Map.getTileWidth(), oY+m_Map.getTileHeight(), 0, 0, m_Window.getWidth(), m_Window.getHeight()))
				{
					drawTile(m_MovingObject, 0, 0, oX, oY);
					if(isSelectiveTileRenderingActive())
					{
						m_Map.markForRendering(m_MO[i].getTileX(), m_MO[i].getTileY());
						m_Map.markForRendering(m_MO[i].getTileX()+1, m_MO[i].getTileY()+1);
						m_Map.markForRendering(m_MO[i].getTileX()-1, m_MO[i].getTileY()-1);
						m_Map.markForRendering(m_MO[i].getTileX()+1, m_MO[i].getTileY()-1);
						m_Map.markForRendering(m_MO[i].getTileX()-1, m_MO[i].getTileY()+1);
						m_Map.markForRendering(m_MO[i].getTileX()+1, m_MO[i].getTileY());
						m_Map.markForRendering(m_MO[i].getTileX()-1, m_MO[i].getTileY());
						m_Map.markForRendering(m_MO[i].getTileX(), m_MO[i].getTileY()+1);
						m_Map.markForRendering(m_MO[i].getTileX(), m_MO[i].getTileY()-1);
					}

					if(m_MO[i].isMarked())
					{
						drawTile(m_Mark, 0, 0, oX, oY);
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
					drawTile(m_StationaryObject, 0, 0, sX, sY, m_Map.getTileWidth(), m_Map.getTileHeight()+16);
				}
			}
		}

		// Draw a frame around the edge of the map
		m_Window.vLine32(m_Map.getLeftEdge(), m_Map.getTopEdge(), m_Map.getWindowHeight()-m_Map.getBottomEdge()-m_Map.getTopEdge(), 0x000000FF);
		m_Window.vLine32(m_Map.getWindowWidth()-m_Map.getRightEdge(), m_Map.getTopEdge(), m_Map.getWindowHeight()-m_Map.getBottomEdge()-m_Map.getTopEdge(), 0x000000FF);
		m_Window.hLine32(m_Map.getLeftEdge(), m_Map.getTopEdge(), m_Map.getWindowWidth()-m_Map.getLeftEdge()-m_Map.getRightEdge(), 0x000000FF);
		m_Window.hLine32(m_Map.getLeftEdge(), m_Map.getWindowHeight()-m_Map.getBottomEdge(), m_Map.getWindowWidth()-m_Map.getLeftEdge()-m_Map.getRightEdge(), 0x000000FF);


		// Draw the mini map if enabled. Also draw all objects on it...
		if(featureMiniMapEnabled())
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
							m_Window.putPixel32(x + m_Window.getWidth() - m_Map.getWidth() - 16, y + 16, 0x3F3F3F3F);
						}
						else if(m_Map.getTileProperty(x, y)  & MGMAP_TP_PROPERTY_2)
						{
							m_Window.putPixel32(x + m_Window.getWidth() - m_Map.getWidth() - 16, y + 16, 0xFFFFFFFF);
						}
					}
					else
					{
						// Draw it black..
						m_Window.putPixel32(x + m_Window.getWidth() - m_Map.getWidth() - 16, y + 16, 0x00000000);
					}
				}
			}
			// Draw all moving objects on the mini map..
			for(int i = getNumberOfMO(); i--;)//for(int i=0;i<getNumberOfMO();i++)
			{
				m_Window.putPixel32(m_MO[i].getTileX() + m_Window.getWidth() - m_Map.getWidth() - 16, m_MO[i].getTileY() + 16, 0x00FF0000);
			}
		}
	}

	// Example of how text can be printed on the surface.. Here FPS and time left between frames.
	m_Window.drawText((string("MOs: ") + MGFramework::toString((int)getNumberOfMO()) + 
			 string("(") + MGFramework::toString((int)MGMovingObject::nMovingMO()) + string(")") + string("          ")).c_str(), 
			 16, m_Window.getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 30, 0, 0, 0, 0, 255, 0);
	m_Window.drawText((string("FD : ") + MGFramework::toString((int)getLastFrameDelayTime()) + string("          ")).c_str(), 
			 16, m_Window.getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 50, 0, 0, 0, 0, 255, 0);
	m_Window.drawText((string("FPS: ") + MGFramework::toString((int)getFPS()) + string("          ")).c_str(), 
			 16, m_Window.getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 70, 0, 0, 0, 0, 255, 0);
	m_Window.drawText((string("DT: ") + MGFramework::toString(getDrawnTilesCounter()) + string("          ")).c_str(), 
			 16, m_Window.getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 90, 0, 0, 0, 0, 255, 0);


	// Draw marking frame if marking is ongoing
	if(!noRenderingNeeded)
	{
		if(isFramingOngoing())
		{
			int uLX=std::min(getFrameStartX(), getFrameEndX());
			int uLY=std::min(getFrameStartY(), getFrameEndY());
			int xL=abs(getFrameStartX() - getFrameEndX());
			int yL=abs(getFrameStartY() - getFrameEndY());
			m_Window.hLine32(uLX, uLY, xL, 0x00FF0000);
			m_Window.hLine32(uLX, uLY+yL, xL, 0x00FF0000);
			m_Window.vLine32(uLX, uLY, yL, 0x00FF0000);
			m_Window.vLine32(uLX+xL, uLY, yL, 0x00FF0000);
		}
	}


}
