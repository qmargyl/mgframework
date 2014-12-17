#ifndef _MG_WINDOW_H
#define _MG_WINDOW_H

#include "mgcomponent.h"
#include "imgwindow.h"

#ifndef UNITTEST_LINUX
  #include "SDL.h"
  #include "SDL_thread.h"
  #ifndef MGF_DISABLE_TTF
    #include "SDL_ttf.h"
  #endif
#endif

class MGWindow : public IMGWindow
{
	private:
		int m_Width;
		int m_Height;
		int m_Bpp;
		bool m_Fullscreen;
		std::string m_Title;
#ifndef UNITTEST_LINUX
		SDL_Window* m_Screen;
		SDL_Renderer* m_Renderer;
  #ifndef MGF_DISABLE_TTF
		TTF_Font* m_Font;
  #endif
#endif

		int m_NDrawnSprites;
		inline void increaseDrawnSpritesCounter(){ ++m_NDrawnSprites; }
	public:

		MGWindow();
		~MGWindow();
			
		bool createWindow();
		bool setProperties(int width, int height, int bpp, bool fullscreen, const std::string &title);
		bool setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const std::string &title);
		void setSize(int width, int height){ m_Height = height; m_Width = width; }
		int getHeight() const { return m_Height; }
		int getWidth() const { return m_Width; }
		void flipSurface();
		void activateFullscreen();
		void deactivateFullscreen();

		void sleep(int ms);
		unsigned int getExecTimeMS() const;

		int getDrawnSpritesCounter() const { return m_NDrawnSprites; }
		void resetDrawnSpritesCounter(){ m_NDrawnSprites = 0; }

		void drawSprite(const MGTexHandle &imageTexture, int srcX, int srcY, int dstX, int dstY, int width, int height);
		void* loadBMPImage(std::string fileName, bool transparent);
		void loadBMPImage(std::string fileName, MGTexHandle &texHandle, bool transparent);
		void drawText(const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB);
		void putPixelRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b);
		void drawCircleRGB(int x0, int y0, int radius, unsigned char r, unsigned char g, unsigned char b);
		void drawRectangleRGB(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b);
		void drawFilledRectangleRGB(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b);
		void drawFilledRectangleRGBA(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};


#endif

