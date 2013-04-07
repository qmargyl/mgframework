#include "mgframework.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;


MGFramework::MGFramework()
{
	m_NMO=0;
	setDesiredFPS(1);
	m_FrameTime=0;
	//m_FrameTime = (Uint32)(1000/getDesiredFPS()); // Initial FPS value of 60...
	m_FrameCountdownEnabled = false;
	m_FrameNumber = 0;
	std::srand((unsigned)std::time(0));
}

MGFramework::~MGFramework()
{
}

void MGFramework::resize(int x, int y)
{
	// Not implemented yet
}

bool MGFramework::processEvents()
{
	// Even processing loop is started after all special cases.
	SDL_Event event;
	while (SDL_PollEvent(&event))//get all events
	{
		switch (event.type)
		{
			// Quit event
			case SDL_QUIT:
			{
				if(loggingEnabled())
				{
					std::cout << "SDL_QUIT" << std::endl;
				}
				// Return false because we are quitting.
				return false;
			}

			case SDL_KEYDOWN:
			{
				if(!typingEnabled())
				{
					SDLKey sym = event.key.keysym.sym;
					if(sym == SDLK_ESCAPE) //Enable typing in the console if ESC is pressed..
					{
						activateConsole();
					}
					m_Keys[sym] = 1;
				}
				break;
			}

			case SDL_KEYUP:
			{
				if(!typingEnabled())
				{
					SDLKey sym = event.key.keysym.sym;

					if(loggingEnabled())
					{
						std::cout << "SDL_KEYUP" << std::endl << "  " << SDL_GetKeyName(sym) << std::endl;
					}

					m_Keys[sym] = 0;
				}
				break;
			}

			case SDL_VIDEORESIZE:
			{
				if(loggingEnabled())
				{
					std::cout << "SDL_VIDEORESIZE" << std::endl << "  " << "Resizing Window to " << event.resize.w << "x" << event.resize.h << std::endl;
				}
				//Not implemented yet
				//The window has been resized so we need to set up our viewport and projection according to the new size
				resize(event.resize.w, event.resize.h);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				// Calculate index, x and y for the tile that was clicked in the map.
				int iClick = m_Map.getTileIndex(event.button.x, event.button.y);
				if (iClick >= 0)
				{
					int xClick = m_Map.getTileX(iClick);
					int yClick = m_Map.getTileY(iClick);
					if (((int) event.button.button) == 1)
					{
						//Left click actions here...
						//m_Map.setTileProperty(xClick, yClick, 1);
						//m_MO1.setDestTileXY(xClick, yClick);

					}
					else if (((int) event.button.button) == 3)
					{
						m_Map.mouseScrollingClick(event.button.x, event.button.y);
					}

					// Log debug information to console.
					if(loggingEnabled())
					{
						std::cout << "SDL_MOUSEBUTTONDOWN" << std::endl << "  " << "Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")" << std::endl;
						if (((int) event.button.button) == 1)
						{
							std::cout << "Map (left click): index = " << iClick << ", x = " << xClick << ", y = " << yClick << std::endl;
						}
						else if (((int) event.button.button) == 3)
						{
							std::cout << "Map (right click): index = " << iClick << ", x = " << xClick << ", y = " << yClick << std::endl;
						}
					}
				}
				else
				{
					if(loggingEnabled())
					{
						std::cout << "SDL_MOUSEBUTTONDOWN" << std::endl << "  " << "Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")" << std::endl;

						if (((int) event.button.button) == 1)
						{
							std::cout << "Map (left click): index = -1, x = <not calculated>, y = <not calculated>" << std::endl;
						}
						else if (((int) event.button.button) == 3)
						{
							std::cout << "Map (right click): index = -1, x = <not calculated>, y = <not calculated>" << std::endl;
						}
					}
				}
				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				if(loggingEnabled())
				{
					std::cout << "SDL_MOUSEBUTTONUP" << std::endl << "  " << "Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")" << std::endl;
				}
				if (((int) event.button.button) == 3)
				{
					m_Map.mouseScrollingRelease(event.button.x, event.button.y);
				}
				break;
			}

			case SDL_MOUSEMOTION:
			{
				m_Map.mouseScrollingUpdate(event.motion.x, event.motion.y);
				break;
			}

			// Default case for unknown events
			default:
			{
				break;
			}
		}
	}
	return true;
}

