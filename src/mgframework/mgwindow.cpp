
#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "mgwindow.h"
#include "mgframework.h"
#ifndef MGF_DEBUGGING_ENABLED
#include "SDL_ttf.h"
#endif

MGWindow::MGWindow():
	m_Width(0),
	m_Height(0),
	m_Bpp(0),
	m_Fullscreen(false),
	m_Screen(NULL),
	m_Flags(0)
{
	registerMemoryAllocation(sizeof(MGWindow));
}

MGWindow::~MGWindow()
{
	registerMemoryDeallocation(sizeof(MGWindow));
	SDL_Quit();
}
	
bool MGWindow::createWindow()
{
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 ) 
	{		
		return false;
	}

#ifndef MGF_DEBUGGING_ENABLED
	TTF_Init(); // To debug all TTF references must be commented out.
#endif

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
			MGFLOG_ERROR("MGWindow::setProperties was given unsupported screen resolution"); 
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
			MGFLOG_ERROR("MGWindow::runConsoleCommand received MGComponent_UNDEFINED from MGWindow::detectMGComponentConsoleCommand"); 
			break;

		case MGComponent_WINDOW_HELP:
		{
			w->registerUsedCommand(MGComponent_WINDOW_HELP);
			return true;
		}

		case MGComponent_WINDOW_FULLSCREEN_ON:
		{
			w->registerUsedCommand(MGComponent_WINDOW_FULLSCREEN_ON);
			m_Fullscreen = true;
			setFlags(SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE);
			m_Screen = SDL_SetVideoMode( m_Width, m_Height, m_Bpp, getFlags() );
			return false; // Deactivate console
		}

		case MGComponent_WINDOW_FULLSCREEN_OFF:
		{
			w->registerUsedCommand(MGComponent_WINDOW_FULLSCREEN_OFF);
			m_Fullscreen = false;
			setFlags(SDL_DOUBLEBUF | SDL_HWSURFACE);
			m_Screen = SDL_SetVideoMode( m_Width, m_Height, m_Bpp, getFlags() );
			return true;
		}

		case MGComponent_WINDOW_LOGGING_ON:
		{
			w->registerUsedCommand(MGComponent_WINDOW_LOGGING_ON);
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			return true;
		}

		case MGComponent_WINDOW_LOGGING_OFF:
		{
			w->registerUsedCommand(MGComponent_WINDOW_LOGGING_OFF);
			MGFLOG_INFO("Logging disabled.");
			disableLogging();
			return true;
		}

		default:
			MGFLOG_ERROR("MGWindow::detectComponentConsoleCommand returned a bad value"); 
			return true;
	}

	std::cout << "Unknown command" << std::endl;
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
		if(cmdvec[1]=="fullscreen" && cmdvec[2]=="off")
		{
			return MGComponent_WINDOW_FULLSCREEN_OFF;
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