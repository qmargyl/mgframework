
#include "mgmap.h"
#include <stdlib.h>
#include "mgframework.h"


MGMap::MGMap()
{
	m_TileProperty = NULL;
	m_MouseScrollingOngoing = false;
	m_MouseScrollingXClick=0;
	m_MouseScrollingYClick=0;
}

MGMap::~MGMap()
{
	if(m_TileProperty != NULL)
	{
		delete[] m_TileProperty;
	}
}

int MGMap::getWidth()
{
	return m_Width;
}

int MGMap::getHeight()
{
	return m_Height;
}
	
int MGMap::getTileWidth()
{
	return m_TileWidth;
}

int MGMap::getTileHeight()
{
	return m_TileHeight;
}

int MGMap::getScrollX()
{
	return m_ScrollX;
}

int MGMap::getScrollY()
{
	return m_ScrollY;
}

void MGMap::setScrollOffset(int px, int py)
{
	m_ScrollX = px;
	m_ScrollY = py;
}

void MGMap::init(int w, int h, int tw, int th, int windowWidth, int windowHeight)
{
	m_TileProperty = new int[w*h];
	m_Width = w;
	m_Height = h;
	m_TileWidth = tw;
	m_TileHeight = th;
	setScrollOffset(0, 0);
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;

	// The proper way to populate the tile properties would be to 
	// for example read in a level file, or set hard coded values.
	// Here is just an example so we have something before the
	// framework can load a file.. Obviously the hardcoded tile
	// properties should not be part of the framework.

	for (int x=0; x < getWidth(); x++)
	{
		for (int y=0; y < getHeight(); y++)
		{
			setTileProperty(x, y, 0);
		}
	}

	// Just an example...
	setTileProperty(3, 3, 1);
	setTileProperty(4, 4, 1);
	setTileProperty(5, 5, 1);
	setTileProperty(6, 4, 1);
	setTileProperty(7, 3, 1);
	setTileProperty(8, 2, 1);

}

void MGMap::setTileProperty(int x, int y, int value)
{
	m_TileProperty[y*getWidth()+x]=value;
}

int MGMap::getTileProperty(int x, int y)
{
	return m_TileProperty[y*getWidth()+x];
}


/***************************************************
 * int MGMap::getTileIndex(int clickX, int clickY)
 * Returns tile index of clicked tile. If click
 * was outside of the map then -1 will be returned.
 ***************************************************/
int MGMap::getTileIndex(int clickX, int clickY)
{
	int x = (clickX - getScrollX()) / getTileWidth();
	int y = (clickY - getScrollY()) / getTileHeight();
	if((x < getWidth()) && (y < getHeight()))
	{
		return y * getWidth() + x;
	}
	else
	{
		return -1;
	}
}

int MGMap::getTileX(int index)
{
	return index % getWidth();
}

int MGMap::getTileY(int index)
{
	return (index - getTileX(index))/getWidth();
}

int MGMap::getWindowHeight()
{
	return m_WindowHeight;
}

int MGMap::getWindowWidth()
{
	return m_WindowWidth;
}

void MGMap::save()
{
	return ;
}

void MGMap::mouseScrollingRelease(int x, int y)
{
	m_MouseScrollingOngoing = false;
}

void MGMap::mouseScrollingClick(int x, int y)
{
	m_MouseScrollingOngoing = true;
	m_MouseScrollingXClick=x;
	m_MouseScrollingYClick=y;
}

void MGMap::mouseScrollingUpdate(int x, int y)
{
	// Possible optimization by re-using x and y variables and making the method inline.
	if(m_MouseScrollingOngoing)
	{
		int setY = getScrollY() + m_MouseScrollingYClick - y;
		int setX = getScrollX() + m_MouseScrollingXClick - x;

		if(setY > 0)
		{
			setY = 0;
		}
		if(setY < getWindowHeight() - getHeight() * getTileHeight())
		{
			setY = getWindowHeight() - getHeight() * getTileHeight();
		}

		if(setX > 0)
		{
			setX = 0;
		}
		if(setX < getWindowWidth() - getWidth() * getTileWidth())
		{
			setX = getWindowWidth() - getWidth() * getTileWidth();
		}

		setScrollOffset(setX, setY);
	}
}