void MGFramework::run()
{
	Uint32 frameStartTime = 0; 
	m_DelayTime = 0;
	Uint32 lastFrameTime = SDL_GetTicks()-1; // SDL_GetTicks() - lastFrameTime cannot be zero.

	while(processEvents())
	{
		// Calculate the current frame time (and implicitly FPS)..
		m_FrameTime = SDL_GetTicks() - lastFrameTime; //number of milli seconds from last frame was handled
		lastFrameTime = SDL_GetTicks();
		frameStartTime = SDL_GetTicks();

		// Handle all calculations and drawing of current frame..


		//If frame countdown is enabled (a.k.a. we are counting down frames to go into console again)
		//and the counter is zero, activate the console.
		if(frameCountdownEnabled())
		{
			static int sleepKPI = 0;
			static int nFrames = 0;
			if(getFrameNumber() == 0)
			{
				std::cout << "Frame countdown results: " << std::endl;
				std::cout << "\tSum of frame sleep time: " << sleepKPI << std::endl;
				std::cout << "\tNumber of frames executed: " << nFrames << std::endl;
				std::cout << "\tAverage sleep time per frame: " << (double)sleepKPI / (double)nFrames << std::endl;
				sleepKPI = 0;
				nFrames = 0;
				disableFrameCountdown();
				activateConsole();
			}
			else if (getFrameNumber() > 0)
			{
				//std::cout << "Frame count down: " << getFrameNumber() << std::endl;
				countdownFrame(1);
				sleepKPI += m_DelayTime; // Add all delay times for the frames counted down as a KPI for performance.
				nFrames ++;
			}
			else
			{
				std::cout << "Error in frame countdown feature" << std::endl;
			}
		}

		handleGameLogics();
		draw();
		SDL_Flip(getSurface());

		// Sleep if there is time to spare..
		m_DelayTime = (1000/getDesiredFPS()) - (SDL_GetTicks() - frameStartTime);

		if(m_DelayTime > 0)
		{
			Sleep((DWORD)m_DelayTime);
		}
	}
}

void MGFramework::activateConsole()
{
	disableFrameCountdown();
	enableTyping();
	string cLine;
	do{
		cout << "mg> ";
		std::getline(std::cin, cLine);
	}while(runConsoleCommand(cLine.c_str()));
	disableTyping();
}



bool MGFramework::runConsoleCommand(const char *c)
{
	std::string cmd(c);
	std::vector<std::string> cmdvec = split(cmd, ' ');
	if(cmdvec.size() == 0)
	{
		std::cout << "Type help for help" << std::endl;
		return true;
	}
	else if(cmdvec.size() >= 1)
	{
		// Decode all commands implemented in other classes
		if(cmdvec[0]=="map")
		{
			return m_Map.runConsoleCommand(c);
		}
		else if(cmdvec[0]=="window")
		{
			return m_Window.runConsoleCommand(c);
		}
		else if(cmdvec[0]=="mo")
		{
			if(cmdvec.size()>2)
			{
				int moIndex=toInt(cmdvec[1]);
				if(moIndex >= 0 && moIndex < getNumberOfMO())
				{
					return m_MO[toInt(cmdvec[1])].runConsoleCommand(c);
				}
			}
			std::cout << "Error in command (mo)" << std::endl;
		}
	}

	// Decode commands implemented in MGFramework
	if(cmdvec.size() == 1)
	{
		if(cmdvec[0]=="exit")
		{
			std::cout << "Exiting console..." << std::endl;
			return false;
		}
		else if(cmdvec[0]=="help")
		{
			std::cout << "Command help" << std::endl;
			std::cout << "[<object> [<object identifier>]] <command> [<parameter 1> ... <parameter n>]" << std::endl;
			std::cout << "-----------------------------------------------------------------------------" << std::endl << std::endl;
			std::cout << "help -    Displays this command help information." << std::endl;
			std::cout << "exit -    Exists shell and resumes framework execution." << std::endl;
			std::cout << "minimap - Toggles mini map on/off." << std::endl;
			std::cout << "debug -   Toggles debug logging on/off (same as logging)." << std::endl;
			std::cout << "logging - Toggles debug logging on/off (same as debug)." << std::endl;
			std::cout << "fps <f> - Sets desired FPS (frames per second) to integer value <f>." << std::endl;
			std::cout << "runframes <f> - Runs <f> game loops (frames) and presents some recorded data." << std::endl;
			std::cout << "          <f> is an integer." << std::endl;
			std::cout << "create <mo> <n> - Creates <n> objects (and deletes any previously" << std::endl;
			std::cout << "          existing ones). <n> is an integer. Only MGMovingObject supported at"  << std::endl;
			std::cout << "          this point (mo)." << std::endl;

			(void)m_Map.runConsoleCommand("map help");
			(void)m_Window.runConsoleCommand("window help");
			if(getNumberOfMO()>0)(void)m_MO[0].runConsoleCommand("mo 0 help");

			return true;
		}
		else if(cmdvec[0]=="minimap")
		{
			if(miniMapEnabled())
			{
				disableMiniMap();
				std::cout << "Mini map disabled." << std::endl;
				return true;
			}
			else
			{
				enableMiniMap();
				std::cout << "Mini map enabled." << std::endl;
				return true;
			}
		}
		else if(cmdvec[0]=="debug" || cmdvec[0]=="logging")
		{
			if(loggingEnabled())
			{
				disableLogging();
				std::cout << "Debug logging disabled." << std::endl;
				return true;
			}
			else
			{
				enableLogging();
				std::cout << "Debug logging enabled." << std::endl;
				return true;
			}
		}
	}
	else if(cmdvec.size() == 2)
	{
		if(cmdvec[0]=="runframes")
		{
			enableFrameCountdown();
			setFrameNumber(toInt(cmdvec[1]));
			return false;
		}
		else if(cmdvec[0]=="fps")
		{
			setDesiredFPS(toInt(cmdvec[1]));
			return true;
		}
	}
	else if(cmdvec.size() == 3)
	{
		if(cmdvec[0]=="create" && cmdvec[1]=="mo")
		{
			int n = toInt(cmdvec[2]);
			if(n>0)
			{
				createMO(n);
			}
			else
			{
				std::cout << "Error in command (create mo <n>)" << std::endl;
				return true;
			}
			for(int i=0;i<getNumberOfMO();i++)
			{
				m_MO[i].setTileXY(MGFramework::randomN(m_Map.getWidth()), MGFramework::randomN(m_Map.getHeight()));
				m_MO[i].setDestTileXY(MGFramework::randomN(m_Map.getWidth()), MGFramework::randomN(m_Map.getHeight()));
				m_MO[i].setSpeed(0.5, m_Map.getTileHeight()); // Move two tiles per second
			}
			return true;
		}
	}

	else
	{

	}

	std::cout << "Unknown command" << std::endl;
	return true;
}



