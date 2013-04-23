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


bool MGWindow::runConsoleCommand(const char *c)
{
	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');

	if(cmdvec.size() == 2)
	{
		if(cmdvec[1]=="help")
		{
			//std::cout << std::endl << "window help - Displays help information for console commands implemented in the window object." << std::endl;
			//std::cout << "window blendmode [none|blend|add|mod]- Sets the SDL_BlendMode of the main window surface to either SDL_BLENDMODE_NONE, SDL_BLENDMODE_BLEND, SDL_BLENDMODE_ADD or SDL_BLENDMODE_MOD. If the parameter is omitted the SDL_BlendMode is read out instead." << std::endl;
			return true;
		}
		std::cout << "Error in command (window <parameter>)" << std::endl;
		return true;
	}
	std::cout << "Error in command (window ...)" << std::endl;
	return true;
}
