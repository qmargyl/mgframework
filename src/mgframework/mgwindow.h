#ifndef _MG_WINDOW_H
#define _MG_WINDOW_H

#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL_ttf.h"
#include "mgcomponent.h"

using std::string;

class MGWindow :public MGComponent
{
	private:
		int m_Width;
		int m_Height;
		int m_Bpp;
		bool m_Fullscreen;
		string m_Title;
			
	public:

		SDL_Surface * m_Screen;

		MGWindow();
		~MGWindow();
			
		bool createWindow();
		bool setProperties(int width, int height, int bpp, bool fullscreen, const string& title);

		void setSize(int width, int height);
		int getHeight();
		int getWidth();

		bool runConsoleCommand(const char *c, MGFramework *w);
};


#endif