Uint32 MGFramework::getFPS()
{
	if(m_FrameTime > 0)
	{
		Uint32 result = (Uint32)(1000/m_FrameTime);
		if((result > 0) && (result < (2*getDesiredFPS())))
		{
			return result;
		}
		else
		{
			std::cout << "WARNING: FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << "m_FrameTime:" << m_FrameTime << ", result:" << result << std::endl;
			return getDesiredFPS();
		}
	}
	else
	{
		std::cout << "WARNING: FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << m_FrameTime << std::endl;
		return getDesiredFPS();
	}
}

void MGFramework::setDesiredFPS(Uint32 f)
{
	if(f >= 0)
	{
		m_FPS = f;
	}
	else
	{
		std::cout << "FPS incorrectly calculated (void MGFramework::setDesiredFPS(Uint32 f)): " << f << std::endl;
	}
}

Uint32 MGFramework::getDesiredFPS()
{
	if(m_FPS >= 0)
	{
		return m_FPS;
	}
	else
	{
		std::cout << "FPS incorrectly calculated (Uint32 MGFramework::getDesiredFPS()): " << m_FPS << ", returning 60" << std::endl;
		return 1;
	}
}



bool MGFramework::setWindowProperties(int width, int height, int bpp, bool fullscreen, const string& title)
{
	m_Window.setProperties(width, height, bpp, fullscreen, title);
	m_WindowPropertiesSet = true;
	return true;
}


void MGFramework::createMO(int n)
{
	delete[] m_MO;
	m_NMO=n;
	m_MO = new MGMovingObject[getNumberOfMO()];
}


void MGFramework::drawSprite(SDL_Surface* imageSurface, SDL_Surface* screenSurface, int srcX, int srcY, int dstX, int dstY, int width, int height)
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
	SDL_BlitSurface(imageSurface, &srcRect, screenSurface, &dstRect);
}

void MGFramework::drawSpriteSeeThrough(SDL_Surface* imageSurface, SDL_Surface* screenSurface, int srcX, int srcY, int dstX, int dstY, int width, int height, Uint32 seethrough)
{
	Uint32 p;
	for(int x=0; x<width; x++)
	{
		for(int y=0; y<height; y++)
		{
			if(dstX+x >=0 && dstX+x < m_Window.getWidth() && dstY+y >= 0 && dstY+y < m_Window.getHeight())
			{
				p=getPixel32(imageSurface, srcX+x, srcY+y);
				if(p!=seethrough)
				{
					putPixel32(screenSurface, dstX+x, dstY+y, p);
				}
			}
		}
	}
}

