#ifndef _MG_MAP_H
#define _MG_MAP_H

#include <SDL/SDL.h>
#include <iostream>
#include "mgcomponent.h"


#define MGMAP_TP_NOOBSTACLE  0
#define MGMAP_TP_PROPERTY_1  2
#define MGMAP_TP_PROPERTY_2  4
#define MGMAP_TP_PROPERTY_3  8
#define MGMAP_TP_PROPERTY_4  16
#define MGMAP_TP_PROPERTY_5  32

enum eMGFPathType{
	MGFSTRAIGHTLINE = 0,
	MGFSKYPATH = 1,
	MGFGROUNDPATH = 2,
	MGFBASICPATH1
};


class MGMap :public MGComponent
{
private:
	int m_Width; // Number of tiles the map is wide.
	int m_Height; // Number of tiles the map is high.
	int m_TileWidth; // The width of each tile (in pixels).
	int m_TileHeight; // The height of each tile (in pixels).
	int m_ScrollX; // Number of pixels from top left.
	int m_ScrollY; // Number of pixels from top left.

	Uint32 *m_TileProperty; // Allows for many unique properties (ex: water, sand, grass, walkable, non-walkable etc.)
	int *m_Occupied;
	int m_WindowHeight;
	int m_WindowWidth;

	// Scrolling related
	bool m_MouseScrollingOngoing;
	int m_MouseScrollingXClick;
	int m_MouseScrollingYClick;

	// A graphics edge around the screen means we have to scroll
	// a bit more to be able to see the whole map.
	unsigned int m_TopEdge;
	unsigned int m_BottomEdge;
	unsigned int m_LeftEdge;
	unsigned int m_RightEdge;


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

	// A graphics edge around the screen means we have to scroll
	// a bit more to be able to see the whole map.
	int getTopEdge(){ return m_TopEdge; }
	int getBottomEdge(){ return m_BottomEdge; }
	int getLeftEdge(){ return m_LeftEdge; }
	int getRightEdge(){ return m_RightEdge; }
	void setTopEdge(int e){ m_TopEdge = e; }
	void setBottomEdge(int e){ m_BottomEdge = e; }
	void setLeftEdge(int e){ m_LeftEdge = e; }
	void setRightEdge(int e){ m_RightEdge = e; }

	void init(int w, int h, int tw, int th, int windowWidth, int windowHeight);
	void reInit(int w, int h, int tw, int th);
	void setTileProperty(int x, int y, Uint32 value){ m_TileProperty[y*getWidth()+x]=value;}
	Uint32 getTileProperty(int x, int y){ return m_TileProperty[y*getWidth()+x];}

	void occupy(int x, int y, int id){ m_Occupied[y*getWidth()+x]=id;}
	void unOccupy(int x, int y){ m_Occupied[y*getWidth()+x]=0;}
	int occupant(int x, int y){ if(x<0) return 0; if(y<0) return 0; if(x>=getWidth()) return 0; if(x>=getHeight()) return 0; return m_Occupied[y*getWidth()+x];}


	int getTileIndex(int clickX, int clickY);
	int getTileX(int index){ return index % getWidth();}
	int getTileY(int index){ return (index - getTileX(index))/getWidth();}

	int getWindowHeight(){ return m_WindowHeight;}
	int getWindowWidth(){ return m_WindowWidth;}

	void save(); // Not implemented yet

	// Path related
	void calculatePath(eMGFPathType pathType, int ax, int ay, int bx, int by); // Calculates the path from A to B

	bool runConsoleCommand(const char *c, MGFramework *w);
	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);
};


#endif







