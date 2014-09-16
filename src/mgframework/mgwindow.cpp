
#include "mgwindow.h"
#include <string>
#include <cstring>
#include <iostream>
#ifndef UNITTEST_LINUX
	#include <SDL/SDL_opengl.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#include "mgframework.h"


MGWindow::MGWindow():
	m_Width(0),
	m_Height(0),
	m_Bpp(0),
	m_Fullscreen(false),
#ifndef UNITTEST_LINUX
	m_Screen(NULL),
  #ifndef MGF_DISABLE_TTF
	m_Font(0),
  #endif
#endif
	m_Flags(0)
{
}

MGWindow::~MGWindow()
{
#ifndef UNITTEST_LINUX
	SDL_Quit();
#endif
}
	
bool MGWindow::createWindow()
{
#ifndef UNITTEST_LINUX
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 ) 
	{		
		return false;
	}

#ifndef MGF_DISABLE_TTF
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
#endif	
	return true;
}

bool MGWindow::setProperties(int width, int height, int bpp, bool fullscreen, const std::string &title)
{
	setSize(width, height);
	m_Title = title;
	m_Fullscreen = fullscreen;
	m_Bpp = bpp;
#ifndef UNITTEST_LINUX
	setFlags(SDL_DOUBLEBUF | SDL_HWSURFACE);
#endif
	return true;
}

bool MGWindow::setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const std::string &title)
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
#ifndef UNITTEST_LINUX
	setFlags(SDL_DOUBLEBUF | SDL_HWSURFACE);
#endif
	return true;
}


#ifndef UNITTEST_LINUX
SDL_Surface *MGWindow::getSurface()
{
	return m_Screen;
}
#endif

void MGWindow::flipSurface()
{
#ifndef UNITTEST_LINUX
	SDL_Flip(m_Screen);
#endif
}

void MGWindow::activateFullscreen()
{
#ifndef UNITTEST_LINUX
	m_Fullscreen = true;
	setFlags(SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE);
	m_Screen = SDL_SetVideoMode(m_Width, m_Height, m_Bpp, getFlags());
#endif
}

void MGWindow::deactivateFullscreen()
{
#ifndef UNITTEST_LINUX
	m_Fullscreen = false;
	setFlags(SDL_DOUBLEBUF | SDL_HWSURFACE);
	m_Screen = SDL_SetVideoMode(m_Width, m_Height, m_Bpp, getFlags());
#endif
}

#ifndef UNITTEST_LINUX
void MGWindow::drawSprite(SDL_Surface* imageSurface, int srcX, int srcY, int dstX, int dstY, int width, int height)
{
	
	SDL_Rect srcRect;
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;
	SDL_Rect dstRect;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = width;
	dstRect.h = height;
	SDL_BlitSurface(imageSurface, &srcRect, m_Screen, &dstRect);
}
#endif

#ifndef UNITTEST_LINUX
SDL_Surface *MGWindow::loadBMPImage( std::string filename ) 
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = SDL_LoadBMP( filename.c_str() );
	if( loadedImage != NULL )
	{
		optimizedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
	}
	return optimizedImage;
}
#endif

#ifndef UNITTEST_LINUX
void MGWindow::drawText(const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB)
{
#ifndef MGF_DISABLE_TTF
	m_Font = TTF_OpenFont("ARIAL.TTF", size);
	SDL_Color foregroundColor = { fR, fG, fB };
	SDL_Color backgroundColor = { bR, bG, bB };
	SDL_Surface* textSurface = TTF_RenderText_Shaded(m_Font, string, foregroundColor, backgroundColor);
	SDL_Rect textLocation = { x, y, 0, 0 };
	SDL_BlitSurface(textSurface, NULL, m_Screen, &textLocation);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(m_Font);
#endif
}
#endif

#ifndef UNITTEST_LINUX
void MGWindow::putPixel32(int x, int y, Uint32 pixel)
{
	Uint32 *pixels = (Uint32 *)m_Screen->pixels;
	pixels[ ( y * m_Screen->w ) + x ] = pixel;
}
#endif

#ifndef UNITTEST_LINUX
Uint32 MGWindow::getPixel32(int x, int y)
{
	Uint32 *pixels = (Uint32 *)m_Screen->pixels;
	return pixels[ ( y * m_Screen->w ) + x ];
}
#endif

#ifndef UNITTEST_LINUX
void MGWindow::drawCircle32(int n_cx, int n_cy, int radius, Uint32 pixel)
{
    // if the first pixel in the screen is represented by (0,0) (which is in sdl)
    // remember that the beginning of the circle is not in the middle of the pixel
    // but to the left-top from it:
 
    double error = (double)-radius;
    double x = (double)radius -0.5;
    double y = (double)0.5;
    double cx = n_cx - 0.5;
    double cy = n_cy - 0.5;
 
    while (x >= y)
    {
        putPixel32((int)(cx + x), (int)(cy + y), pixel);
        putPixel32((int)(cx + y), (int)(cy + x), pixel);
 
        if (x != 0)
        {
            putPixel32((int)(cx - x), (int)(cy + y), pixel);
            putPixel32((int)(cx + y), (int)(cy - x), pixel);
        }
 
        if (y != 0)
        {
            putPixel32((int)(cx + x), (int)(cy - y), pixel);
            putPixel32((int)(cx - y), (int)(cy + x), pixel);
        }
 
        if (x != 0 && y != 0)
        {
            putPixel32((int)(cx - x), (int)(cy - y), pixel);
            putPixel32((int)(cx - y), (int)(cy - x), pixel);
        }
 
        error += y;
        ++y;
        error += y;
 
        if (error >= 0)
        {
            --x;
            error -= x;
            error -= x;
        }
    }
}
#endif

#ifndef UNITTEST_LINUX
void MGWindow::drawFillCircle32(int cx, int cy, int radius, Uint32 pixel)
{
    static const int BPP = 4;
    double r = (double)radius;
    for (double dy = 1; dy <= r; dy += 1.0)
    {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        int x = cx - (int)dx;
 
        // Grab a pointer to the left-most pixel for each half of the circle
        Uint8 *target_pixel_a = (Uint8 *)m_Screen->pixels + ((int)(cy + r - dy)) * m_Screen->pitch + x * BPP;
        Uint8 *target_pixel_b = (Uint8 *)m_Screen->pixels + ((int)(cy - r + dy)) * m_Screen->pitch + x * BPP;
 
        for (; x <= cx + dx; x++)
        {
            *(Uint32 *)target_pixel_a = pixel;
            *(Uint32 *)target_pixel_b = pixel;
            target_pixel_a += BPP;
            target_pixel_b += BPP;
        }
    }
}
#endif

#ifndef UNITTEST_LINUX
void MGWindow::vLine32(int x, int y, int length, Uint32 pixel)
{
	for(int i=y; i<y+length; i++)
	{
		putPixel32(x, i, pixel);
	}
}
#endif

#ifndef UNITTEST_LINUX
void MGWindow::hLine32(int x, int y, int length, Uint32 pixel)
{
	for(int i=x; i<x+length; i++)
	{
		putPixel32(i, y, pixel);
	}
}
#endif

bool MGWindow::runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s)
{
	char cmd[MGF_SCRIPTLINE_MAXLENGTH];
	strcpy(cmd, c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, " ");

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
			activateFullscreen();
			return false; // Deactivate console
		}

		case MGComponent_WINDOW_FULLSCREEN_OFF:
		{
			w->registerUsedCommand(MGComponent_WINDOW_FULLSCREEN_OFF);
			deactivateFullscreen();
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