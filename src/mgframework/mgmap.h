#ifndef _MG_MAP_H
#define _MG_MAP_H

#include <SDL/SDL.h>

class MGMap
{
private:
	int m_Width; // Number of tiles the map is wide.
	int m_Height; // Number of tiles the map is high.
	int m_TileWidth; // The width of each tile (in pixels).
	int m_TileHeight; // The height of each tile (in pixels).
	int m_ScrollX; // Number of pixels from top left.
	int m_ScrollY; // Number of pixels from top left.

	int *m_TileProperty; // Allows for many unique properties (ex: water, sand, grass, walkable, non-walkable etc.)
	int m_WindowHeight;
	int m_WindowWidth;

	// Scrolling related
	bool m_MouseScrollingOngoing;
	int m_MouseScrollingXClick;
	int m_MouseScrollingYClick;

public:
	MGMap();
	~MGMap();

	int getWidth();
	int getHeight();
	int getTileWidth();
	int getTileHeight();

	int getScrollX();
	int getScrollY();
	void setScrollOffset(int px, int py);
	void mouseScrollingRelease(int x, int y);
	void mouseScrollingClick(int x, int y);
	void mouseScrollingUpdate(int x, int y);


	void init(int w, int h, int tw, int th, int windowWidth, int windowHeight);
	void setTileProperty(int x, int y, int value); // Public to allow dynamic map properties.
	int getTileProperty(int x, int y);

	int getTileIndex(int clickX, int clickY);
	int getTileX(int index);
	int getTileY(int index);

	int getWindowHeight();
	int getWindowWidth();

	void save(); // Not implemented yet
};


#endif







