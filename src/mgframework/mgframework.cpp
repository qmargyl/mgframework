#include "mgframework.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <winsock.h>

using namespace std;


MGFramework::MGFramework()
{
	m_NMO=0;
	m_NPE=0;
	setDesiredFPS(20);
	m_FrameTime=16;
	m_FrameCountdownEnabled = false;
	m_FrameNumber = 0;
	m_KeepSocketTerminalOpen = false;
	std::srand((unsigned)std::time(0));
	m_FramingOngoing = false;
	m_XFrameStart = 0;
	m_YFrameStart = 0;
	m_XFrameEnd = 0;
	m_YFrameEnd = 0;
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
	// No events for a server instance..
	if(getInstanceType() == MGFSERVERINSTANCE) return true;

	// Event processing loop is started after all special cases.
	SDL_Event event;
	while (SDL_PollEvent(&event))//get all events
	{
		switch (event.type)
		{
			// Quit event
			case SDL_QUIT:
			{
				MGFLOG(std::cout << "SDL_QUIT" << std::endl;)
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
					MGFLOG(std::cout << "SDL_KEYUP" << std::endl << "  " << SDL_GetKeyName(sym) << std::endl;)
					m_Keys[sym] = 0;
				}
				break;
			}

			case SDL_VIDEORESIZE:
			{
				MGFLOG(std::cout << "SDL_VIDEORESIZE" << std::endl << "  " << "Resizing Window to " << event.resize.w << "x" << event.resize.h << std::endl;)
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
					MGFLOG(std::cout << "SDL_MOUSEBUTTONDOWN" << std::endl << "  " << "Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")" << std::endl;)
					if (((int) event.button.button) == 1)
					{
						activateFraming(event.button.x, event.button.y);
						MGFLOG(std::cout << "Map (left click): index = " << iClick << ", x = " << xClick << ", y = " << yClick << std::endl;)
					}
					else if (((int) event.button.button) == 3)
					{
						MGFLOG(std::cout << "Map (right click): index = " << iClick << ", x = " << xClick << ", y = " << yClick << std::endl;)
					}
				}
				else
				{
					MGFLOG(std::cout << "SDL_MOUSEBUTTONDOWN" << std::endl << "  " << "Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")" << std::endl;)
					if (((int) event.button.button) == 1)
					{
						MGFLOG(std::cout << "Map (left click): index = -1, x = <not calculated>, y = <not calculated>" << std::endl;)
					}
					else if (((int) event.button.button) == 3)
					{
						MGFLOG(std::cout << "Map (right click): index = -1, x = <not calculated>, y = <not calculated>" << std::endl;)
					}
				}
				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				MGFLOG(std::cout << "SDL_MOUSEBUTTONUP" << std::endl << "  " << "Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")" << std::endl;)
				if (((int) event.button.button) == 1)
				{
					int endClickX = m_Map.getTileX(m_Map.getTileIndex(getFrameEndX(), getFrameEndY()));
					int endClickY = m_Map.getTileY(m_Map.getTileIndex(getFrameEndX(), getFrameEndY()));
					int startClickX = m_Map.getTileX(m_Map.getTileIndex(getFrameStartX(), getFrameStartY()));
					int startClickY = m_Map.getTileY(m_Map.getTileIndex(getFrameStartX(), getFrameStartY()));
					for(int x=smallest(startClickX, endClickX); x<=biggest(startClickX, endClickX); x++)
					{
						for(int y=smallest(startClickY, endClickY); y<=biggest(startClickY, endClickY); y++)
						{
							for(int i=0; i<getNumberOfMO(); i++)
							{
								if(m_MO[i].getTileX()==x && m_MO[i].getTileY()==y)
								{
									m_MO[i].mark();
								}
							}
						}
					}
					deactivateFraming();
				}
				else if (((int) event.button.button) == 3)
				{
					m_Map.mouseScrollingRelease(event.button.x, event.button.y);
					for(int i=0; i<getNumberOfMO(); i++)
					{
						m_MO[i].unMark();
					}
				}
				break;
			}

			case SDL_MOUSEMOTION:
			{
				m_Map.mouseScrollingUpdate(event.motion.x, event.motion.y);
				if(isFramingOngoing()) updateFraming(event.motion.x, event.motion.y);
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
		if(getInstanceType() != MGFSERVERINSTANCE) SDL_Flip(getSurface());

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
				if(cmdvec[1]=="marked")
				{
					for(int i=0; i<getNumberOfMO(); i++)
					{
						if(m_MO[i].isMarked())
						{
							m_MO[i].runConsoleCommand(c);
						}
					}
					return true;
				}
				else
				{
					int moIndex=toInt(cmdvec[1]);
					if(moIndex >= 0 && moIndex < getNumberOfMO())
					{
						return m_MO[toInt(cmdvec[1])].runConsoleCommand(c);
					}
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
			std::cout << "logging - Toggles logging on/off." << std::endl;
			std::cout << "setfps <f> - Sets desired FPS (frames per second) to integer value <f>." << std::endl;
			std::cout << "runframes <f> - Runs <f> game loops (frames) and presents some recorded data." << std::endl;
			std::cout << "          <f> is an integer." << std::endl;
			std::cout << "create <mo|pe> <n> - Creates <n> objects (and deletes any previously" << std::endl;
			std::cout << "          existing ones). <n> is an integer. Only MGMovingObject (mo)"  << std::endl;
			std::cout << "          and MGPeriodicEvent are (pe) supported at this point." << std::endl;
			std::cout << "add <mo|pe> <n> - Adds <n> objects. <n> is an integer. Only MGMovingObject" << std::endl;
			std::cout << "          (mo) and MGPeriodicEvent (pe) are supported at this point."  << std::endl;
			std::cout << "open <terminalserver> - Opens the terminal server for framework commands " << std::endl;
			std::cout << "          over TCP/IP." << std::endl;
			std::cout << "close <terminalserver> - Closes the terminal server for framework commands " << std::endl;
			std::cout << "          over TCP/IP." << std::endl;

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
		else if(cmdvec[0]=="logging")
		{
			if(loggingEnabled())
			{
				disableLogging();
				std::cout << "Logging disabled." << std::endl;
				return true;
			}
			else
			{
				enableLogging();
				std::cout << "Logging enabled." << std::endl;
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
		else if(cmdvec[0]=="setfps")
		{
			setDesiredFPS(toInt(cmdvec[1]));
			return true;
		}
		else if(cmdvec[0]=="open" && cmdvec[1]=="terminalserver")
		{
				std::cout << "Opening terminal server..." << std::endl;
				openSocketTerminal();
				m_SocketTerminal = SDL_CreateThread(runMGFrameworkSocketTerminal, this);
				return true;
		}
		else if(cmdvec[0]=="close" && cmdvec[1]=="terminalserver")
		{
				std::cout << "Closing terminal server..." << std::endl;
				if(socketTerminalOpen())
				{
					closeSocketTerminal();
				}
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
				// Clear the map of occupied marks...
				for(int y=0; y<m_Map.getHeight(); y++)
				{
					for(int x=0; x<m_Map.getWidth(); x++)
					{
						m_Map.unOccupy(x, y);
					}
				}
				// ...and then create the new MOs.
				createMO(n);
			}
			else
			{
				std::cout << "Error in command (create mo <n>)" << std::endl;
				return true;
			}
			for(int i=0;i<getNumberOfMO();i++)
			{
				m_MO[i].setTileXY(MGFramework::randomN(m_Map.getWidth()), MGFramework::randomN(m_Map.getHeight()), this);
				//m_MO[i].setDestTileXY(MGFramework::randomN(m_Map.getWidth()), MGFramework::randomN(m_Map.getHeight()));
				m_MO[i].setSpeed(0.5, m_Map.getTileHeight()); // Move two tiles per second
			}
			return true;
		}
		else if(cmdvec[0]=="add" && cmdvec[1]=="mo")
		{
			int nBefore=getNumberOfMO();
			int n = toInt(cmdvec[2]);
			if(n>0)
			{
				addMO(n);
			}
			else
			{
				std::cout << "Error in command (add mo <n>)" << std::endl;
				return true;
			}
			for(int i=nBefore; i<getNumberOfMO(); i++)
			{
				m_MO[i].setTileXY(MGFramework::randomN(m_Map.getWidth()), MGFramework::randomN(m_Map.getHeight()), this);
				//m_MO[i].setDestTileXY(MGFramework::randomN(m_Map.getWidth()), MGFramework::randomN(m_Map.getHeight()));
				m_MO[i].setSpeed(0.5, m_Map.getTileHeight()); // Move two tiles per second
			}
			return true;
		}
		if(cmdvec[0]=="create" && cmdvec[1]=="pe")
		{
			int n = toInt(cmdvec[2]);
			if(n>0)
			{
				createPE(n);
			}
			else
			{
				std::cout << "Error in command (create pe <n>)" << std::endl;
			}
			return true;
		}
		else if(cmdvec[0]=="add" && cmdvec[1]=="pe")
		{
			int nBefore=getNumberOfPE();
			int n = toInt(cmdvec[2]);
			if(n>0)
			{
				addPE(n);
			}
			else
			{
				std::cout << "Error in command (add pe <n>)" << std::endl;
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
			MGFLOG(std::cout << "WARNING: FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << "m_FrameTime:" << m_FrameTime << ", result:" << result << std::endl;)
			return getDesiredFPS();
		}
	}
	else
	{
		MGFLOG(std::cout << "WARNING: FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << m_FrameTime << std::endl;)
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
		MGFLOG(std::cout << "FPS incorrectly calculated (void MGFramework::setDesiredFPS(Uint32 f)): " << f << std::endl;)
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
		MGFLOG(std::cout << "FPS incorrectly calculated (Uint32 MGFramework::getDesiredFPS()): " << m_FPS << ", returning 60" << std::endl;)
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

void MGFramework::addMO(int n)
{
	MGMovingObject *oldMO = new MGMovingObject[getNumberOfMO()];
	int nOld=getNumberOfMO();
	for(int i=0; i<nOld; i++)
	{
		oldMO[i].copy(&m_MO[i]);
	}
	delete[] m_MO;
	m_NMO=nOld+n;
	m_MO = new MGMovingObject[getNumberOfMO()];
	for(int i=0; i<nOld; i++)
	{
		m_MO[i].copy(&oldMO[i]);
	}
}


void MGFramework::createPE(int n)
{
	delete[] m_PE;
	m_NPE=n;
	m_PE = new MGPeriodicEvent[getNumberOfPE()];
}

void MGFramework::addPE(int n)
{
	MGPeriodicEvent *oldPE = new MGPeriodicEvent[getNumberOfPE()];
	int nOld=getNumberOfPE();
	for(int i=0; i<nOld; i++)
	{
		oldPE[i].copy(&m_PE[i]);
	}
	delete[] m_PE;
	m_NPE=nOld+n;
	m_PE = new MGPeriodicEvent[getNumberOfPE()];
	for(int i=0; i<nOld; i++)
	{
		m_PE[i].copy(&oldPE[i]);
	}
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


void MGFramework::vLine32(SDL_Surface *surface, int x, int y, int length, Uint32 pixel)
{
	for(int i=y; i<y+length; i++)
	{
		putPixel32(surface, x, i, pixel);
	}
}

void MGFramework::hLine32(SDL_Surface *surface, int x, int y, int length, Uint32 pixel)
{
	for(int i=x; i<x+length; i++)
	{
		putPixel32(surface, i, y, pixel);
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

bool MGFramework::detectCollisionRectangle(int x1, int y1, int x2, int y2, int a1, int b1, int a2, int b2)
{
	if(detectCollisionPointRectangle(x1, y1, a1, b1, a2, b2))
		return true;
	if(detectCollisionPointRectangle(x2, y2, a1, b1, a2, b2))
		return true;
	if(detectCollisionPointRectangle(a1, b1, x1, y1, x2, y2))
		return true;
	if(detectCollisionPointRectangle(a2, b2, x1, y1, x2, y2))
		return true;
	return false;
}

bool MGFramework::detectCollisionPointRectangle(int px, int py, int x1, int y1, int x2, int y2)
{ 
	if(x1 >= px && px >=x2 && y1 >= py && py >= y2)
		return true;
	if(x2 >= px && px >=x1 && y2 >= py && py >= y1)
		return true;
	return false;
}

int MGFramework::initializeWinsock(WORD wVersionRequested)
{
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);

	if (err!=0) return 0; // Tell the user that we couldn't find a usable winsock.dll 
	if (LOBYTE(wsaData.wVersion )!=1 || HIBYTE(wsaData.wVersion)!=1) return 0;
	//Everything is fine: proceed
	return 1;
}

bool MGFramework::okMGFrameworkSyntax(const char *c)
{
	if(strlen(c)>2)
	{
		return true;
	}
	return false;
}

int runMGFrameworkSocketTerminal(void *fm)
{
	MGFramework *mgf = (MGFramework *)fm;
	mgf->logIfEnabled("Opening socket terminal...");
	#define PORTNR 666
	bool connectionOpen=true;
	int nZerosInARow=0;

	while(mgf->socketTerminalOpen())
	{
		if (!mgf->initializeWinsock (MAKEWORD(1,1) ) ) 
		{
			mgf->logIfEnabled("Error initializing Winsock.");
			return 1;
		}

		SOCKET fd, fd_new; 	// "file" descriptors for the network sockets
		SOCKADDR_IN saddr_me;

		if ((fd=socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
		{
			mgf->logIfEnabled("Server: socket not connected ");
			return 1;
		}

		saddr_me.sin_family = AF_INET;
		saddr_me.sin_addr.s_addr= htonl(INADDR_ANY);
		saddr_me.sin_port = htons(PORTNR);

		if (bind(fd, (LPSOCKADDR) &saddr_me, sizeof(saddr_me)) == SOCKET_ERROR)
		{
			mgf->logIfEnabled("Server: bind failure ");
			return 1;
		}

		if (listen(fd,1) == SOCKET_ERROR)
		{
			mgf->logIfEnabled("Server: listen failure ");
			return 1;
		}

		// the server is now started and ready to accept a command..
		mgf->logIfEnabled("Waiting for terminal command...");

		if ( (fd_new=accept(fd, NULL, NULL)) == INVALID_SOCKET)
		{
			mgf->logIfEnabled("Server: accept failure ");
			return 1;
		}

		while(connectionOpen)
		{
			char buf[256];
			for (int i=0; i < 256; i++)
			{
				buf[i]=0;
			}
			if( recv(fd_new, buf, sizeof(buf), 0) == SOCKET_ERROR )
			{
				mgf->logIfEnabled("Server: receiving failure ");
				return 1;
			}

			// Now buf contains the request string.

			int lBuf=(int)strlen(buf);
			if(lBuf==0)
			{
				nZerosInARow++;
			}
			else
			{
				nZerosInARow=0;
				if(mgf->okMGFrameworkSyntax(buf))
				{
					mgf->runConsoleCommand(buf);
					if(send(fd_new, "ok\n\r", 4, 0) == SOCKET_ERROR)
					{
						mgf->logIfEnabled("Server: Failed sending an answer to client request");
						return 1;
					}
				}
			}
			if(nZerosInARow>4)
			{
				connectionOpen=false;
			}
			//Sleep((DWORD)100);
		}
		WSACleanup();
	}
	mgf->logIfEnabled("Socket terminal closed...");
	return 0;
}