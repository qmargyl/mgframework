#ifndef _MG_WINDOW_H
#define _MG_WINDOW_H

#include "mgcomponent.h"
#include "imgwindow.h"

#ifndef UNITTEST_LINUX
// SDL
  #include <SDL/SDL.h>
  #include <SDL/SDL_thread.h>

  #ifndef MGF_DISABLE_TTF
    #include "SDL_ttf.h"
  #endif
#endif

class MGWindow :public MGComponent
{
	private:
		int m_Width;
		int m_Height;
		int m_Bpp;
		bool m_Fullscreen;
		std::string m_Title;
#ifndef UNITTEST_LINUX
		SDL_Surface * m_Screen;
  #ifndef MGF_DISABLE_TTF
		TTF_Font* m_Font;
  #endif
#endif
		int m_Flags;

		void setFlags(int flags){ m_Flags = flags; }
		int getFlags(){ return m_Flags; }
			
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

		bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
		eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);


		void drawSprite(void* imageSurface, int srcX, int srcY, int dstX, int dstY, int width, int height);
#ifndef UNITTEST_LINUX
		SDL_Surface *loadBMPImage(std::string filename);
		void drawText(const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB);
		void putPixel32(int x, int y, Uint32 pixel);
		Uint32 getPixel32(int x, int y);
		void drawCircle32(int n_cx, int n_cy, int radius, Uint32 pixel);
		void drawFillCircle32(int cx, int cy, int radius, Uint32 pixel);
		void vLine32(int x, int y, int length, Uint32 pixel);
		void hLine32(int x, int y, int length, Uint32 pixel);
#endif

};


#endif