SDL_Surface *MGFramework::loadBMPImage( std::string filename ) 
{
	//Temporary storage for the image that's loaded
	SDL_Surface* loadedImage = NULL;
	//The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;
	//Load the image
	loadedImage = SDL_LoadBMP( filename.c_str() );
	//If nothing went wrong in loading the image
	if( loadedImage != NULL )
	{
		//Create an optimized image
		optimizedImage = SDL_DisplayFormat( loadedImage );
		//Free the old image
		SDL_FreeSurface( loadedImage );
	}
	//Return the optimized image
	return optimizedImage;
}

void MGFramework::drawText(SDL_Surface* screen, const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB)
{
	TTF_Font* font = TTF_OpenFont("ARIAL.TTF", size);
	SDL_Color foregroundColor = { fR, fG, fB };
	SDL_Color backgroundColor = { bR, bG, bB };
	SDL_Surface* textSurface = TTF_RenderText_Shaded(font, string, foregroundColor, backgroundColor);
	SDL_Rect textLocation = { x, y, 0, 0 };
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
}

string MGFramework::toString(int number)
{
	stringstream ss;	//create a stringstream
	ss << number;		//add number to the stream
	return ss.str();	//return a string with the contents of the stream
}

double MGFramework::distance(int x1, int y1, int x2, int y2)
{
	return sqrt((double)(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1))));
}

void MGFramework::putPixel32(SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;
	//Set the pixel
	pixels[ ( y * surface->w ) + x ] = pixel;
}

Uint32 MGFramework::getPixel32(SDL_Surface *surface, int x, int y)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[ ( y * surface->w ) + x ];
}

void MGFramework::drawCircle32(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel)
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
        putPixel32(surface, (int)(cx + x), (int)(cy + y), pixel);
        putPixel32(surface, (int)(cx + y), (int)(cy + x), pixel);
 
        if (x != 0)
        {
            putPixel32(surface, (int)(cx - x), (int)(cy + y), pixel);
            putPixel32(surface, (int)(cx + y), (int)(cy - x), pixel);
        }
 
        if (y != 0)
        {
            putPixel32(surface, (int)(cx + x), (int)(cy - y), pixel);
            putPixel32(surface, (int)(cx - y), (int)(cy + x), pixel);
        }
 
        if (x != 0 && y != 0)
        {
            putPixel32(surface, (int)(cx - x), (int)(cy - y), pixel);
            putPixel32(surface, (int)(cx - y), (int)(cy - x), pixel);
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


void MGFramework::drawFillCircle32(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel)
{
    static const int BPP = 4;
    double r = (double)radius;
    for (double dy = 1; dy <= r; dy += 1.0)
    {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        int x = cx - (int)dx;
 
        // Grab a pointer to the left-most pixel for each half of the circle
        Uint8 *target_pixel_a = (Uint8 *)surface->pixels + ((int)(cy + r - dy)) * surface->pitch + x * BPP;
        Uint8 *target_pixel_b = (Uint8 *)surface->pixels + ((int)(cy - r + dy)) * surface->pitch + x * BPP;
 
        for (; x <= cx + dx; x++)
        {
            *(Uint32 *)target_pixel_a = pixel;
            *(Uint32 *)target_pixel_b = pixel;
            target_pixel_a += BPP;
            target_pixel_b += BPP;
        }
    }
}


std::vector<std::string> MGFramework::split(std::string str, char c)
{
	std::string strTempString;
	std::vector<int> splitIndices;
	std::vector<std::string> splitLine;
    int nCharIndex = 0;
    int nLineSize = (int)str.size();

    // find indices
    for(int i = 0; i < nLineSize; i++)
    {
        if(str[i] == c)
            splitIndices.push_back(i);
    }
    splitIndices.push_back(nLineSize); // end index

    // fill split lines
    for(int i = 0; i < (int)splitIndices.size(); i++)
    {
        strTempString = str.substr(nCharIndex, (splitIndices[i] - nCharIndex));
        splitLine.push_back(strTempString);
        nCharIndex = splitIndices[i] + 1;
    }
	return splitLine;
}

int MGFramework::toInt(const string &s)
{
	std::istringstream buffer(s);
	int value;
	buffer >> value;
	return value;
}