#include "mgframework.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <winsock.h>

using namespace std;


MGFramework::MGFramework():
	m_NMO(0),
	m_NPE(0),
	m_FrameTime(16),
	m_FrameCountdownEnabled(false),
	m_FrameNumber(0),
	m_KeepSocketTerminalOpen(false),
	m_FramingOngoing(false),
	m_XFrameStart(0),
	m_YFrameStart(0),
	m_XFrameEnd(0),
	m_YFrameEnd(0),
	m_MarkedMOs(0),
	m_Quit(false),
	m_DelayTime(0),
	m_InputEnabled(true)
{
	setDesiredFPS(20);
	std::srand((int)std::time(0));
}

MGFramework::~MGFramework()
{
//	delete[] m_MO;
//	delete[] m_PE;
}

void MGFramework::resize(int x, int y)
{
	// Not implemented yet
}

bool MGFramework::processEvents()
{
	// No events for a server instance..
	if(getInstanceType() == MGFSERVERINSTANCE) return true;

	// Don't handle input if input is disabled..
	if(!isInputEnabled()) return true;

	// Event processing loop is started after all special cases.
	SDL_Event event;
	while (SDL_PollEvent(&event))//get all events
	{
		switch (event.type)
		{
			// Quit event
			case SDL_QUIT:
			{
				MGFLOG_INFO(std::cout << "SDL_QUIT" << std::endl;)
				// Return false because we are quitting.
				//quit();
				//break;
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
					MGFLOG_INFO(std::cout << "SDL_KEYUP" << std::endl << "  " << SDL_GetKeyName(sym) << std::endl;)
					m_Keys[sym] = 0;
				}
				break;
			}

			case SDL_VIDEORESIZE:
			{
				MGFLOG_INFO(std::cout << "SDL_VIDEORESIZE" << std::endl << "  " << "Resizing Window to " << event.resize.w << "x" << event.resize.h << std::endl;)
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

						// Unmark all MO..
						for(int i=0; i<getNumberOfMO(); i++)
						{
							if(m_MO != NULL && m_MO[i].isMarked())
							{
								m_MO[i].unMark();
								countUnMark();
							}
							if(m_MO == NULL)
							{
								MGFLOG_WARNING(std::cout << "m_MO = NULL and getNumberOfMO() = " << getNumberOfMO() << std::endl;)
							}
						}

						activateFraming(event.button.x, event.button.y);
						MGFLOG_INFO(std::cout << "Map (left click): index = " << iClick << ", x = " << xClick << ", y = " << yClick << std::endl;)
					}
					else if (((int) event.button.button) == 3)
					{
						MGFLOG_INFO(std::cout << "Map (right click): index = " << iClick << ", x = " << xClick << ", y = " << yClick << std::endl;)
						m_Map.mouseScrollingClick(event.button.x, event.button.y);
					}

					// Log debug information to console.
					MGFLOG_INFO(std::cout << "SDL_MOUSEBUTTONDOWN: Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")" << std::endl;)

				}
				else
				{
					MGFLOG_INFO(std::cout << "SDL_MOUSEBUTTONDOWN" << std::endl << "  " << "Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")" << std::endl;)
					if (((int) event.button.button) == 1)
					{
						MGFLOG_INFO(std::cout << "Map (left click): index = -1, x = <not calculated>, y = <not calculated>" << std::endl;)
					}
					else if (((int) event.button.button) == 3)
					{
						MGFLOG_INFO(std::cout << "Map (right click): index = -1, x = <not calculated>, y = <not calculated>" << std::endl;)
					}
				}
				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				MGFLOG_INFO(std::cout << "SDL_MOUSEBUTTONUP: Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")" << std::endl;)
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
								if(m_MO != NULL && m_MO[i].getTileX() == x && m_MO[i].getTileY() == y)
								{
									if(!m_MO[i].isMarked())
									{
										m_MO[i].mark();
										countMark();
									}
								}
								if(m_MO == NULL)
								{
									MGFLOG_WARNING(std::cout << "m_MO = NULL and getNumberOfMO() = " << getNumberOfMO() << std::endl;)
								}
							}
						}
					}
					deactivateFraming();
				}
				else if (((int) event.button.button) == 3)
				{
					// Order marked MOs to go to mouse location
					if(getNumberOfMarkedMO()>0)
					{
						int iClick = m_Map.getTileIndex(event.button.x, event.button.y);
						int xClick = m_Map.getTileX(iClick);
						int yClick = m_Map.getTileY(iClick);
						char c[64];
						sprintf(c, "mo marked setdestination %d %d", xClick, yClick);
						runConsoleCommand(c, this);
					}

					m_Map.mouseScrollingRelease(event.button.x, event.button.y);
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

	// Quit if it has been decided to do so..
	if(getQuitFlag()) return false;

	return true;
}

