#ifndef _IMGWINDOWIMPL_H
#define _IMGWINDOWIMPL_H

#include "../imgwindow.h"

class IMGWindowImpl : public IMGWindow
{
	private:
		unsigned int m_flipSurfaceCounter;
		unsigned int m_activateFullscreenCounter;
		unsigned int m_deactivateFullscreenCounter;

		// Simulated time
		unsigned int m_currentTime;
	public:
		IMGWindowImpl()
		:	m_flipSurfaceCounter(0),
			m_activateFullscreenCounter(0),
			m_deactivateFullscreenCounter(0),
			m_currentTime(0)
		{}
		bool createWindow(){ return true; }
		bool setProperties(int width, int height, int bpp, bool fullscreen, const std::string &title){ return true; }
		bool setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const std::string &title){ return true; }
		void setSize(int width, int height){}
		int getHeight() const { return 768; }
		int getWidth() const { return 1024; }

		void flipSurface(){ m_flipSurfaceCounter++; }
		unsigned int getFlipSurfaceCounter(){ return m_flipSurfaceCounter; }

		void activateFullscreen(){ m_activateFullscreenCounter++; }
		unsigned int getActivateFullscreenCounter(){ return m_activateFullscreenCounter; }

		void deactivateFullscreen(){ m_deactivateFullscreenCounter++; }
		unsigned int getDeactivateFullscreenCounter(){ return m_deactivateFullscreenCounter; }

		void sleep(int ms){}
		unsigned int getExecTimeMS() const { return m_currentTime; }
		void elapseTime(unsigned int ms){ m_currentTime += ms; }

		int getDrawnSpritesCounter() const { return 0; }
		void resetDrawnSpritesCounter(){}

		void drawSprite(const MGTexHandle &imageTexture, int srcX, int srcY, int dstX, int dstY, int width, int height){}
		void* loadBMPImage(std::string filename, bool transparent){ return (void*)0; }
		void loadBMPImage(std::string fileName, MGTexHandle &texHandle, bool transparent){}
		void drawText(const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB){}
		void putPixelRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b){}
		void drawCircleRGB(int x0, int y0, int radius, unsigned char r, unsigned char g, unsigned char b){}
		void drawRectangleRGB(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b){}
		void drawFilledRectangleRGB(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b){}
		void drawFilledRectangleRGBA(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a){}
};

#endif