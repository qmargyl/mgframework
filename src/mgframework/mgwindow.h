#ifndef _MG_WINDOW_H
#define _MG_WINDOW_H

#include "mgcomponent.h"

#ifndef UNITTEST_LINUX
// SDL
  #include <SDL/SDL.h>
  #include <SDL/SDL_thread.h>

  #ifndef MGF_DISABLE_TTF
    #include "SDL_ttf.h"
  #endif
#endif

using std::string;

enum eMGWindowScreenResolution{
	// Wide screen resolutions (16:9)
	MGWindow_RES_1920_1080 = 0,
	MGWindow_RES_1600_900,
	// Normal screen resolutions (4:3)
	MGWindow_RES_1024_768,
	MGWindow_RES_800_600,
	MGWindow_RES_640_480
};

class MGWindow :public MGComponent
{
	private:
		int m_Width;
		int m_Height;
		int m_Bpp;
		bool m_Fullscreen;
		string m_Title;
		int m_Flags;
#ifndef UNITTEST_LINUX
		SDL_Surface * m_Screen;
		// Font
  #ifndef MGF_DISABLE_TTF
		TTF_Font* m_Font;
  #endif
#endif
		void setFlags(int flags){ m_Flags = flags; }
		int getFlags(){ return m_Flags; }
			
	public:

		MGWindow();
		~MGWindow();
			
		bool createWindow();

		bool setProperties(int width, int height, int bpp, bool fullscreen, const string& title);
		bool setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const string& title);

		void setSize(int width, int height){ m_Height = height; m_Width = width; }
		int getHeight(){ return m_Height; }
		int getWidth(){ return m_Width; }

		bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
		eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);
#ifndef UNITTEST_LINUX
		SDL_Surface *getSurface();
		void flipSurface();
		void drawSprite(SDL_Surface* imageSurface, int srcX, int srcY, int dstX, int dstY, int width, int height);
		SDL_Surface *loadBMPImage( std::string filename );
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

