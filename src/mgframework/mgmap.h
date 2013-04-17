#ifndef _MG_MAP_H
#define _MG_MAP_H

#include <SDL/SDL.h>
#include "mgcomponent.h"

class MGMap :public MGComponent
{
private:
	int m_Width; // Number of tiles the map is wide.
	int m_Height; // Number of tiles the map is high.
	int m_TileWidth; // The width of each tile (in pixels).
	int m_TileHeight; // The height of each tile (in pixels).
	int m_ScrollX; // Number of pixels from top left.
	int m_ScrollY; // Number of pixels from top left.

	int *m_TileProperty; // Allows for many unique properties (ex: water, sand, grass, walkable, non-walkable etc.)
	int *m_Occupied;
	int m_WindowHeight;
	int m_WindowWidth;

	// Scrolling related
	bool m_MouseScrollingOngoing;
	int m_MouseScrollingXClick;
	int m_MouseScrollingYClick;

public:
	MGMap();
	~MGMap();


	int getWidth(){ return m_Width;}
	int getHeight(){ return m_Height;}
	int getTileWidth(){ return m_TileWidth;}
	int getTileHeight(){ return m_TileHeight;}

	int getScrollX(){ return m_ScrollX;}
	int getScrollY(){ return m_ScrollY;}
	void setScrollOffset(int px, int py);
	void mouseScrollingRelease(int x, int y);
	void mouseScrollingClick(int x, int y);
	void mouseScrollingUpdate(int x, int y);


	void init(int w, int h, int tw, int th, int windowWidth, int windowHeight);
	void setTileProperty(int x, int y, int value){ m_TileProperty[y*getWidth()+x]=value;}
	int getTileProperty(int x, int y){ return m_TileProperty[y*getWidth()+x];}

	void occupy(int x, int y, int id){ m_Occupied[y*getWidth()+x]=id;}
	void unOccupy(int x, int y){ m_Occupied[y*getWidth()+x]=0;}
	int occupant(int x, int y){ return m_Occupied[y*getWidth()+x];}


	int getTileIndex(int clickX, int clickY);
	int getTileX(int index){ return index % getWidth();}
	int getTileY(int index){ return (index - getTileX(index))/getWidth();}

	int getWindowHeight(){ return m_WindowHeight;}
	int getWindowWidth(){ return m_WindowWidth;}

	void save(); // Not implemented yet

	bool runConsoleCommand(const char *c);
};


#endif