void MGFramework::parse(const char *scriptFileName)
{
	FILE *sf = NULL;
	errno_t scriptError = fopen_s(&sf, scriptFileName, "rt");
	if(sf == NULL)
	{
		MGFLOG_ERROR(std::cout << "MGFramework::parse failed to open script file " << scriptFileName << ", error(" << scriptError << ")" << std::endl;);
	}
	else
	{
		char scriptLine[MGF_SCRIPTLINE_MAXLENGTH] = "";
		char *neof = NULL;
		MGFLOG_INFO(std::cout << "MGFramework::parse starting to parse script file " << scriptFileName << std::endl;);

		while(true)
		{
			// Read until new line or end of file, whichever happens first..
			neof = fgets(scriptLine, MGF_SCRIPTLINE_MAXLENGTH, sf);
			if(neof == NULL)
			{
				break;
			}
			else
			{
				// Remove the newline and any tailing "special" characters before sending command to runConsoleCommand
				for(int i=(int)strlen(scriptLine); i>=0 ; --i)
				{
					if(scriptLine[i] <= 32) // Remove characters from 0 to space in the ASCII table
					{
						scriptLine[i] = '\0';
					}
					else
					{
						break;
					}
				}
				// Also remove comments..
				int l=(int)strlen(scriptLine);
				for(int i=0; i<l; ++i)
				{
					if(scriptLine[i] == '/') // Remove comments
					{
						scriptLine[i] = '\0';
					}
				}

				MGFLOG_INFO(std::cout << ">" << scriptLine << std::endl;);

				if(okMGFrameworkSyntax(scriptLine))
				{
					MGFLOG_INFO(std::cout << "MGFramework::parse calls runConsoleCommand(" << scriptLine << ")" << std::endl;);
					runConsoleCommand(scriptLine, this);
				}
			}
		}

		if(sf != NULL)
		{
			fclose(sf);
		}

		MGFLOG_INFO(std::cout << "MGFramework::parse finished parsing script file " << scriptFileName << std::endl;);
	}
}

void MGFramework::logEval(const char *logFileName)
{
	FILE *lf = NULL;
	errno_t logError = fopen_s(&lf, logFileName, "rt");
	if(lf == NULL)
	{
		MGFLOG_ERROR(std::cout << "MGFramework::logEval failed to open log file " << logFileName << ", error(" << logError << ")" << std::endl;);
	}
	else
	{
		std::cout << "Evaluating file: " << logFileName << " ... ";

		char logLine[MGF_LOGLINE_MAXLENGTH] = "";
		char *neof = NULL;
		//MGFLOG_INFO(std::cout << "MGFramework::logEval starting to parse log file " << logFileName << std::endl;);

		int nErrors=0;
		while(true)
		{
			// Read until new line or end of file, whichever happens first..
			neof = fgets(logLine, MGF_LOGLINE_MAXLENGTH, lf);
			if(neof == NULL)
			{
				break;
			}
			else
			{
				// An error defined as a log line containing at least one "ERROR".
				std::string line(logLine);
				std::string errSubstr("ERROR");
				std::size_t found = line.find(errSubstr);
				if (found!=std::string::npos) nErrors++;
			}
		}

		if(nErrors!=0)
		{
			std::cout << "FAIL (" << nErrors << " errors)" << std::endl;
		}
		else
		{
			std::cout << "PASS" << std::endl;
		}

		if(lf != NULL)
		{
			fclose(lf);
		}

		//MGFLOG_INFO(std::cout << "MGFramework::logEval finished parsing log file " << logFileName << std::endl;);
	}
}


