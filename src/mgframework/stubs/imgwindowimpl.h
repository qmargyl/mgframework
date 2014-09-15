#ifndef _IMGWINDOWIMPL_H
#define _IMGWINDOWIMPL_H

#include "../imgwindow.h"

class IMGWindowImpl : public IMGWindowImpl
{
	public:
		bool createWindow(){ return true; }
		bool setProperties(int width, int height, int bpp, bool fullscreen, const std::string &title){ return true; }
		bool setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const std::string &title){ return true; }
		void setSize(int width, int height){}
		int getHeight(){ return 0; }
		int getWidth(){ return 0; }
		void flipSurface(){}
};