#ifndef _MG_WINDOW_H
#define _MG_WINDOW_H

#include "mgcomponent.h"

struct SDL_Surface;

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

		void setFlags(int flags){ m_Flags = flags; }
		int getFlags(){ return m_Flags; }
			
	public:

		SDL_Surface * m_Screen;

		MGWindow();
		~MGWindow();
			
		bool createWindow();

		bool setProperties(int width, int height, int bpp, bool fullscreen, const string& title);
		bool setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const string& title);

		void setSize(int width, int height){ m_Height = height; m_Width = width; }
		int getHeight(){ return m_Height; }
		int getWidth(){ return m_Width; }

		bool runConsoleCommand(const char *c, MGFramework *w);
		eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);

};


#endif