void MGFramework::run(const char *scriptFileName)
{
	parse(scriptFileName);

	Uint32 frameStartTime = 0; 
	m_DelayTime = 0;
	Uint32 lastFrameTime = SDL_GetTicks()-1; // SDL_GetTicks() - lastFrameTime cannot be zero.

	// Frame loop (processEvents, handleGameLogics, draw)
	while(processEvents())
	{
		// Calculate the current frame time (and implicitly FPS)..
		m_FrameTime = SDL_GetTicks() - lastFrameTime; //number of milli seconds from last frame was handled
		lastFrameTime = SDL_GetTicks();
		frameStartTime = SDL_GetTicks();

		// Handle all calculations and draw current frame..


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
				activateConsole();
			}
			else if (getFrameNumber() > 0)
			{
				countdownFrame(1);
				sleepKPI += m_DelayTime; // Add all delay times for the frames counted down as a KPI for performance.
				nFrames ++;
			}
			else
			{
				MGFLOG_ERROR(std::cout << "Error in frame countdown feature" << std::endl;);
			}
		}

		handleGameLogics();
		draw();
		if(getInstanceType() != MGFSERVERINSTANCE) SDL_Flip(getSurface()); // A server instance of the framework has no graphics.

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
		std::cout << "mg> ";
		std::getline(std::cin, cLine);
	}while(runConsoleCommand(cLine.c_str(), this));
	disableTyping();
}



