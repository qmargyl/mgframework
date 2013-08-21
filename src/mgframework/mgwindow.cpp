
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
	m_Screen(NULL)
{
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
	int flags = SDL_DOUBLEBUF | SDL_HWSURFACE;
		
	if(m_Fullscreen == true)
	{
		flags |= SDL_FULLSCREEN;
	}

	// Create the window
	m_Screen = SDL_SetVideoMode( m_Width, m_Height, m_Bpp, flags );
		
	if(m_Screen == 0)
	{
		return false;
	}
		
	return true;
}

bool MGWindow::setProperties(int width, int height, int bpp, bool fullscreen, const string& title)
{
	//Copy the values incase we need them
	m_Height = height;
	m_Width = width;
	m_Title = title;
	m_Fullscreen = fullscreen;
	m_Bpp = bpp;
	return true;
}

void MGWindow::setSize(int width, int height)
{
	m_Height = height;
	m_Width = width;
}
	
int MGWindow::getHeight()
{
	return m_Height;
}
	
int MGWindow::getWidth()
{
	return m_Width;
}


bool MGWindow::runConsoleCommand(const char *c, MGFramework *w)
{
	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
			MGFPRINT(std::cout << "Error in command (window ...), MGComponent_UNDEFINED received from MGWindow::detectMGComponentConsoleCommand" << std::endl;); 
			break;

		case MGComponent_WINDOW_HELP:
		{
			return true;
		}

		case MGComponent_WINDOW_FULLSCREEN_ON:
		{
			m_Fullscreen = true;
			int flags = SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN;
			m_Screen = SDL_SetVideoMode( m_Width, m_Height, m_Bpp, flags );
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