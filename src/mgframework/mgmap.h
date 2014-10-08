#ifndef _MG_MAP_H
#define _MG_MAP_H

#include <iostream>
#include <list>
#include "mgcomponent.h"

#define MGMAP_TP_NOOBSTACLE  0
#define MGMAP_TP_PROPERTY_1  2
#define MGMAP_TP_PROPERTY_2  4
#define MGMAP_TP_PROPERTY_3  8
#define MGMAP_TP_PROPERTY_4  16
#define MGMAP_TP_PROPERTY_5  32

class MGPathItem;
enum eMGFPathType;

class MGMap :public MGComponent
{
private:
	int m_Width;      // Number of tiles the map is wide.
	int m_Height;     // Number of tiles the map is high.
	int m_TileWidth;  // The width of each tile (in pixels).
	int m_TileHeight; // The height of each tile (in pixels).
	int m_ScrollX;    // Number of pixels from top left.
	int m_ScrollY;    // Number of pixels from top left.

	unsigned int *m_TileProperty; // Allows for many unique properties (ex: water, sand, grass, walkable, non-walkable etc.)
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

	// Statistics counters
	static unsigned long int m_SC_CalculatedPaths;
	static unsigned long int m_SC_LongestCalculatedPath;
	static unsigned long int m_SC_FailedPathCalculations;

	// Rendering optimizations
	bool *m_MarkedForRendering;



public:
	MGMap();
	~MGMap();


	int getWidth(){ return m_Width; }
	int getHeight(){ return m_Height; }
	int getTileWidth(){ return m_TileWidth; }
	int getTileHeight(){ return m_TileHeight; }

	int getScrollX(){ return m_ScrollX; }
	int getScrollY(){ return m_ScrollY; }
	void setScrollOffset(int px, int py);
	void mouseScrollingRelease(int x, int y);
	void mouseScrollingClick(int x, int y);
	bool mouseScrollingUpdate(int x, int y);

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
	void setTileProperty(int x, int y, unsigned int value){ m_TileProperty[y * getWidth() + x]=value; }
	unsigned int getTileProperty(int x, int y){ return m_TileProperty[y * getWidth() + x]; }

	void occupy(int x, int y, int id)
	{ 
		m_Occupied[y * getWidth() + x] = id;
		//TODO: Should we also markForRendering(x, y); ???
	}

	void unOccupy(int x, int y)
	{ 
		m_Occupied[y * getWidth() + x] = 0;
		//TODO: Should we also markForRendering(x, y); ???
	}

	int occupant(int x, int y)
	{ 
		if(x >= 0 && y >= 0 && x < getWidth() && y < getHeight())
		{
			return m_Occupied[y * getWidth() + x];
		}
		return 0;
	}


	int getTileIndex(int clickX, int clickY);
	int getTileX(int index){ return index % getWidth(); }
	int getTileY(int index){ return (index - getTileX(index)) / getWidth(); }

	int getWindowHeight(){ return m_WindowHeight; }
	int getWindowWidth(){ return m_WindowWidth; }

	// Path related
	std::list<MGPathItem> calculatePath(eMGFPathType pathType, int ax, int ay, int bx, int by); // Calculates the path from A to B

	bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);

	// Statistics counters
	void printStatisticsCounters();
	unsigned long int getSCCalculatedPaths(){ return m_SC_CalculatedPaths; }
	unsigned long int getSCLongestCalculatedPath(){ return m_SC_LongestCalculatedPath; }
	unsigned long int getSCFailedPathCalculations(){ return m_SC_FailedPathCalculations; }

	// Rendering optimizations
	void unmarkForRendering(int x, int y)
	{
		if(x >= 0 && y >= 0 && x < getWidth() && y < getHeight())
		{
			m_MarkedForRendering[y * getWidth() + x] = false;
		}
	}

	void markForRendering(int x, int y)
	{ 
		if(x >= 0 && y >= 0 && x < getWidth() && y < getHeight())
		{
			m_MarkedForRendering[y * getWidth() + x] = true;
		}
	}

	void markForRendering(int i)
	{ 
		if(i >= 0 && i < getHeight() * getWidth())
		{
			m_MarkedForRendering[i] = true; 
		}
	}

	bool isMarkedForRendering(int x, int y)
	{
		return x >= 0 && y >= 0 && x < getWidth() && y < getHeight() && m_MarkedForRendering[y * getWidth() + x];
	}

};


#endif







