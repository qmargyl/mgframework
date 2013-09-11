
#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL_ttf.h"
#include "mgwindow.h"
#include "mgframework.h"


MGWindow::MGWindow():
	m_Width(0),
	m_Height(0),
	m_Bpp(0),
	m_Fullscreen(false),
	m_Screen(NULL),
	m_Flags(0)
{
	// Nothing besides the initialization list..
}

MGWindow::~MGWindow()
{
	SDL_Quit();
}
	
bool MGWindow::createWindow()
{
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 ) 
	{		
		return false;
	}

	TTF_Init(); // To debug all TTF references must be commented out.

	// Set the title.
	SDL_WM_SetCaption(m_Title.c_str(), m_Title.c_str());

	// Flags tell SDL about the type of window we are creating.
	if(m_Fullscreen == true)
	{
		setFlags(SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE);
	}
	else
	{
		setFlags(SDL_DOUBLEBUF | SDL_HWSURFACE);
	}

	// Create the window
	m_Screen = SDL_SetVideoMode( m_Width, m_Height, m_Bpp, getFlags() );
		
	if(m_Screen == 0)
	{
		return false;
	}
		
	return true;
}

bool MGWindow::setProperties(int width, int height, int bpp, bool fullscreen, const string& title)
{
	//Copy the values incase we need them
	setSize(width, height);
	m_Title = title;
	m_Fullscreen = fullscreen;
	m_Bpp = bpp;
	setFlags(SDL_DOUBLEBUF | SDL_HWSURFACE);
	return true;
}

bool MGWindow::setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const string& title)
{
	switch(screenResolution)
	{
		case MGWindow_RES_1920_1080:
			setSize(1920, 1080);
			break;
		case MGWindow_RES_1600_900:
			setSize(1600, 900);
			break;
		case MGWindow_RES_1024_768:
			setSize(1024, 768);
			break;
		case MGWindow_RES_800_600:
			setSize(800, 600);
			break;
		case MGWindow_RES_640_480:
			setSize(640, 480);
			break;

		default:
			MGFLOG(std::cout << "ERROR: MGWindow::setProperties was given unsupported screen resolution" << std::endl;); 
			return false;

	}

	m_Title = title;
	m_Fullscreen = fullscreen;
	m_Bpp = bpp;
	setFlags(SDL_DOUBLEBUF | SDL_HWSURFACE);
	return true;
}

bool MGWindow::runConsoleCommand(const char *c, MGFramework *w)
{
	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
			MGFLOG(std::cout << "ERROR: MGWindow::runConsoleCommand received MGComponent_UNDEFINED from MGWindow::detectMGComponentConsoleCommand" << std::endl;); 
			break;

		case MGComponent_WINDOW_HELP:
		{
			return true;
		}

		case MGComponent_WINDOW_FULLSCREEN_ON:
		{
			m_Fullscreen = true;
			setFlags(SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE);
			m_Screen = SDL_SetVideoMode( m_Width, m_Height, m_Bpp, getFlags() );
			return false; // Deactivate console
		}

		case MGComponent_WINDOW_LOGGING_ON:
		{
			enableLogging();
			MGFPRINT(std::cout << "Logging enabled." << std::endl;);
			return true;
		}

		case MGComponent_WINDOW_LOGGING_OFF:
		{
			disableLogging();
			MGFPRINT(std::cout << "Logging disabled." << std::endl;);
			return true;
		}

		default:
			MGFLOG(std::cout << "ERROR: MGWindow::detectComponentConsoleCommand returned a bad value" << std::endl;); 
			return true;
	}

	MGFPRINT(std::cout << "Unknown command" << std::endl;);
	return true;
}

eMGComponentConsoleCommand MGWindow::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	if(cmdvec.size() == 2)
	{
		if(cmdvec[1]=="help")
		{
			return MGComponent_WINDOW_HELP;
		}
	}
	else if(cmdvec.size() == 3)
	{
		if(cmdvec[1]=="fullscreen" && cmdvec[2]=="on")
		{
			return MGComponent_WINDOW_FULLSCREEN_ON;
		}
		else if(cmdvec[1]=="logging" && cmdvec[2]=="on")
		{
			return MGComponent_WINDOW_LOGGING_ON;
		}
		else if(cmdvec[1]=="logging" && cmdvec[2]=="off")
		{
			return MGComponent_WINDOW_LOGGING_OFF;
		}
	}
	return MGComponent_UNDEFINED;
}