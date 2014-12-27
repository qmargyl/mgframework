#include "project2.h"
#include <iostream>
#include <algorithm> 
#include "mgframework/mgframework.h"

#define DEBUG_MARK_OCCUPIED 1

Project2::Project2()
{
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
		if (!getWindow()->createWindow()) 
		{
			return false;
		}

		// All graphics should be loaded here.
		for(unsigned int i = 0; i < NO_OF_TEXTURES; i++) textures.push_back(MGTexHandle());
		getWindow()->loadBMPImage("tileset.bmp", textures[TEX_GRASS], false);
		getWindow()->loadBMPImage("movingobject.bmp", textures[TEX_PINKBLOB], true);
		getWindow()->loadBMPImage("tree.bmp", textures[TEX_TREE], true);
		getWindow()->loadBMPImage("tree2.bmp", textures[TEX_TREE2], true);
		getWindow()->loadBMPImage("mark.bmp", textures[TEX_REDFRAME], true);

		// Objects such as the map are initialized here.
		m_Map.init(w, h, tw, th, getWindow()->getWidth(), getWindow()->getHeight()); // width (in number of tiles), height, tile width (in pixels), tile height, resolution x and y.

		// Setup the edge around the screen to allow scrolling to see the entire map.
		m_Map.setTopEdge(8);
		m_Map.setBottomEdge(64);
		m_Map.setLeftEdge(8);
		m_Map.setRightEdge(64);

		// Activate/deactivate framework features
		enableFeatureSelectiveTileRendering();
		enableFeatureMouseScrolling();
		disableFeatureCenterOnMO();

		// Setup application specific logics..
		setPort(667);
		runConsoleCommand("open terminalserver", this, NULL);
		runConsoleCommand("logging off", this, NULL);
		runConsoleCommand("map logging off", this, NULL);
		runConsoleCommand("minimap on", this, NULL);

//		runConsoleCommand("add so 2000 -type 17", this, NULL);
//		increaseDensityOfStationaryObjects(17, 2);
//		increaseDensityOfStationaryObjects(17, 2);
//		fillInStationaryObjectClusters(17);

		std::vector<MGTexHandle*> tHVec;
		tHVec.push_back(&textures[TEX_TREE]);
		tHVec.push_back(&textures[TEX_TREE2]);
		setStationaryObjectTexHandles(17, tHVec);

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
		int tX, tY;
		for(int x = 0; x < m_Map.getWidth(); x++)
		{
			tX = x * m_Map.getTileWidth() + m_Map.getScrollX();
			for(int y = 0; y < m_Map.getHeight(); y++)
			{
				// Only draw the tiles actually visible (+1 to draw partly visible tiles) in the window...
				tY = y * m_Map.getTileHeight() + m_Map.getScrollY();
				if(  (tX <= getWindow()->getWidth() + m_Map.getTileWidth()) &&
					 (tX >= 0 - m_Map.getTileWidth()) &&
					 (tY <= getWindow()->getHeight() + m_Map.getTileHeight()) &&
					 (tY >= 0 - m_Map.getTileHeight()) &&
					 (!isSelectiveTileRenderingActive() || renderAllTiles() || m_Map.isMarkedForRendering(x, y)) )
				{
					if(m_Map.getTileProperty(x, y) & MGMAP_TP_PROPERTY_1)
					{
						getWindow()->drawSprite(textures[TEX_GRASS], 0, 0, tX, tY, m_Map.getTileWidth(), m_Map.getTileHeight());
					}
					else if(m_Map.getTileProperty(x, y) & MGMAP_TP_PROPERTY_2)
					{
						getWindow()->drawSprite(textures[TEX_GRASS], 32, 64, tX, tY, m_Map.getTileWidth(), m_Map.getTileHeight());
					}
					m_Map.unmarkForRendering(x, y);
					if(DEBUG_MARK_OCCUPIED)
					{
						if(m_Map.occupant(x, y) != 0)
						{
							getWindow()->drawCircleRGB(tX + m_Map.getTileWidth() / 2, tY + m_Map.getTileHeight() / 2, m_Map.getTileHeight() / 2, 0, 0, 255);
						}
					}
				}
			}
		}

		// Draw all moving objects...
		int oX, oY;
		for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
		{
			oX = it->getTileX() * m_Map.getTileWidth() + m_Map.getScrollX() + it->getXOffset();
			oY = it->getTileY() * m_Map.getTileHeight() + m_Map.getScrollY() + it->getYOffset();
			// Only draw visible moving objects...
			if(detectCollisionRectangle(oX, oY, oX + m_Map.getTileWidth(), oY + m_Map.getTileHeight(), 0, 0, getWindow()->getWidth(), getWindow()->getHeight()))
			{
				getWindow()->drawSprite(textures[TEX_PINKBLOB], 0, 0, oX, oY, m_Map.getTileWidth(), m_Map.getTileHeight());
				if(!it->isIdle() && isSelectiveTileRenderingActive())
				{
					m_Map.markForRendering(it->getTileX(), it->getTileY());
					m_Map.markForRendering(it->getTileX() + 1, it->getTileY() + 1);
					m_Map.markForRendering(it->getTileX() - 1, it->getTileY() - 1);
					m_Map.markForRendering(it->getTileX() + 1, it->getTileY() - 1);
					m_Map.markForRendering(it->getTileX() - 1, it->getTileY() + 1);
					m_Map.markForRendering(it->getTileX() + 1, it->getTileY());
					m_Map.markForRendering(it->getTileX() - 1, it->getTileY());
					m_Map.markForRendering(it->getTileX(), it->getTileY() + 1);
					m_Map.markForRendering(it->getTileX(), it->getTileY() - 1);
				}

				if(it->isMarked())
				{
					getWindow()->drawSprite(textures[TEX_REDFRAME], 0, 0, oX, oY, m_Map.getTileWidth(), m_Map.getTileHeight());
				}
			}
		}

		// Draw all stationary objects...
		drawAllSOWithTexHandles();

		// Draw a frame around the edge of the map
		getWindow()->drawRectangleRGB(m_Map.getLeftEdge(), m_Map.getTopEdge(), m_Map.getWindowWidth() - m_Map.getLeftEdge() - m_Map.getRightEdge(), m_Map.getWindowHeight() - m_Map.getBottomEdge() - m_Map.getTopEdge(),  0x00, 0x00, 0xFF);

		// Draw the mini map if enabled. Also draw all objects on it...
		drawBasicMiniMap(16, 16);
	}

	// Example of how text can be printed on screen.. Here FPS and time left between frames.
	getWindow()->drawText((std::string("MOs: ") + MGFramework::toString((int)getNumberOfMO()) + 
		std::string("(") + MGFramework::toString((int)MGMovingObject::nMovingMO()) + std::string(")") + std::string("      ")).c_str(), 
			 16, getWindow()->getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 30, 0, 0, 0, 0, 255, 0);
	getWindow()->drawText((std::string("FD : ") + MGFramework::toString((int)getLastFrameDelayTime()) + std::string("      ")).c_str(), 
			 16, getWindow()->getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 50, 0, 0, 0, 0, 255, 0);
	getWindow()->drawText((std::string("FPS: ") + MGFramework::toString((int)getFPS()) + std::string("      ")).c_str(), 
			 16, getWindow()->getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 70, 0, 0, 0, 0, 255, 0);
	getWindow()->drawText((std::string("DT: ") + MGFramework::toString(getWindow()->getDrawnSpritesCounter()) + std::string("      ")).c_str(), 
			 16, getWindow()->getWidth() - m_Map.getWidth() - 16, m_Map.getHeight() + 90, 0, 0, 0, 0, 255, 0);


	// Draw marking frame if marking is ongoing
	if(!noRenderingNeeded)
	{
		if(isFramingOngoing())
		{
			getWindow()->drawRectangleRGB(getFrameStartX(), getFrameStartY(), getFrameEndX(), getFrameEndY(),  0xFF, 0x00, 0x00);
		}
	}


}