bool MGFramework::runConsoleCommand(const char *c, MGFramework *w)
{
	std::string cmd(c);
	std::vector<std::string> cmdvec = split(cmd, ' ');

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
			MGFLOG_ERROR(std::cout << "MGFramework::runConsoleCommand received MGComponent_UNDEFINED from MGFramework::detectMGComponentConsoleCommand" << std::endl;); 
			break;

		case MGComponent_MAP_X:
		{
			return m_Map.runConsoleCommand(c, this);
		}

		case MGComponent_WINDOW_X:
		{
			return m_Window.runConsoleCommand(c, this);
		}

		case MGComponent_PE_INT_X:
		case MGComponent_PE_INT_HELP:
		case MGComponent_PE_INT_ACTIVATE:
		case MGComponent_PE_INT_DEACTIVATE:
		case MGComponent_PE_INT_SETUPTIMER_INT:
		case MGComponent_PE_INT_LOGGING_ON:
		case MGComponent_PE_INT_LOGGING_OFF:
		case MGComponent_PE_INT_STOREFILENAME_FILENAME:
		{
			int peIndex=toInt(cmdvec[1]);
			if(peIndex >= 0 && peIndex < getNumberOfPE())
			{
				return m_PE[toInt(cmdvec[1])].runConsoleCommand(c, this);
			}
			MGFLOG_WARNING(std::cout << "Console command was not forwarded to PE " << peIndex << std::endl;); 
			return true;
		}

		case MGComponent_PE_ALL_X:
		{
			for(int i=0; i<getNumberOfPE(); i++)
			{
				m_PE[i].runConsoleCommand(c, this);
			}
			return true;
		}

		case MGComponent_SETFPS_INT:
		{
			setDesiredFPS(toInt(cmdvec[1]));
			return true;
		}


		case MGComponent_CREATE_MO_INT_PARAMLIST:
		{
			int n = toInt(cmdvec[2]);
			int owner = 0;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1]);
					++i;
				}
				else
				{
					MGFLOG_ERROR(std::cout << "Error in command (create mo <n>), bad parameter list" << std::endl;);
					n = 0; // Abort MO creation..
				}
			}
			if(n > 0)
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
				MGFLOG_ERROR(std::cout << "Error in command (create mo <n>)" << std::endl;);
				return true;
			}
			for(int i=0;i<getNumberOfMO();i++)
			{
				// Here there should be a check whether the tile is already occupied..
				if(m_MO != NULL)
				{
					m_MO[i].setTileXY(MGFramework::randomN(m_Map.getWidth()), MGFramework::randomN(m_Map.getHeight()), this);
					m_MO[i].setDestTileXY(m_MO[i].getTileX(), m_MO[i].getTileY());
					m_MO[i].setSpeed(0.5, m_Map.getTileHeight()); // Move two tiles per second
					m_MO[i].setOwner(owner);
					m_Map.occupy(m_MO[i].getTileX(), m_MO[i].getTileY(), m_MO[i].getID());
				}
				if(m_MO == NULL)
				{
					MGFLOG_WARNING(std::cout << "m_MO = NULL and getNumberOfMO() = " << getNumberOfMO() << std::endl;)
				}
			}
			return true;
		}



		case MGComponent_DELETE_ALL_MO_PARAMLIST:
		{
			// Clear the map of occupied marks...
			int owner = 0;
			bool ownerParamSet=false;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1]);
					ownerParamSet=true;
					++i;
				}
				else
				{
					MGFLOG_ERROR(std::cout << "Error in command (delete all mo), bad parameter list" << std::endl;);
					return true;
				}
			}

			if(ownerParamSet)
			{
				// Delete only MOs connected to a specific owner
				MGFLOG_INFO(std::cout << "Deleting MOs owned by " << owner << std::endl;);
				for(int i = getNumberOfMO()-1; i>=0; --i)
				{
					if(m_MO[i].getOwner() == owner)
					{
						deleteMO(i);
					}
				}
			}
			else
			{
				//Deleting ALL MOs is faster than deleting them per owner 
				for(int y=0; y<m_Map.getHeight(); y++)
				{
					for(int x=0; x<m_Map.getWidth(); x++)
					{
						m_Map.unOccupy(x, y);
					}
				}
				// ...and then create zero new MOs.
				MGFLOG_INFO(std::cout << "Creating zero MO..." << std::endl;);
				createMO(0);
			}
			return true;
		}



		case MGComponent_DELETE_ALL_PE_PARAMLIST:
		{
			// Clear the map of occupied marks...
			int owner = 0;
			bool ownerParamSet=false;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1]);
					ownerParamSet=true;
					++i;
				}
				else
				{
					MGFLOG_ERROR(std::cout << "Error in command (delete all pe), bad parameter list" << std::endl;);
					return true;
				}
			}

			if(ownerParamSet)
			{
				// Delete only PEs connected to a specific owner
				MGFLOG_INFO(std::cout << "Deleting PEs owned by " << owner << std::endl;);
				for(int i = getNumberOfPE()-1; i>=0; --i)
				{
					if(m_PE[i].getOwner() == owner)
					{
						deletePE(i);
					}
				}
			}
			else
			{
				// Create zero new PEs.
				MGFLOG_INFO(std::cout << "Creating zero PE..." << std::endl;);
				createPE(0);
			}
			return true;
		}



		case MGComponent_ADD_MO_INT_PARAMLIST:
		{
			int nBefore=getNumberOfMO();
			int n = toInt(cmdvec[2]);
			int owner = 0;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1]);
					++i;
				}
				else
				{
					MGFLOG_ERROR(std::cout << "Error in command (add mo <n>), bad parameter list" << std::endl;);
					n = 0; // Abort MO creation..
				}
			}
			if(n>0)
			{
				addMO(n);
			}
			else
			{
				MGFLOG_ERROR(std::cout << "Error in command (add mo <n>)" << std::endl;);
				return true;
			}
			for(int i=nBefore; i<getNumberOfMO(); i++)
			{
				// Here there should be a check whether the tile is already occupied..
				if(m_MO != NULL)
				{
					m_MO[i].setTileXY(MGFramework::randomN(m_Map.getWidth()), MGFramework::randomN(m_Map.getHeight()), this);
					m_MO[i].setDestTileXY(m_MO[i].getTileX(), m_MO[i].getTileY());
					m_MO[i].setSpeed(0.5, m_Map.getTileHeight()); // Move two tiles per second
					m_MO[i].setOwner(owner);
					m_Map.occupy(m_MO[i].getTileX(), m_MO[i].getTileY(), m_MO[i].getID());
				}
				if(m_MO == NULL)
				{
					MGFLOG_WARNING(std::cout << "m_MO = NULL and getNumberOfMO() = " << getNumberOfMO() << std::endl;)
				}
			}
			return true;
		}


		case MGComponent_CREATE_PE_INT_PARAMLIST:
		{
			int n = toInt(cmdvec[2]);
			int owner = 0;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1]);
					++i;
				}
				else
				{
					MGFLOG_ERROR(std::cout << "Error in command (create pe <n>), bad parameter list" << std::endl;);
					n = 0; // Abort PE creation..
				}
			}
			if(n > 0)
			{
				// Create the new MOs.
				createPE(n);
			}
			else
			{
				MGFLOG_ERROR(std::cout << "Error in command (create pe <n>)" << std::endl;);
				return true;
			}
			for(int i=0;i<getNumberOfPE();i++)
			{
				if(m_PE != NULL)
				{
					m_PE[i].setOwner(owner);
				}
				if(m_PE == NULL)
				{
					MGFLOG_WARNING(std::cout << "m_PE = NULL and getNumberOfPE() = " << getNumberOfPE() << std::endl;)
				}
			}
			return true;
		}


		case MGComponent_ADD_PE_INT_PARAMLIST:
		{
			int nBefore=getNumberOfPE();
			int n = toInt(cmdvec[2]);
			int owner = 0;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1]);
					++i;
				}
				else
				{
					MGFLOG_ERROR(std::cout << "Error in command (add pe <n>), bad parameter list" << std::endl;);
					n = 0; // Abort PE creation..
				}
			}
			if(n>0)
			{
				addPE(n);
			}
			else
			{
				MGFLOG_ERROR(std::cout << "Error in command (add pe <n>)" << std::endl;);
				return true;
			}
			for(int i=nBefore; i<getNumberOfPE(); i++)
			{
				if(m_PE != NULL)
				{
					m_PE[i].setOwner(owner);
				}
				if(m_PE == NULL)
				{
					MGFLOG_WARNING(std::cout << "m_PE = NULL and getNumberOfPE() = " << getNumberOfPE() << std::endl;)
				}
			}
			return true;
		}


		case MGComponent_OPEN_TERMINALSERVER:
		{
			MGFLOG_INFO(std::cout << "Opening terminal server..." << std::endl;);
#ifndef MGF_DEBUGGING_ENABLED
			openSocketTerminal();
			m_SocketTerminal = SDL_CreateThread(runMGFrameworkSocketTerminal, this);
#endif
			return true;
		}

		case MGComponent_CLOSE_TERMINALSERVER:
		{
			MGFLOG_INFO(std::cout << "Closing terminal server..." << std::endl;);
#ifndef MGF_DEBUGGING_ENABLED
			if(socketTerminalOpen())
			{
				closeSocketTerminal();
			}
#endif
			return true;
		}

		case MGComponent_MO_INT_X:
		case MGComponent_MO_INT_MARK:
		{
			int moIndex=toInt(cmdvec[1]);
			if(m_MO != NULL && moIndex >= 0 && moIndex < getNumberOfMO())
			{
				return m_MO[toInt(cmdvec[1])].runConsoleCommand(c, this);
			}
			if(m_MO == NULL)
			{
				MGFLOG_WARNING(std::cout << "m_MO = NULL and getNumberOfMO() = " << getNumberOfMO() << std::endl;)
			}
			MGFLOG_WARNING(std::cout << "Console command was not forwarded to MO " << moIndex << std::endl;); 
			return true;
		}

		case MGComponent_MO_MARKED_X:
		{
			for(int i=0; i<getNumberOfMO(); i++)
			{
				if(m_MO != NULL && m_MO[i].isMarked())
				{
					m_MO[i].runConsoleCommand(c, this);
				}
				if(m_MO == NULL)
				{
					MGFLOG_ERROR(std::cout << "m_MO = NULL and getNumberOfMO() = " << getNumberOfMO() << std::endl;)
				}
			}
			return true;
		}

		case MGComponent_MO_ALL_X:
		{
			for(int i=0; i<getNumberOfMO(); i++)
			{
				if(m_MO != NULL)
				{
					m_MO[i].runConsoleCommand(c, this);
				}
				if(m_MO == NULL)
				{
					MGFLOG_ERROR(std::cout << "m_MO = NULL and getNumberOfMO() = " << getNumberOfMO() << std::endl;)
				}
			}
			return true;
		}

		case MGComponent_LOGGING_ON:
		{
			enableLogging();
			MGFLOG_INFO(std::cout << "Logging enabled." << std::endl;);
			return true;
		}

		case MGComponent_LOGGING_OFF:
		{
			MGFLOG_INFO(std::cout << "Logging disabled." << std::endl;);
			disableLogging();
			return true;
		}

		case MGComponent_MINIMAP_ON:
		{
			enableMiniMap();
			MGFLOG_INFO(std::cout << "Mini map enabled." << std::endl;);
			return true;
		}

		case MGComponent_MINIMAP_OFF:
		{
			disableMiniMap();
			MGFLOG_INFO(std::cout << "Mini map disabled." << std::endl;);
			return true;
		}

		case MGComponent_RUNFRAMES_INT:
		{
			enableFrameCountdown();
			setFrameNumber(toInt(cmdvec[1]));
			return false;
		}

		case MGComponent_EXIT:
		{
			std::cout << "Exiting console..." << std::endl;
			return false;
		}

		case MGComponent_EXIT_APPLICATION:
		{
			std::cout << "Exiting application..." << std::endl;
			quit();
			return false;
		}

		case MGComponent_HELP:
		{
			std::cout << "Command help" << std::endl;
			std::cout << "[<object> [<object identifier>]] <command> [<parameter 1> ... <parameter n>]" << std::endl;
			std::cout << "-----------------------------------------------------------------------------" << std::endl << std::endl;
			std::cout << "help -    Displays this command help information." << std::endl;
			std::cout << "exit -    Exists shell and resumes framework execution." << std::endl;
			std::cout << "minimap <on|off> - Turns mini map on/off." << std::endl;
			std::cout << "logging <on|off> - Turns logging on/off." << std::endl;
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

			(void)m_Map.runConsoleCommand("map help", this);
			(void)m_Window.runConsoleCommand("window help", this);
			if(m_MO != NULL && getNumberOfMO() > 0)(void)m_MO[0].runConsoleCommand("mo 0 help", this);

			return true;
		}

		case MGComponent_GETFPS:
		{
			MGFLOG_INFO(std::cout << "" << getFPS() << std::endl;);
			return true;
		}

		case MGComponent_GETNUMBEROFMARKEDMO:
		{
			MGFLOG_INFO(std::cout << "" << getNumberOfMarkedMO() << std::endl;);
			return true;
		}

		case MGComponent_GETNUMBEROFMO:
		{
			MGFLOG_INFO(std::cout << "" << getNumberOfMO() << std::endl;);
			return true;
		}

		case MGComponent_INPUT_ON:
		{
			enableInput();
			return true;
		}

		case MGComponent_INPUT_OFF:
		{
			disableInput();
			return true;
		}

		case MGComponent_EXPECT_GETNUMBEROFMARKEDMO_INT:
		{
			int exp = toInt(cmdvec[2]);
			if(exp == getNumberOfMarkedMO())
			{
				MGFLOG_INFO(std::cout << "MGFramework::detectComponentConsoleCommand found the expected value (" << exp << ")" << std::endl;);
			}
			else
			{
				MGFLOG_ERROR(std::cout << "MGFramework::detectComponentConsoleCommand did not find the expected value (" << exp << " != " << getNumberOfMarkedMO() << ")" << std::endl;);
			}
			return true;
		}

		case MGComponent_EXPECT_GETNUMBEROFMO_INT:
		{
			int exp = toInt(cmdvec[2]);
			if(exp == getNumberOfMO())
			{
				MGFLOG_INFO(std::cout << "MGFramework::detectComponentConsoleCommand found the expected value (" << exp << ")" << std::endl;);
			}
			else
			{
				MGFLOG_ERROR(std::cout << "MGFramework::detectComponentConsoleCommand did not find the expected value (" << exp << " != " << getNumberOfMO() << ")" << std::endl;);
			}
			return true;
		}

		case MGComponent_EXPECT_GETNUMBEROFPE_INT:
		{
			int exp = toInt(cmdvec[2]);
			if(exp == getNumberOfPE())
			{
				MGFLOG_INFO(std::cout << "MGFramework::detectComponentConsoleCommand found the expected value (" << exp << ")" << std::endl;);
			}
			else
			{
				MGFLOG_ERROR(std::cout << "MGFramework::detectComponentConsoleCommand did not find the expected value (" << exp << " != " << getNumberOfPE() << ")" << std::endl;);
			}
			return true;
		}

		default:
			MGFLOG_ERROR(std::cout << "MGFramework::detectComponentConsoleCommand returned a bad value" << std::endl;); 
			return true;
	}

	std::cout << "Unknown command" << std::endl;
	return true;
}


