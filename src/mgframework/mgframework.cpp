#include "mgframework.h"
#include <iostream>
#include <cmath>

using namespace std;


MGFramework::MGFramework()
{
	m_CommandLine[0]='\0';
	setDesiredFPS(60);
	m_FrameTime = (Uint32)(1000/getDesiredFPS()); // Initial FPS value of 60...
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
						enableTyping();
						do{
							cout << "mg> ";
							cin >> m_CommandLine;
							// Execute command here..
							if(strcmp(m_CommandLine, "quit")==0)
							{
								std::cout << "Terminating program..." << std::endl;
								return false;
							}
							else if(strcmp(m_CommandLine, "exit")==0)
							{
								std::cout << "Exiting console..." << std::endl;
							}
							else if(strcmp(m_CommandLine, "help")==0)
							{
								std::cout << "Command help" << std::endl;
								std::cout << "help - Displays this command help information." << std::endl;
								std::cout << "quit - Quits program." << std::endl;
								std::cout << "exit - Exists shell and resumes framework execution." << std::endl;
								std::cout << "minimap - Toggles mini map on/off." << std::endl;
								std::cout << "debug - Toggles debug logging on/off (same as logging)." << std::endl;
								std::cout << "logging - Toggles debug logging on/off (same as debug)." << std::endl;
								std::cout << "fps30 / fps60 / fps90 / fps200 - Sets desired FPS (frames per second)" << std::endl;
							}
							else if(strcmp(m_CommandLine, "minimap")==0)
							{
								if(miniMapEnabled())
								{
									disableMiniMap();
									std::cout << "Mini map disabled." << std::endl;
								}
								else
								{
									enableMiniMap();
									std::cout << "Mini map enabled." << std::endl;
								}
							}
							else if(strcmp(m_CommandLine, "debug")==0 || strcmp(m_CommandLine, "logging")==0)
							{
								if(loggingEnabled())
								{
									disableLogging();
									std::cout << "Debug logging disabled." << std::endl;
								}
								else
								{
									enableLogging();
									std::cout << "Debug logging enabled." << std::endl;
								}
							}
							else if(strcmp(m_CommandLine, "fps30")==0)
							{
								setDesiredFPS(30);
							}
							else if(strcmp(m_CommandLine, "fps60")==0)
							{
								setDesiredFPS(60);
							}
							else if(strcmp(m_CommandLine, "fps90")==0)
							{
								setDesiredFPS(90);
							}
							else if(strcmp(m_CommandLine, "fps200")==0)
							{
								setDesiredFPS(200);
							}
							else
							{
								std::cout << "Unknown command (" << m_CommandLine << ")." << std::endl;
							}

						}while(strcmp(m_CommandLine, "exit")!=0);

						disableTyping();
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
				int iClick = map.getTileIndex(event.button.x, event.button.y);
				if (iClick >= 0)
				{
					int xClick = map.getTileX(iClick);
					int yClick = map.getTileY(iClick);
					if (((int) event.button.button) == 1)
					{
						map.setTileProperty(xClick, yClick, 1);
					}
					else if (((int) event.button.button) == 3)
					{
						map.mouseScrollingClick(event.button.x, event.button.y);
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
					map.mouseScrollingRelease(event.button.x, event.button.y);
				}
				break;
			}

			case SDL_MOUSEMOTION:
			{
				/*
				if(loggingEnabled())
				{
					std::cout << "SDL_MOUSEMOTION" << std::endl;
				}
				*/
				map.mouseScrollingUpdate(event.motion.x, event.motion.y);
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

void MGFramework::enableLogging()
{
	m_LoggingEnabled = true;
}

void MGFramework::disableLogging()
{
	m_LoggingEnabled = false;
}

bool MGFramework::loggingEnabled()
{
	return m_LoggingEnabled;
}

void MGFramework::disableTyping()
{
	m_TypingEnabled = false;
}

void MGFramework::enableTyping()
{
	m_TypingEnabled = true;
}

bool MGFramework::typingEnabled()
{
	return m_TypingEnabled;
}

void MGFramework::setProgramName(const char *name)
{
	// Not implemented yet
}

const char *MGFramework::getProgramName()
{
	return "Project2 default demo application name";
}

void MGFramework::setProgramVersion(const char *version)
{
	m_ProgramVersion = string(version);
}

const char *MGFramework::getProgramVersion()
{
	return m_ProgramVersion.c_str();
}

void MGFramework::enableMiniMap()
{
	m_MiniMapEnabled = true;
}

bool MGFramework::miniMapEnabled()
{
	return m_MiniMapEnabled;
}

void MGFramework::disableMiniMap()
{
	m_MiniMapEnabled = false;
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
			std::cout << "FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << "m_FrameTime:" << m_FrameTime << ", result:" << result << std::endl;
			return getDesiredFPS();
		}
	}
	else
	{
		std::cout << "FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << m_FrameTime << std::endl;
		return getDesiredFPS();
	}
}

void MGFramework::setDesiredFPS(Uint32 f)
{
	if(f > 0)
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
	if(m_FPS > 0)
	{
		return m_FPS;
	}
	else
	{
		std::cout << "FPS incorrectly calculated (Uint32 MGFramework::getDesiredFPS()): " << m_FPS << ", returning 60" << std::endl;
		return 60;
	}
}

Sint32  MGFramework::getLastFrameDelayTime()
{
	return m_DelayTime;
}

bool MGFramework::setWindowProperties(int width, int height, int bpp, bool fullscreen, const string& title)
{
	m_Window.setProperties(width, height, bpp, fullscreen, title);
	m_WindowPropertiesSet = true;
	return true;
}

void MGFramework::unsetWindowProperties()
{
	m_WindowPropertiesSet = false;
}

bool MGFramework::windowPropertiesSet()
{
	return m_WindowPropertiesSet;
}

SDL_Surface * MGFramework::getSurface()
{
	return m_Window.screen;
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
	stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

double MGFramework::distance(int x1, int y1, int x2, int y2)
{
	return sqrt((double)(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1))));
}

void MGFramework::putPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;
	//Set the pixel
	pixels[ ( y * surface->w ) + x ] = pixel;
}


