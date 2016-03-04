
#include "mgwindow.h"
#include <string>
#include <cstring>
#include <iostream>
#include "SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "mgframework.h"

extern "C" { FILE __iob_func[3] = { *stdin, *stdout, *stderr }; }

MGTexHandle::MGTexHandle()
{
	tex = NULL;
}

MGTexHandle::~MGTexHandle()
{
	if(tex)
	{
		SDL_DestroyTexture(static_cast<SDL_Texture*>(tex));
	}
}


MGWindow::MGWindow():
	m_Width(0),
	m_Height(0),
	m_Bpp(0),
	m_Fullscreen(false),
	m_NDrawnSprites(0),
#ifndef MGF_DISABLE_TTF
	m_Font(0),
#endif
	m_Screen(NULL)
{
}

MGWindow::~MGWindow()
{
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Screen);
#ifndef MGF_DISABLE_TTF
	TTF_CloseFont(m_Font);
	TTF_Quit();
#endif
	SDL_Quit();
}

bool MGWindow::createWindow()
{
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 ) 
	{		
		return false;
	}

#ifndef MGF_DISABLE_TTF
	TTF_Init(); // To debug all TTF references must be commented out.
#endif

	if(m_Fullscreen == true)
	{
		m_Screen = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Width, m_Height, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		m_Screen = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Width, m_Height, 0);
	}

	// Create the window
	m_Renderer = SDL_CreateRenderer(m_Screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 255);
		
	if(m_Screen == 0)
	{
		return false;
	}

#ifndef MGF_DISABLE_TTF
	m_Font = TTF_OpenFont("ARIAL.TTF", 16);
#endif

	return true;
}

bool MGWindow::setProperties(int width, int height, int bpp, bool fullscreen, const std::string &title)
{
	setSize(width, height);
	m_Title = title;
	m_Fullscreen = fullscreen;
	m_Bpp = bpp;
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
			std::cout << "ERROR: MGWindow::setProperties was given unsupported screen resolution" << std::endl; 
			return false;
	}

	m_Title = title;
	m_Fullscreen = fullscreen;
	m_Bpp = bpp;
	return true;
}


void MGWindow::flipSurface()
{
	SDL_RenderPresent(m_Renderer);
}

void MGWindow::activateFullscreen()
{
	m_Fullscreen = true;
	SDL_SetWindowFullscreen(m_Screen, SDL_WINDOW_FULLSCREEN);
}

void MGWindow::deactivateFullscreen()
{
	m_Fullscreen = false;
	SDL_SetWindowFullscreen(m_Screen, 0);
}

void MGWindow::sleep(int ms)
{
	if(ms > 0)
	{
		SDL_Delay((Uint32)ms);
	}
}

unsigned int MGWindow::getExecTimeMS() const
{
	return SDL_GetTicks();
}

void MGWindow::drawSprite(const MGTexHandle &imageTexture, int srcX, int srcY, int dstX, int dstY, int width, int height)
{
	increaseDrawnSpritesCounter();
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
	SDL_RenderCopy(m_Renderer, static_cast<SDL_Texture*>(imageTexture.tex), &srcRect, &dstRect);
}


void* MGWindow::loadBMPImage(std::string fileName, bool transparent) 
{
	SDL_Surface* loadedImage = NULL;
	SDL_Texture* optimizedImage = NULL;
	loadedImage = SDL_LoadBMP(fileName.c_str());
	if(loadedImage != NULL)
	{
		if(transparent)
		{
			// TODO: Make it possible to have other color codes than
			// zero represent transparency
			SDL_SetColorKey(loadedImage, SDL_TRUE, 0);
		}
		optimizedImage = SDL_CreateTextureFromSurface(m_Renderer, loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	return (void*)optimizedImage;
}


void MGWindow::loadBMPImage(std::string fileName, MGTexHandle &texHandle, bool transparent)
{
	if(texHandle.tex)
	{
		SDL_DestroyTexture(static_cast<SDL_Texture*>(texHandle.tex));
	}
	texHandle.tex = loadBMPImage(fileName, transparent);
}


void MGWindow::drawText(const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB)
{
#ifndef MGF_DISABLE_TTF
	SDL_Color foregroundColor = {static_cast<Uint8>(fR), static_cast<Uint8>(fG), static_cast<Uint8>(fB)};
	SDL_Color backgroundColor = {static_cast<Uint8>(bR), static_cast<Uint8>(bG), static_cast<Uint8>(bB)};
	SDL_Surface* textSurface = TTF_RenderText_Shaded(m_Font, string, foregroundColor, backgroundColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_Renderer, textSurface);
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(textTexture, NULL, NULL, &texW, &texH);
	SDL_Rect textLocation = {x, y, texW, texH};
	SDL_RenderCopy(m_Renderer, textTexture, NULL, &textLocation);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
#endif
}


void MGWindow::putPixelRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(m_Renderer, (Uint8)r, (Uint8)g, (Uint8)b, 255);
	SDL_RenderDrawPoint(m_Renderer, x, y);
}

void MGWindow::drawCircleRGB(int x0, int y0, int radius, unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(m_Renderer, (Uint8)r, (Uint8)g, (Uint8)b, 255);
	int x = radius;
	int y = 0;
	int radiusError = 1 - x;

	while(x >= y)
	{
		SDL_RenderDrawPoint(m_Renderer, x + x0, y + y0);
		SDL_RenderDrawPoint(m_Renderer, y + x0, x + y0);
		SDL_RenderDrawPoint(m_Renderer, -x + x0, y + y0);
		SDL_RenderDrawPoint(m_Renderer, -y + x0, x + y0);
		SDL_RenderDrawPoint(m_Renderer, -x + x0, -y + y0);
		SDL_RenderDrawPoint(m_Renderer, -y + x0, -x + y0);
		SDL_RenderDrawPoint(m_Renderer, x + x0, -y + y0);
		SDL_RenderDrawPoint(m_Renderer, y + x0, -x + y0);
		y++;
		if(radiusError < 0)
		{
			radiusError += 2 * y + 1;
		}
		else
		{
			x--;
			radiusError += 2 * (y - x + 1);
		}
	}
}

void MGWindow::drawRectangleRGB(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, 255);
	SDL_Rect dstRect;
	dstRect.x = x1;
	dstRect.y = y1;
	dstRect.w = x2 - x1;
	dstRect.h = y2 - y1;
	SDL_RenderDrawRect(m_Renderer, &dstRect);
}

void MGWindow::drawFilledRectangleRGB(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, 255);
	SDL_Rect dstRect;
	dstRect.x = x1;
	dstRect.y = y1;
	dstRect.w = x2 - x1;
	dstRect.h = y2 - y1;
	SDL_RenderFillRect(m_Renderer, &dstRect);
}

void MGWindow::drawFilledRectangleRGBA(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
	SDL_Rect dstRect;
	dstRect.x = x1;
	dstRect.y = y1;
	dstRect.w = x2 - x1;
	dstRect.h = y2 - y1;
	SDL_RenderFillRect(m_Renderer, &dstRect);
}