eMGComponentConsoleCommand MGFramework::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	if(cmdvec.size() == 1)
	{
		if(cmdvec[0]=="exit")
		{
			return MGComponent_EXIT;
		}
		else if(cmdvec[0]=="help")
		{
			return MGComponent_HELP;
		}
		else if(cmdvec[0]=="getfps")
		{
			return MGComponent_GETFPS;
		}
		else if(cmdvec[0]=="getnumberofmarkedmo")
		{
			return MGComponent_GETNUMBEROFMARKEDMO;
		}
		else if(cmdvec[0]=="getnumberofmo")
		{
			return MGComponent_GETNUMBEROFMO;
		}

	}
	else if(cmdvec.size() == 2)
	{
		if(cmdvec[0]=="setfps")
		{
			return MGComponent_SETFPS_INT;
		}
		else if(cmdvec[0]=="open" && cmdvec[1]=="terminalserver")
		{
			return MGComponent_OPEN_TERMINALSERVER;
		}
		else if(cmdvec[0]=="close" && cmdvec[1]=="terminalserver")
		{
			return MGComponent_CLOSE_TERMINALSERVER;
		}
		else if(cmdvec[0]=="logging" && cmdvec[1]=="on")
		{
			return MGComponent_LOGGING_ON;
		}
		else if(cmdvec[0]=="logging" && cmdvec[1]=="off")
		{
			return MGComponent_LOGGING_OFF;
		}
		else if(cmdvec[0]=="minimap" && cmdvec[1]=="on")
		{
			return MGComponent_MINIMAP_ON;
		}
		else if(cmdvec[0]=="minimap" && cmdvec[1]=="off")
		{
			return MGComponent_MINIMAP_OFF;
		}
		else if(cmdvec[0]=="runframes")
		{
			return MGComponent_RUNFRAMES_INT;
		}
		else if(cmdvec[0]=="exit" && cmdvec[1]=="application")
		{
			return MGComponent_EXIT_APPLICATION;
		}
		else if(cmdvec[0]=="input" && cmdvec[1]=="on")
		{
			return MGComponent_INPUT_ON;
		}
		else if(cmdvec[0]=="input" && cmdvec[1]=="off")
		{
			return MGComponent_INPUT_OFF;
		}
	}
	else if(cmdvec.size() > 2)
	{
		if(cmdvec[0]=="create" && cmdvec[1]=="mo")
		{
			return MGComponent_CREATE_MO_INT_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="add" && cmdvec[1]=="mo")
		{
			return MGComponent_ADD_MO_INT_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="create" && cmdvec[1]=="pe")
		{
			return MGComponent_CREATE_PE_INT_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="add" && cmdvec[1]=="pe")
		{
			return MGComponent_ADD_PE_INT_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="mo" && cmdvec[1]=="marked")
		{
			return MGComponent_MO_MARKED_X;
		}
		else if(cmdvec[0]=="mo" && cmdvec[1]=="all")
		{
			return MGComponent_MO_ALL_X;
		}
		else if(cmdvec[0]=="mo")
		{
			return MGComponent_MO_INT_X;
		}
		else if(cmdvec[0]=="pe" && cmdvec[1]=="all")
		{
			return MGComponent_PE_ALL_X;
		}
		else if(cmdvec[0]=="pe")
		{
			return MGComponent_PE_INT_X;
		}
		else if(cmdvec[0]=="delete" && cmdvec[1]=="all" && cmdvec[2]=="mo" )
		{
			return MGComponent_DELETE_ALL_MO_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="delete" && cmdvec[1]=="all" && cmdvec[2]=="pe" )
		{
			return MGComponent_DELETE_ALL_PE_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="expect" && cmdvec[1]=="getnumberofmarkedmo")
		{
			return MGComponent_EXPECT_GETNUMBEROFMARKEDMO_INT;
		}
		else if(cmdvec[0]=="expect" && cmdvec[1]=="getnumberofmo")
		{
			return MGComponent_EXPECT_GETNUMBEROFMO_INT;
		}
		else if(cmdvec[0]=="expect" && cmdvec[1]=="getnumberofpe")
		{
			return MGComponent_EXPECT_GETNUMBEROFPE_INT;
		}
	}


	if(cmdvec.size() > 1)
	{
		if(cmdvec[0]=="map")
		{
			return MGComponent_MAP_X;
		}
		else if(cmdvec[0]=="window")
		{
			return MGComponent_WINDOW_X;
		}
	}

	// MGFramework failed to detect a proper command in the given string..
	return MGComponent_UNDEFINED;
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
			MGFLOG_WARNING(std::cout << "FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << "m_FrameTime:" << m_FrameTime << ", result:" << result << std::endl;)
			return getDesiredFPS();
		}
	}
	else
	{
		MGFLOG_WARNING(std::cout << "FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << m_FrameTime << std::endl;)
		return getDesiredFPS();
	}
}

