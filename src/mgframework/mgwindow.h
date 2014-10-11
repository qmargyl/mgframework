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

	public:

		MGWindow();
		~MGWindow();
			
		bool createWindow();
		bool setProperties(int width, int height, int bpp, bool fullscreen, const std::string &title);
		bool setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const std::string &title);
		void setSize(int width, int height){ m_Height = height; m_Width = width; }
		int getHeight(){ return m_Height; }
		int getWidth(){ return m_Width; }
		void flipSurface();
		void activateFullscreen();
		void deactivateFullscreen();

		void drawSprite(void* imageSurface, int srcX, int srcY, int dstX, int dstY, int width, int height);
		void* loadBMPImage(std::string filename, bool transparent);
		void drawText(const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB);
		void putPixelRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b);
		void vLineRGB(int x, int y, int length, unsigned char r, unsigned char g, unsigned char b);
		void hLineRGB(int x, int y, int length, unsigned char r, unsigned char g, unsigned char b);

};


#endif

