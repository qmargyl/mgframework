#ifndef _IMGWINDOWIMPL_H
#define _IMGWINDOWIMPL_H

#include "../imgwindow.h"

class IMGWindowImpl : public IMGWindow
{
	private:
		unsigned int m_flipSurfaceCounter;
		unsigned int m_activateFullscreenCounter;
		unsigned int m_deactivateFullscreenCounter;
	public:
		IMGWindowImpl()
		:	m_flipSurfaceCounter(0),
			m_activateFullscreenCounter(0),
			m_deactivateFullscreenCounter(0)
		{}
		bool createWindow(){ return true; }
		bool setProperties(int width, int height, int bpp, bool fullscreen, const std::string &title){ return true; }
		bool setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const std::string &title){ return true; }
		void setSize(int width, int height){}
		int getHeight(){ return 768; }
		int getWidth(){ return 1024; }

		void flipSurface(){ m_flipSurfaceCounter++; }
		unsigned int getFlipSurfaceCounter(){ return m_flipSurfaceCounter; }

		void activateFullscreen(){ m_activateFullscreenCounter++; }
		unsigned int getActivateFullscreenCounter(){ return m_activateFullscreenCounter; }

		void deactivateFullscreen(){ m_deactivateFullscreenCounter++; }
		unsigned int getDeactivateFullscreenCounter(){ return m_deactivateFullscreenCounter; }
};

#endif