void MGFramework::setDesiredFPS(Uint32 f)
{
	if(f >= 1) // Don't allow an FPS lower than 1
	{
		m_FPS = f;
	}
	else
	{
		MGFLOG_WARNING(std::cout << "FPS incorrectly calculated (void MGFramework::setDesiredFPS(Uint32 f)): " << f << std::endl;)
	}
}

Uint32 MGFramework::getDesiredFPS()
{
	if(m_FPS >= 1) // Don't allow an FPS lower than 1
	{
		return m_FPS;
	}
	else
	{
		MGFLOG_WARNING(std::cout << "FPS incorrectly calculated (Uint32 MGFramework::getDesiredFPS()): " << m_FPS << ", returning 1" << std::endl;)
		return 1;
	}
}



bool MGFramework::setWindowProperties(int width, int height, int bpp, bool fullscreen, const string& title)
{
	m_Window.setProperties(width, height, bpp, fullscreen, title);
	m_WindowPropertiesSet = true;
	return true;
}

bool MGFramework::setWindowProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const string& title)
{
	m_Window.setProperties(screenResolution, bpp, fullscreen, title);
	m_WindowPropertiesSet = true;
	return true;
}


void MGFramework::createMO(int n)
{
	delete[] m_MO;
	m_NMO=n;
	if(getNumberOfMO() > 0)
	{
		m_MO = new MGMovingObject[getNumberOfMO()];
	}
	else
	{
		m_MO = NULL;
		m_MarkedMOs = 0;
	}
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

void MGFramework::deleteMO(int index)
{
	if(index < 0 || index >= getNumberOfMO())
	{
		MGFLOG_ERROR(std::cout << "MGFramework::deleteMO was given a bad index: " << index << std::endl;)
	}
	else
	{
		m_Map.unOccupy(m_MO[index].getTileX(), m_MO[index].getTileY());

		for(int i=0; i<getNumberOfMO(); ++i)
		{
			if(i<index)
			{
				// Do nothing...
			}
			else if(i>=index && i<getNumberOfMO()-1)
			{
				// Overwrite mo(i) with mo(i+1)
				m_MO[i].copy(&m_MO[i+1]);
			}
			else if(i==getNumberOfMO()-1)
			{
				//No need to actually delete the MO since we will not access it if it's outside getNumberOfMO()...
				//delete m_MO[i];
			}
			else
			{
				MGFLOG_ERROR(std::cout << "MGFramework::deleteMO was not able to find the given index: " << index << std::endl;)
				return;
			}
		}
		m_NMO = getNumberOfMO()-1;
	}
}



void MGFramework::createPE(int n)
{
	delete[] m_PE;
	m_NPE=n;
	if(getNumberOfPE() > 0)
	{
		m_PE = new MGPeriodicEvent[getNumberOfPE()];
	}
	else
	{
		m_PE = NULL;
	}
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


void MGFramework::deletePE(int index)
{
	if(index < 0 || index >= getNumberOfPE())
	{
		MGFLOG_ERROR(std::cout << "MGFramework::deletePE was given a bad index: " << index << std::endl;)
	}
	else
	{
		for(int i=0; i<getNumberOfPE(); ++i)
		{
			if(i<index)
			{
				// Do nothing...
			}
			else if(i>=index && i<getNumberOfPE()-1)
			{
				// Overwrite pe(i) with mo(i+1)
				m_PE[i].copy(&m_PE[i+1]);
			}
			else if(i==getNumberOfPE()-1)
			{
				//No need to actually delete the PE since we will not access it if it's outside getNumberOfPE()...
				//delete m_PE[i];
			}
			else
			{
				MGFLOG_ERROR(std::cout << "MGFramework::deletePE was not able to find the given index: " << index << std::endl;)
				return;
			}
		}
		m_NPE = getNumberOfPE()-1;
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

#ifndef MGF_DEBUGGING_ENABLED
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
#endif

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

#ifndef MGF_DEBUGGING_ENABLED
int MGFramework::initializeWinsock(WORD wVersionRequested)
{
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);

	if (err!=0) return 0; // Tell the user that we couldn't find a usable winsock.dll 
	if (LOBYTE(wsaData.wVersion )!=1 || HIBYTE(wsaData.wVersion)!=1) return 0;
	//Everything is fine: proceed
	return 1;
}
#endif

bool MGFramework::okMGFrameworkSyntax(const char *c)
{
	// Dummy implementation
	if(strlen(c)>2)
	{
		return true;
	}
	return false;
}


void MGFramework::addConsoleCommandToQueue(const char *c)
{

}

#ifndef MGF_DEBUGGING_ENABLED
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
					mgf->runConsoleCommand(buf, mgf);
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
#endif