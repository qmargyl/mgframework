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

		void drawSprite(void* imageSurface, int srcX, int srcY, int dstX, int dstY, int width, int height){}
		void* loadBMPImage(std::string filename){ return (void*)0; }
		void drawText(const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB){}
		void putPixel32(int x, int y, unsigned int pixel){}
		unsigned int getPixel32(int x, int y){ return (unsigned int)0; }
		void drawCircle32(int n_cx, int n_cy, int radius, unsigned int pixel){}
		void drawFillCircle32(int cx, int cy, int radius, unsigned int pixel){}
		void vLine32(int x, int y, int length, unsigned int pixel){}
		void hLine32(int x, int y, int length, unsigned int pixel){}
};

#endif