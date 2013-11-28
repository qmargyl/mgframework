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
	m_NSO(0),
	m_FrameTime(16),
	m_ActualFrameTime(16),
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
	m_InputEnabled(true),
	m_Font(0),
	m_DynamicFPSEnabled(true),
	m_Port(0)
{
	setDesiredFPS(20);
	std::srand((int)std::time(0));

	// Setup the framework for automatic regression testing...

	// At framework creation, no commands have been used..
	for(int i=0; i<MGComponent_NUMBEROFCOMMANDIDENTIFIERS; ++i)
	{
		m_UsedCommands[i] = false;
	}
	// MGComponent_UNDEFINED does not represent a command or forwarding a command
	m_UsedCommands[MGComponent_UNDEFINED] = true;

	// MGComponent_EXIT_APPLICATION is impossible to call before evaluating the 
	// number of used commands as it will terminate execution of the application
	m_UsedCommands[MGComponent_EXIT_APPLICATION] = true;

	m_SymbolTable = new MGSymbolTable();
}

MGFramework::~MGFramework()
{
	delete[] m_MO;
	delete[] m_PE;
	delete[] m_SO;
	delete m_SymbolTable;
}


bool MGFramework::processEvents()
{
	// Quit if it has been decided to do so.. also for server instances
	if(getQuitFlag()) return false;

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
				MGFLOG_INFO("SDL_QUIT")
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
					MGFLOG_INFO("SDL_KEYUP" << std::endl << "  " << SDL_GetKeyName(sym))
					m_Keys[sym] = 0;
				}
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
								MGFLOG_WARNING("m_MO = NULL and getNumberOfMO() = " << getNumberOfMO())
							}
						}

						activateFraming(event.button.x, event.button.y);
						MGFLOG_INFO("Map (left click): index = " << iClick << ", x = " << xClick << ", y = " << yClick)
					}
					else if (((int) event.button.button) == 3)
					{
						MGFLOG_INFO("Map (right click): index = " << iClick << ", x = " << xClick << ", y = " << yClick)
						m_Map.mouseScrollingClick(event.button.x, event.button.y);
					}

					// Log debug information to console.
					MGFLOG_INFO("SDL_MOUSEBUTTONDOWN: Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")")

				}
				else
				{
					MGFLOG_INFO("SDL_MOUSEBUTTONDOWN" << std::endl << "  " << "Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")")
					if (((int) event.button.button) == 1)
					{
						MGFLOG_INFO("Map (left click): index = -1, x = <not calculated>, y = <not calculated>")
					}
					else if (((int) event.button.button) == 3)
					{
						MGFLOG_INFO("Map (right click): index = -1, x = <not calculated>, y = <not calculated>")
					}
				}
				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				MGFLOG_INFO("SDL_MOUSEBUTTONUP: Button " << (int) event.button.button << " at (" << event.button.x << "," << event.button.y << ")")
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
									MGFLOG_WARNING("m_MO = NULL and getNumberOfMO() = " << getNumberOfMO())
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
						if(iClick >= 0)
						{
							int xClick = m_Map.getTileX(iClick);
							int yClick = m_Map.getTileY(iClick);
							char c[64];
							sprintf(c, "mo marked setdestination %d %d", xClick, yClick);
							runConsoleCommand(c, this, NULL);
						}
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

	return true;
}

int MGFramework::parse(const char *sFileName)
{
	int returnValue = 0;
	if(sFileName==NULL)
	{
		MGFLOG_INFO("MGFramework::parse was called with argument NULL, exiting function...");
		return returnValue;
	}

	MGFLOG_INFO("MGFramework::parse was called with argument " << sFileName);
	FILE *sf = NULL;
	char *functionName = NULL;
	char scriptFileName[128];
	bool foundFunction = false;
	bool globalScope = false;
	int lineNumber = 0;
	int skipToEndIf = 0;
	int insideIf = 0;
	MGSymbolTable *symbols = new MGSymbolTable();


	strcpy(scriptFileName,sFileName);

	std::size_t foundColon = string(scriptFileName).find(string(":"));
	if (foundColon!=std::string::npos && foundColon>0)
	{
		functionName = scriptFileName + foundColon + 1;
		scriptFileName[foundColon]='\0';
		MGFLOG_INFO("MGFramework::parse will parse file " << scriptFileName << ", function " << functionName);
		globalScope = false;
	}
	else
	{
		globalScope = true;
	}

	errno_t scriptError = fopen_s(&sf, scriptFileName, "rt");
	if(sf == NULL)
	{
		MGFLOG_ERROR("MGFramework::parse failed to open script file " << scriptFileName << ", error(" << scriptError << ")");
	}
	else
	{
		char scriptLine[MGF_SCRIPTLINE_MAXLENGTH] = "";
		char *neof = NULL;
		MGFLOG_INFO("MGFramework::parse starting to parse script file " << scriptFileName);
		if(functionName!=NULL)
		{
			MGFLOG_INFO("MGFramework::parse starting to execute function " << functionName);
		}
		else
		{
			foundFunction=true;
		}

		// No input during file parsing. Turn back on after parsing, and if it was enabled in the first place.
		bool inputEnabled = isInputEnabled();
		disableInput();

		while(true)
		{
			// Read until new line or end of file, whichever happens first..
			neof = fgets(scriptLine, MGF_SCRIPTLINE_MAXLENGTH, sf);
			if(neof == NULL)
			{
				MGFLOG_INFO("MGFramework::parse detected end of file");
				break;
			}
			else
			{
				lineNumber++;
				if((int)strlen(scriptLine) > MGF_SCRIPTLINE_MAXLENGTH)
				{
					MGFLOG_ERROR("MGFramework::parse skipped script line exceeding the max line length");
					continue;
				}

				// Change all special characters to spaces
				for(int i=0; i<(int)strlen(scriptLine); ++i)
				{
					if(scriptLine[i] <= 32) 
					{
						scriptLine[i] = 32;
					}
				}

				// Remove the newline and any tailing "special" characters before sending command to runConsoleCommand
				for(int i=(int)strlen(scriptLine); i>=0 ; --i)
				{
					if(i<0 || i>MGF_SCRIPTLINE_MAXLENGTH)
					{
						MGFLOG_ERROR("MGFramework::parse found inconsistent indexing: " << i << " s='" << scriptLine << "'");
						break;
					}
					else if(scriptLine[i] <= 32) // Remove characters from 0 to space in the ASCII table
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
					// XXX: This should be done differently: "//...", not "/..."
					if(scriptLine[i] == '/') // Remove comments
					{
						scriptLine[i] = '\0';
					}
				}

				// Create vector of strings (tokens) representing the current script line
				std::string s_scriptLine(scriptLine);
				std::vector<std::string> v_scriptLine = split(scriptLine, " ");

				if(foundFunction)
				{
					if(v_scriptLine.size() == 1 && v_scriptLine[0] == "end")
					{
						if(functionName!=NULL)
						{
							MGFLOG_INFO("MGFramework::parse found end of function");
							break;
						}
						else
						{
							MGFLOG_ERROR("MGFramework::parse found 'end' outside a function");
						}
					}
					else if(v_scriptLine.size() == 2 && v_scriptLine[0] == "return")
					{
						if(functionName!=NULL)
						{
							MGFLOG_INFO("MGFramework::parse found return from function");
							returnValue = toInt(v_scriptLine[1], symbols);
							break;
						}
						else
						{
							MGFLOG_ERROR("MGFramework::parse found 'return' outside a function");
						}						
					}
					else if(v_scriptLine.size()==4 && v_scriptLine[0] == "if")
					{
						if(v_scriptLine[2] == "==")
						{
							if(toInt(v_scriptLine[1], symbols) == toInt(v_scriptLine[3], symbols))
							{
								insideIf++;
							}
							else
							{
								skipToEndIf++;
							}
						}
						else if(v_scriptLine[2] == "!=")
						{
							if(toInt(v_scriptLine[1], symbols) != toInt(v_scriptLine[3], symbols))
							{
								insideIf++;
							}
							else
							{
								skipToEndIf++;
							}
						}
						else if(v_scriptLine[2] == ">")
						{
							if(toInt(v_scriptLine[1], symbols) > toInt(v_scriptLine[3], symbols))
							{
								insideIf++;
							}
							else
							{
								skipToEndIf++;
							}
						}
						else if(v_scriptLine[2] == "<")
						{
							if(toInt(v_scriptLine[1], symbols) < toInt(v_scriptLine[3], symbols))
							{
								insideIf++;
							}
							else
							{
								skipToEndIf++;
							}
						}
						else if(v_scriptLine[2] == ">=")
						{
							if(toInt(v_scriptLine[1], symbols) >= toInt(v_scriptLine[3], symbols))
							{
								insideIf++;
							}
							else
							{
								skipToEndIf++;
							}
						}
						else if(v_scriptLine[2] == "<=")
						{
							if(toInt(v_scriptLine[1], symbols) <= toInt(v_scriptLine[3], symbols))
							{
								insideIf++;
							}
							else
							{
								skipToEndIf++;
							}
						}
						else
						{
							MGFLOG_ERROR("Unsupported operator");
						}
					}
					else if(v_scriptLine.size() == 1 && v_scriptLine[0] == "endif")
					{
						if(skipToEndIf == 0 && insideIf == 0)
						{
							MGFLOG_ERROR("Unexpected 'endif'");
						}
						else if(skipToEndIf > 0)
						{
							skipToEndIf--;
						}
						else if(insideIf > 0)
						{
							insideIf--;
						}
					}
					else if(skipToEndIf == 0 && okMGFrameworkSyntax(v_scriptLine))
					{
						// function call..
						//MGFLOG_INFO("Tokens: " << v_scriptLine.size());

						if(v_scriptLine.size()==2 && v_scriptLine[0]=="call")
						{
							std::size_t fColon = v_scriptLine[1].find(string(":"));
							if (fColon!=std::string::npos)
							{
								MGFLOG_INFO("MGFramework::parse calling " << v_scriptLine[1].c_str());
								symbolAssignTo(	v_scriptLine[1], 
												MGComponent::toString(parse(v_scriptLine[1].c_str())), 
												symbols);
							}
							else
							{
								MGFLOG_INFO("MGFramework::parse calling " << (string(scriptFileName)+string(":")+v_scriptLine[1]).c_str());
								symbolAssignTo(	v_scriptLine[1], 
												MGComponent::toString(parse((string(scriptFileName)+string(":")+v_scriptLine[1]).c_str())), 
												symbols);
							}
							if(getQuitFlag())
							{
								MGFLOG_INFO("MGFramework::parse quitting after function return");
								break;
							}
						}
						// other statement..
						else
						{
							// Create a string of all tokens in the command
							string cmd=string("");
							for(unsigned int i=0; i<v_scriptLine.size(); ++i)
							{
								if(v_scriptLine[i]!=string(""))
								{
									cmd+=v_scriptLine[i];
									if(i<v_scriptLine.size()-1)
									{
										cmd+=string(" ");
									}
								}
							}
							MGFLOG_INFO("MGFramework::parse calls runConsoleCommand(" << cmd.c_str() << ")");
							if(globalScope)
							{
								runConsoleCommand(cmd.c_str(), this, NULL);
							}
							else
							{
								runConsoleCommand(cmd.c_str(), this, symbols);
							}
							if(getQuitFlag())
							{
								MGFLOG_INFO("MGFramework::parse quitting after running console command");
								break;
							}
						}
					}
				}
				else if(functionName!=NULL && v_scriptLine.size()==2 && v_scriptLine[0]=="function" && v_scriptLine[1]==string(functionName))
				{
					foundFunction=true;
				}
			}
		}

		// Enable input if it was enabled when we started parsing.
		if(inputEnabled) enableInput();

		if(sf != NULL)
		{
			fclose(sf);
		}

		MGFLOG_INFO("MGFramework::parse finished parsing script file " << scriptFileName);
	}
	delete symbols;
	return returnValue;
}

void MGFramework::logEval(const char *logFileName)
{
	FILE *lf = NULL;
	errno_t logError = fopen_s(&lf, logFileName, "rt");

	int nErrors=0;
	int nWarnings=0;
	int nExitApp=0;
	std::string execTimeMS("");

	if(lf == NULL)
	{
		MGFLOG_ERROR("MGFramework::logEval failed to open log file " << logFileName << ", error(" << logError << ")");
	}
	else
	{
		std::cout << "Evaluating file: " << logFileName << " ... ";

		char logLine[MGF_LOGLINE_MAXLENGTH] = "";
		char *neof = NULL;
		//MGFLOG_INFO(std::cout << "MGFramework::logEval starting to parse log file " << logFileName << std::endl;);



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
				std::string warnSubstr("WARNING");
				std::string exitSubstr("Exiting application...");
				std::size_t foundErr = line.find(errSubstr);
				std::size_t foundWarn = line.find(warnSubstr);
				std::size_t foundExit = line.find(exitSubstr);
				if (foundErr != std::string::npos) nErrors++;
				if (foundWarn != std::string::npos) nWarnings++;
				if (foundExit != std::string::npos) nExitApp++;


				std::string exetimeSubstr("Execution time: ");
				std::size_t foundExecutionTime = line.find(exetimeSubstr);
				if (foundExecutionTime != std::string::npos)
				{
					execTimeMS = line.substr(exetimeSubstr.size(), line.size() - exetimeSubstr.size() -1);
					//C++11: int execTimeMS = std::stoi(execTime);
				}
			}
		}

		if(nErrors != 0)
		{
			std::cout << "FAIL (" << nErrors << " errors, " << nWarnings << " warnings)";
		}
		else if(nExitApp == 0)
		{
			std::cout << "FAIL (did not finish)";
		}
		else
		{
			std::cout << "PASS";
			if(nWarnings>0)
			{
				std::cout << " (" << nWarnings << " warnings)";
			}
		}
		std::cout << ", " << execTimeMS.c_str() << " ms" << std::endl;

		if(lf != NULL)
		{
			fclose(lf);
		}

		//MGFLOG_INFO(std::cout << "MGFramework::logEval finished parsing log file " << logFileName << std::endl;);
	}
}


void MGFramework::run(const char *scriptFileName, bool runOneFrame)
{
	int result = parse(scriptFileName);

	Uint32 frameStartTime = 0; 
	if(!runOneFrame) m_DelayTime = 0;
	// Assume an initial value of m_FrameTime of 1000/getDesiredFPS().
	Uint32 lastFrameTime = MGF_GetExecTimeMS()-(1000/getDesiredFPS()); // MGF_GetExecTimeMS() - lastFrameTime cannot be zero.

	// Frame loop (processEvents, handleGameLogics, draw)
	while(processEvents())
	{
		// Calculate the current frame time (and implicitly FPS)..
		m_FrameTime = MGF_GetExecTimeMS() - lastFrameTime; //number of milli seconds from last frame was handled
		lastFrameTime = MGF_GetExecTimeMS();
		frameStartTime = MGF_GetExecTimeMS();

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
				// XXX: Print warnings based on the KPIs?
				sleepKPI = 0;
				nFrames = 0;
				if(!runOneFrame)activateConsole();
			}
			else if (getFrameNumber() > 0)
			{
				countdownFrame(1);
				sleepKPI += m_DelayTime; // Add all delay times for the frames counted down as a KPI for performance.
				nFrames ++;
			}
			else
			{
				MGFLOG_ERROR("Error in frame countdown feature");
			}
		}

		// General game logics..
		handleMGFGameLogics();

		// Application specific game logics and graphics..
		handleGameLogics();
		
		// A server instance of the framework has no graphics.
		if(getInstanceType() != MGFSERVERINSTANCE) 
		{
			draw();
			SDL_Flip(getSurface());
		}

		// Sleep if there is time to spare..
		m_DelayTime = (1000/getDesiredFPS()) - (MGF_GetExecTimeMS() - frameStartTime);

		if(m_DelayTime > 0)
		{
			Sleep((DWORD)m_DelayTime);
		}

		m_ActualFrameTime = MGF_GetExecTimeMS() - frameStartTime;
		if(runOneFrame) break;
	}
}


void MGFramework::handleMGFGameLogics()
{
	// Handle all events in the event queue
	// Not implemented yet

	// Update periodic event to trigger rare events
	for(int i=0;i<getNumberOfPE();i++)
	{
		if(m_PE[i].update())
		{
			MGFLOG_INFO("PE triggered an update");
			m_PE[i].runFile1(this);
		}
	}

	// Update all moving objects
	for(int i=0;i<getNumberOfMO();i++)
	{
		m_MO[i].update(this);
	}

	// Example of how FPS can be controlled dynamically
	if(getDynamicFPSEnabled())
	{
		if(getLastFrameDelayTime() > 10)
		{
			setDesiredFPS(getFPS() + 1);
		}
		else if(getLastFrameDelayTime() < 7)
		{
			setDesiredFPS(biggest(1, getFPS() - 1));
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
	}while(runConsoleCommand(cLine.c_str(), this, NULL));
	disableTyping();
}



bool MGFramework::runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s)
{
	char cmd[MGF_SCRIPTLINE_MAXLENGTH];
	strcpy(cmd, c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, " ");

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
		{
			MGFLOG_ERROR("MGFramework::runConsoleCommand received MGComponent_UNDEFINED from MGFramework::detectMGComponentConsoleCommand"); 
			break;
		}

		case MGComponent_RUNONEFRAME:
		{
			registerUsedCommand(MGComponent_RUNONEFRAME);
			run(NULL, true);
			return true;
		}

		case MGComponent_MAP_X:
		{
			registerUsedCommand(MGComponent_MAP_X);
			return m_Map.runConsoleCommand(c, this, s);
		}

		case MGComponent_WINDOW_X:
		{
			registerUsedCommand(MGComponent_WINDOW_X);
			return m_Window.runConsoleCommand(c, this, s);
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
			registerUsedCommand(MGComponent_PE_INT_X);
			int peIndex=toInt(cmdvec[1], s);
			if(peIndex >= 0 && peIndex < getNumberOfPE())
			{
				return m_PE[toInt(cmdvec[1], s)].runConsoleCommand(c, this, s);
			}
			MGFLOG_WARNING("Console command was not forwarded to PE " << peIndex); 
			return true;
		}

		case MGComponent_PE_ALL_X:
		{
			registerUsedCommand(MGComponent_PE_ALL_X);
			for(int i=0; i<getNumberOfPE(); i++)
			{
				m_PE[i].runConsoleCommand(c, this, s);
			}
			return true;
		}

		case MGComponent_SO_INT_X:
		{
			registerUsedCommand(MGComponent_SO_INT_X);
			int soIndex=toInt(cmdvec[1], s);
			if(soIndex >= 0 && soIndex < getNumberOfSO())
			{
				return m_SO[toInt(cmdvec[1], s)].runConsoleCommand(c, this, s);
			}
			MGFLOG_WARNING("Console command was not forwarded to SO " << soIndex); 
			return true;
		}

		case MGComponent_SO_ALL_X:
		{
			registerUsedCommand(MGComponent_SO_ALL_X);
			for(int i=0; i<getNumberOfSO(); i++)
			{
				m_SO[i].runConsoleCommand(c, this, s);
			}
			return true;
		}

		case MGComponent_SETFPS_INT:
		{
			registerUsedCommand(MGComponent_SETFPS_INT);
			setDesiredFPS(toInt(cmdvec[1], s));
			return true;
		}


		case MGComponent_CREATE_MO_INT_PARAMLIST:
		{
			registerUsedCommand(MGComponent_CREATE_MO_INT_PARAMLIST);
			int n = toInt(cmdvec[2], s);
			int owner = 0;
			int x = -1; // Invalid default value
			int y = -1; // Invalid default value
			int speed = 2; // Tiles per second.
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1], s);
					++i;
				}
				else if(cmdvec[i]=="-x" && cmdvec.size() > (i + 1))
				{
					x = toInt(cmdvec[i+1], s);
					++i;
					if(n!=1)
					{
						MGFLOG_ERROR("Parameter -x can only be set when creating one MO");
						n = 0; // Abort MO creation..
					}
					
				}
				else if(cmdvec[i]=="-y" && cmdvec.size() > (i + 1))
				{
					y = toInt(cmdvec[i+1], s);
					++i;
					if(n!=1)
					{
						MGFLOG_ERROR("Parameter -y can only be set when creating one MO");
						n = 0; // Abort MO creation..
					}
					
				}
				else if(cmdvec[i]=="-speed" && cmdvec.size() > (i + 1))
				{
					speed = toInt(cmdvec[i+1], s);
					++i;
				}
				else
				{
					MGFLOG_ERROR("Error in command (create mo <n>), bad parameter list");
					n = 0; // Abort MO creation..
				}
			}
			if(n > 0)
			{
				createMO(n);
			}
			else
			{
				MGFLOG_ERROR("Error in command (create mo <n>)");
				return true;
			}

			if(m_MO != NULL)
			{
				for(int i=0;i<getNumberOfMO();i++)
				{
					// If setup fails we must setup the same index again 
					// since the failing MO has been deleted.
					if(!setupMO(i, x, y, owner, speed))--i;
				}
			}
			else
			{
				MGFLOG_ERROR("MO storage is undefined");
			}
			return true;
		}



		case MGComponent_DELETE_ALL_MO_PARAMLIST:
		{
			registerUsedCommand(MGComponent_DELETE_ALL_MO_PARAMLIST);
			int owner = 0;
			bool ownerParamSet=false;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1], s);
					ownerParamSet=true;
					++i;
				}
				else
				{
					MGFLOG_ERROR("Error in command (delete all mo), bad parameter list");
					return true;
				}
			}

			if(ownerParamSet)
			{
				// Delete only MOs connected to a specific owner
				MGFLOG_INFO("Deleting MOs owned by " << owner);
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
				MGFLOG_INFO("Creating zero MO...");
				createMO(0);
			}
			return true;
		}



		case MGComponent_DELETE_ALL_PE_PARAMLIST:
		{
			registerUsedCommand(MGComponent_DELETE_ALL_PE_PARAMLIST);
			int owner = 0;
			bool ownerParamSet=false;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1], s);
					ownerParamSet=true;
					++i;
				}
				else
				{
					MGFLOG_ERROR("Error in command (delete all pe), bad parameter list");
					return true;
				}
			}

			if(ownerParamSet)
			{
				// Delete only PEs connected to a specific owner
				MGFLOG_INFO("Deleting PEs owned by " << owner);
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
				MGFLOG_INFO("Creating zero PE...");
				createPE(0);
			}
			return true;
		}



		case MGComponent_DELETE_ALL_SO_PARAMLIST:
		{
			registerUsedCommand(MGComponent_DELETE_ALL_SO_PARAMLIST);
			int owner = 0;
			bool ownerParamSet=false;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1], s);
					ownerParamSet=true;
					++i;
				}
				else
				{
					MGFLOG_ERROR("Error in command (delete all so), bad parameter list");
					return true;
				}
			}

			if(ownerParamSet)
			{
				// Delete only MOs connected to a specific owner
				MGFLOG_INFO("Deleting SOs owned by " << owner);
				for(int i = getNumberOfSO()-1; i>=0; --i)
				{
					if(m_SO[i].getOwner() == owner)
					{
						deleteSO(i);
					}
				}
			}
			else
			{
				//Deleting ALL SOs is faster than deleting them per owner 
				MGFLOG_INFO("Creating zero SO...");
				createSO(0);
			}
			return true;
		}


		case MGComponent_ADD_MO_INT_PARAMLIST:
		{
			registerUsedCommand(MGComponent_ADD_MO_INT_PARAMLIST);
			int nBefore=getNumberOfMO();
			int n = toInt(cmdvec[2], s);
			int owner = 0;
			int x = -1; // Invalid default value
			int y = -1; // Invalid default value
			int speed = 2; // Tiles per second.
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1], s);
					++i;
				}
				else if(cmdvec[i]=="-x" && cmdvec.size() > (i + 1))
				{
					x = toInt(cmdvec[i+1], s);
					++i;
					if(n!=1)
					{
						MGFLOG_ERROR("Parameter -x can only be set when adding one MO");
						n = 0; // Abort MO creation..
					}
					
				}
				else if(cmdvec[i]=="-y" && cmdvec.size() > (i + 1))
				{
					y = toInt(cmdvec[i+1], s);
					++i;
					if(n!=1)
					{
						MGFLOG_ERROR("Parameter -y can only be set when adding one MO");
						n = 0; // Abort MO creation..
					}
					
				}
				else if(cmdvec[i]=="-speed" && cmdvec.size() > (i + 1))
				{
					speed = toInt(cmdvec[i+1], s);
					++i;
				}
				else
				{
					MGFLOG_ERROR("Error in command (add mo <n>), bad parameter list");
					n = 0; // Abort MO creation..
				}
			}
			if(n>0)
			{
				addMO(n);
			}
			else
			{
				MGFLOG_ERROR("Error in command (add mo <n>)");
				return true;
			}

			if(m_MO != NULL)
			{
				for(int i=nBefore; i<getNumberOfMO(); i++)
				{
					// If setup fails we must setup the same index again 
					// since the failing MO has been deleted.
					if(!setupMO(i, x, y, owner, speed))--i;
				}
			}
			else
			{
				MGFLOG_ERROR("MO storage is undefined");
			}
			return true;
		}



		case MGComponent_ADD_SO_INT_PARAMLIST:
		{
			registerUsedCommand(MGComponent_ADD_SO_INT_PARAMLIST);
			int nBefore=getNumberOfSO();
			int n = toInt(cmdvec[2], s);
			int x = -1; // Invalid default value
			int y = -1; // Invalid default value
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-x" && cmdvec.size() > (i + 1))
				{
					x = toInt(cmdvec[i+1], s);
					++i;
					if(n!=1)
					{
						MGFLOG_ERROR("Parameter -x can only be set when adding one SO");
						n = 0; // Abort SO creation..
					}
					
				}
				else if(cmdvec[i]=="-y" && cmdvec.size() > (i + 1))
				{
					y = toInt(cmdvec[i+1], s);
					++i;
					if(n!=1)
					{
						MGFLOG_ERROR("Parameter -y can only be set when adding one SO");
						n = 0; // Abort SO creation..
					}
					
				}
				else
				{
					MGFLOG_ERROR("Error in command (add so <n>), bad parameter list");
					n = 0; // Abort SO creation..
				}
			}
			if(n>0)
			{
				addSO(n);
			}
			else
			{
				MGFLOG_ERROR("Error in command (add so <n>)");
				return true;
			}

			if(m_SO != NULL)
			{
				for(int i=nBefore; i<getNumberOfSO(); i++)
				{
					// If setup fails we must setup the same index again 
					// since the failing SO has been deleted.
					if(!setupSO(i, x, y))--i;
				}
			}
			else
			{
				MGFLOG_ERROR("SO storage is undefined");
			}
			return true;
		}




		case MGComponent_CREATE_PE_INT_PARAMLIST:
		{
			registerUsedCommand(MGComponent_CREATE_PE_INT_PARAMLIST);
			int n = toInt(cmdvec[2], s);
			int owner = 0;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1], s);
					++i;
				}
				else
				{
					MGFLOG_ERROR("Error in command (create pe <n>), bad parameter list");
					n = 0; // Abort PE creation..
				}
			}
			if(n > 0)
			{
				// Create the new PEs.
				createPE(n);
			}
			else
			{
				MGFLOG_ERROR("Error in command (create pe <n>)");
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
					MGFLOG_WARNING("m_PE = NULL and getNumberOfPE() = " << getNumberOfPE())
				}
			}
			return true;
		}


		case MGComponent_ADD_PE_INT_PARAMLIST:
		{
			registerUsedCommand(MGComponent_ADD_PE_INT_PARAMLIST);
			int nBefore=getNumberOfPE();
			int n = toInt(cmdvec[2], s);
			int owner = 0;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i+1], s);
					++i;
				}
				else
				{
					MGFLOG_ERROR("Error in command (add pe <n>), bad parameter list");
					n = 0; // Abort PE creation..
				}
			}
			if(n>0)
			{
				addPE(n);
			}
			else
			{
				MGFLOG_ERROR("Error in command (add pe <n>)");
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
					MGFLOG_WARNING("m_PE = NULL and getNumberOfPE() = " << getNumberOfPE())
				}
			}
			return true;
		}


		case MGComponent_CREATE_SO_INT_PARAMLIST:
		{
			registerUsedCommand(MGComponent_CREATE_SO_INT_PARAMLIST);
			int n = toInt(cmdvec[2], s);
			int x = -1; // Invalid default value
			int y = -1; // Invalid default value
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-x" && cmdvec.size() > (i + 1))
				{
					x = toInt(cmdvec[i+1], s);
					++i;
					if(n!=1)
					{
						MGFLOG_ERROR("Parameter -x can only be set when creating one SO");
						n = 0; // Abort MO creation..
					}
					
				}
				else if(cmdvec[i]=="-y" && cmdvec.size() > (i + 1))
				{
					y = toInt(cmdvec[i+1], s);
					++i;
					if(n!=1)
					{
						MGFLOG_ERROR("Parameter -y can only be set when creating one SO");
						n = 0; // Abort MO creation..
					}
					
				}
				else
				{
					MGFLOG_ERROR("Error in command (create so <n>), bad parameter list");
					n = 0; // Abort MO creation..
				}
			}
			if(n > 0)
			{
				createSO(n);
			}
			else
			{
				MGFLOG_ERROR("Error in command (create so <n>)");
				return true;
			}

			if(m_SO != NULL)
			{
				for(int i=0;i<getNumberOfSO();i++)
				{
					// If setup fails we must setup the same index again 
					// since the failing SO has been deleted.
					if(!setupSO(i, x, y))--i;
				}
			}
			else
			{
				MGFLOG_ERROR("SO storage is undefined");
			}
			return true;
		}


		case MGComponent_OPEN_TERMINALSERVER:
		{
			registerUsedCommand(MGComponent_OPEN_TERMINALSERVER);
			MGFLOG_INFO("Opening terminal server...");
#ifndef MGF_DEBUGGING_ENABLED
			openSocketTerminal();
			m_SocketTerminal = SDL_CreateThread(runMGFrameworkSocketTerminal, this);
#endif
			return true;
		}

		case MGComponent_CLOSE_TERMINALSERVER:
		{
			registerUsedCommand(MGComponent_CLOSE_TERMINALSERVER);
			MGFLOG_INFO("Closing terminal server...");
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
			registerUsedCommand(MGComponent_MO_INT_X);
			int moIndex=toInt(cmdvec[1], s);
			if(m_MO != NULL && moIndex >= 0 && moIndex < getNumberOfMO())
			{
				return m_MO[toInt(cmdvec[1], s)].runConsoleCommand(c, this, s);
			}
			if(m_MO == NULL)
			{
				MGFLOG_ERROR("m_MO = NULL and getNumberOfMO() = " << getNumberOfMO())
			}
			MGFLOG_WARNING("Console command was not forwarded to MO " << moIndex); 
			return true;
		}

		case MGComponent_MO_MARKED_X:
		{
			registerUsedCommand(MGComponent_MO_MARKED_X);
			for(int i=0; i<getNumberOfMO(); i++)
			{
				if(m_MO != NULL && m_MO[i].isMarked())
				{
					m_MO[i].runConsoleCommand(c, this, s);
				}
				else if(m_MO == NULL)
				{
					MGFLOG_ERROR("m_MO = NULL and getNumberOfMO() = " << getNumberOfMO())
				}
			}
			return true;
		}

		case MGComponent_MO_ALL_X:
		{
			registerUsedCommand(MGComponent_MO_ALL_X);
			for(int i=0; i<getNumberOfMO(); i++)
			{
				if(m_MO != NULL)
				{
					m_MO[i].runConsoleCommand(c, this, s);
				}
				else
				{
					MGFLOG_ERROR("m_MO = NULL and getNumberOfMO() = " << getNumberOfMO())
				}
			}
			return true;
		}

		case MGComponent_DELETE_MO_INT:
		{
			registerUsedCommand(MGComponent_DELETE_MO_INT);
			int moIndex=toInt(cmdvec[2], s);
			if(m_MO != NULL && moIndex >= 0 && moIndex < getNumberOfMO())
			{
				deleteMO(moIndex);
			}
			else if(m_MO == NULL)
			{
				MGFLOG_ERROR("m_MO = NULL and getNumberOfMO() = " << getNumberOfMO())
			}
			return true;
		}

		case MGComponent_DELETE_SO_INT:
		{
			registerUsedCommand(MGComponent_DELETE_SO_INT);
			int soIndex=toInt(cmdvec[2], s);
			if(m_SO != NULL && soIndex >= 0 && soIndex < getNumberOfSO())
			{
				deleteSO(soIndex);
			}
			else if(m_SO == NULL)
			{
				MGFLOG_ERROR("m_SO = NULL and getNumberOfSO() = " << getNumberOfSO())
			}
			return true;
		}

		case MGComponent_LOGGING_BOOL:
		{
			registerUsedCommand(MGComponent_LOGGING_BOOL);
			int logOn = toInt(cmdvec[1], s);
			if(logOn == MGF_TRUE)
			{
				enableLogging();
				MGFLOG_INFO("Logging enabled.");
			}
			else
			{
				MGFLOG_INFO("Logging disabled.");
				disableLogging();
			}
			return true;
		}

		case MGComponent_MINIMAP_BOOL:
		{
			registerUsedCommand(MGComponent_MINIMAP_BOOL);
			int miniOn = toInt(cmdvec[1], s);
			if(miniOn == MGF_TRUE)
			{
				enableMiniMap();
				MGFLOG_INFO("Mini map enabled.");
			}
			else
			{
				disableMiniMap();
				MGFLOG_INFO("Mini map disabled.");
			}
			return true;
		}

		case MGComponent_DYNAMICFPS_BOOL:
		{
			registerUsedCommand(MGComponent_DYNAMICFPS_BOOL);
			int dynOn = toInt(cmdvec[1], s);
			if(dynOn == MGF_TRUE)
			{
				setDynamicFPSEnabled(true);
				MGFLOG_INFO("Dynamic FPS enabled.");
			}
			else
			{
				setDynamicFPSEnabled(false);
				MGFLOG_INFO("Dynamic FPS disabled.");
			}
			return true;
		}

		case MGComponent_RUNFRAMES_INT:
		{
			registerUsedCommand(MGComponent_RUNFRAMES_INT);
			enableFrameCountdown();
			setFrameNumber(toInt(cmdvec[1], s));
			return false;
		}

		case MGComponent_EXIT:
		{
			registerUsedCommand(MGComponent_EXIT);
			std::cout << "Exiting console..." << std::endl;
			return false;
		}

		case MGComponent_EXIT_APPLICATION:
		{
			registerUsedCommand(MGComponent_EXIT_APPLICATION);
			std::cout << "Exiting application..." << std::endl;
			quit();
			return false;
		}

		case MGComponent_HELP:
		{
			registerUsedCommand(MGComponent_HELP);
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

			(void)m_Map.runConsoleCommand("map help", this, NULL);
			(void)m_Window.runConsoleCommand("window help", this, NULL);
			if(m_MO != NULL && getNumberOfMO() > 0)(void)m_MO[0].runConsoleCommand("mo 0 help", this, NULL);

			return true;
		}

		case MGComponent_INPUT_BOOL:
		{
			registerUsedCommand(MGComponent_INPUT_BOOL);
			int inputOn = toInt(cmdvec[1], s);
			if(inputOn == MGF_TRUE)
			{
				enableInput();
			}
			else
			{
				disableInput();
			}
			return true;
		}

		case MGComponent_EXPECT_INT_INT:
		{
			registerUsedCommand(MGComponent_EXPECT_INT_INT);
			int exp = toInt(cmdvec[2], s);
			int act = toInt(cmdvec[1], s);
			if(act == exp)
			{
				MGFLOG_INFO("MGFramework::detectComponentConsoleCommand found the expected value (" << exp << ")");
			}
			else
			{
				MGFLOG_ERROR("MGFramework::detectComponentConsoleCommand did not find the expected value (" << exp << " != " << act << ")");
			}
			return true;
		}

		case MGComponent_INT:
		{
			registerUsedCommand(MGComponent_INT);
			MGFLOG_INFO("" << toInt(cmdvec[0], s));
			return true;
		}

		case MGComponent_SYMBOL_ASSIGNTO_INT:
		{
			registerUsedCommand(MGComponent_SYMBOL_ASSIGNTO_INT);
			// First check symbol table of local variables..
			symbolAssignTo(cmdvec[0], cmdvec[2], s);
			/*
			if(s && s->hasValue(cmdvec[0]))
			{
				s->setValue(cmdvec[0], toInt(cmdvec[2], s));
			}
			else if(m_SymbolTable->hasValue(cmdvec[0]))
			{
				m_SymbolTable->setValue(cmdvec[0], toInt(cmdvec[2], s));
			}
			else if(s)
			{
				s->addSymbol(cmdvec[0], toInt(cmdvec[2], s));
			}
			else
			{
				m_SymbolTable->addSymbol(cmdvec[0], toInt(cmdvec[2], s));
			}

			if(s)
			{
				s->printTable();
			}
			m_SymbolTable->printTable();
			*/
			return true;
		}

		default:
			MGFLOG_ERROR("MGFramework::detectComponentConsoleCommand returned a bad value"); 
			return true;
	}

	std::cout << "Unknown command" << std::endl;
	return true;
}

void MGFramework::symbolAssignTo(string sym, string val, MGSymbolTable *s)
{
	if(s && s->hasValue(sym))
	{
		s->setValue(sym, toInt(val, s));
	}
	else if(m_SymbolTable->hasValue(sym))
	{
		m_SymbolTable->setValue(sym, toInt(val, s));
	}
	else if(s)
	{
		s->addSymbol(sym, toInt(val, s));
	}
	else
	{
		m_SymbolTable->addSymbol(sym, toInt(val, s));
	}

	if(s)
	{
		s->printTable();
	}
	m_SymbolTable->printTable();
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
		else if(cmdvec[0]=="runoneframe")
		{
			return MGComponent_RUNONEFRAME;
		}
		else
		{
			return MGComponent_INT;
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
		else if(cmdvec[0]=="logging")
		{
			return MGComponent_LOGGING_BOOL;
		}
		else if(cmdvec[0]=="minimap")
		{
			return MGComponent_MINIMAP_BOOL;
		}
		else if(cmdvec[0]=="dynamicfps")
		{
			return MGComponent_DYNAMICFPS_BOOL;
		}
		else if(cmdvec[0]=="runframes")
		{
			return MGComponent_RUNFRAMES_INT;
		}
		else if(cmdvec[0]=="exit" && cmdvec[1]=="application")
		{
			return MGComponent_EXIT_APPLICATION;
		}
		else if(cmdvec[0]=="input")
		{
			return MGComponent_INPUT_BOOL;
		}
	}
	else if(cmdvec.size() > 2)
	{
		if(cmdvec[0]=="create" && cmdvec[1]=="mo")
		{
			return MGComponent_CREATE_MO_INT_PARAMLIST; // Zero or more parameters..
		}
		if(cmdvec[0]=="create" && cmdvec[1]=="so")
		{
			return MGComponent_CREATE_SO_INT_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="add" && cmdvec[1]=="mo")
		{
			return MGComponent_ADD_MO_INT_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="add" && cmdvec[1]=="so")
		{
			return MGComponent_ADD_SO_INT_PARAMLIST; // Zero or more parameters..
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
		else if(cmdvec[0]=="delete" && cmdvec[1]=="all" && cmdvec[2]=="so" )
		{
			return MGComponent_DELETE_ALL_SO_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="delete" && cmdvec[1]=="all" && cmdvec[2]=="pe" )
		{
			return MGComponent_DELETE_ALL_PE_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0]=="expect")
		{
			return MGComponent_EXPECT_INT_INT;
		}
		else if(cmdvec[0]=="delete" && cmdvec[1]=="mo")
		{
			return MGComponent_DELETE_MO_INT;
		}
		else if(cmdvec[0]=="delete" && cmdvec[1]=="so")
		{
			return MGComponent_DELETE_SO_INT;
		}
		else if(cmdvec[0]=="so" && cmdvec[1]=="all")
		{
			return MGComponent_SO_ALL_X;
		}
		else if(cmdvec[0]=="so")
		{
			return MGComponent_SO_INT_X;
		}
		else if(cmdvec[1]=="=" || cmdvec[1]=="assignto")
		{
			return MGComponent_SYMBOL_ASSIGNTO_INT;
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
	/*
	// XXX: Will this give more accurate timing?
	if(m_ActualFrameTime>0) return (Uint32)(1000/m_ActualFrameTime);
	return getDesiredFPS();
	*/

	if(m_FrameTime > 0)
	{
		Uint32 result = (Uint32)(1000/m_FrameTime);
		if((result > 0) && (result < (2*getDesiredFPS())))
		{
			return result;
		}
		else
		{
			MGFLOG_WARNING("FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << "m_FrameTime:" << m_FrameTime << ", result:" << result);
			return getDesiredFPS();
		}
	}
	else
	{
		MGFLOG_WARNING("FPS incorrectly calculated (Uint32 MGFramework::getIntFPS()): " << m_FrameTime);
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
		MGFLOG_WARNING("FPS incorrectly calculated (void MGFramework::setDesiredFPS(Uint32 f)): " << f);
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
		MGFLOG_WARNING("FPS incorrectly calculated (Uint32 MGFramework::getDesiredFPS()): " << m_FPS << ", returning 1");
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
	if(n > m_Map.getWidth()*m_Map.getHeight())
	{
		MGFLOG_ERROR(	"MGFramework::createMO cannot create " << n << " MO on a " 
						<< m_Map.getWidth() << " by " << m_Map.getHeight() << " map")
		return;
	}

	// Since all MO are deleted we can unoccupy all their tiles..
	for(int i = 0; i < getNumberOfMO(); ++i)
	{
		m_Map.unOccupy(m_MO[i].getTileX(), m_MO[i].getTileY());
	}

	delete[] m_MO;
	m_NMO=n;
	if(getNumberOfMO() > 0)
	{
		m_MO = new MGMovingObject[getNumberOfMO()];
		for(int i=0; i<getNumberOfMO(); ++i)
		{
			m_MO[i].initialize();
		}
	}
	else
	{
		m_MO = NULL;
		m_MarkedMOs = 0;
	}
}

int MGFramework::addMO(int n)
{
	int tmpTotal = n + getNumberOfMO();
	if(tmpTotal > m_Map.getWidth()*m_Map.getHeight())
	{
		MGFLOG_ERROR(	"MGFramework::addMO cannot create " << tmpTotal << " MO on a " 
						<< m_Map.getWidth() << " by " << m_Map.getHeight() << " map")
		return -1;
	}

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
	for(int i=nOld; i<getNumberOfMO(); ++i)
	{
		m_MO[i].initialize();
	}

	return nOld; // Return index of first MO added.
}

void MGFramework::deleteMO(int index)
{
	if(index < 0 || index >= getNumberOfMO())
	{
		MGFLOG_ERROR("MGFramework::deleteMO was given a bad index: " << index)
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
			}
			else
			{
				MGFLOG_ERROR("MGFramework::deleteMO was not able to find the given index: " << index)
				return;
			}
		}
		m_NMO = getNumberOfMO()-1;
	}
}



bool MGFramework::setupMO(int i, int x, int y, unsigned int owner, int speed)
{
	if(i < 0 || i >= getNumberOfMO())
	{
		MGFLOG_ERROR("MGFramework::setupMO was given a bad index: " << i)
	}
	else
	{
		if(x<0) x = randomN(m_Map.getWidth());
		if(y<0) y = randomN(m_Map.getHeight());
		bool successful=false;

		for(int q=0; q<MGF_MOPOSITIONINGATTEMPTS; ++q)
		{
			if(m_Map.occupant(x,y) != 0)
			{
				x = randomN(m_Map.getWidth());
				y = randomN(m_Map.getHeight());
			}
			else
			{
				successful=true;
				break;
			}
		}
		// Find the first available (x,y) since the random placement failed.
		if(!successful)
		{
			for(int t=0; t < m_Map.getWidth()*m_Map.getHeight(); ++t)
			{
				if(m_Map.occupant(m_Map.getTileX(t), m_Map.getTileY(t))==0)
				{
					x = m_Map.getTileX(t);
					y = m_Map.getTileY(t);
					successful = true;
					break;
				}
			}
		}

		if(successful)
		{
			m_MO[i].setTileXY(x, y, this);
			//m_MO[i].setNextXY(x, y, this);
			m_MO[i].setDestTileXY(m_MO[i].getTileX(), m_MO[i].getTileY());
			m_MO[i].setSpeed(1.0/(double)speed, m_Map.getTileHeight()); // speed = 2 means 2 tiles per second
			m_MO[i].setOwner(owner);
			m_Map.occupy(m_MO[i].getTileX(), m_MO[i].getTileY(), m_MO[i].getID());
		}
		else
		{
			MGFLOG_ERROR("Failed to find space for MO at creation");
			deleteMO(i);
			return false;
		}

	}
	return true;
}




bool MGFramework::setupSO(int i, int x, int y)
{
	if(i < 0 || i >= getNumberOfSO())
	{
		MGFLOG_ERROR("MGFramework::setupSO was given a bad index: " << i)
	}
	else
	{
		if(x<0) x = randomN(m_Map.getWidth());
		if(y<0) y = randomN(m_Map.getHeight());
		bool successful=false;

		for(int q=0; q<MGF_SOPOSITIONINGATTEMPTS; ++q)
		{
			if(m_Map.occupant(x,y) != 0)
			{
				x = randomN(m_Map.getWidth());
				y = randomN(m_Map.getHeight());
			}
			else
			{
				successful=true;
				break;
			}
		}
		// Find the first available (x,y) since the random placement failed.
		if(!successful)
		{
			for(int t=0; t < m_Map.getWidth()*m_Map.getHeight(); ++t)
			{
				if(m_Map.occupant(m_Map.getTileX(t), m_Map.getTileY(t))==0)
				{
					x = m_Map.getTileX(t);
					y = m_Map.getTileY(t);
					successful = true;
					break;
				}
			}
		}

		if(successful)
		{
			m_SO[i].setTileXY(x, y, this);
			m_Map.occupy(m_SO[i].getTileX(), m_SO[i].getTileY(), m_SO[i].getID());
		}
		else
		{
			MGFLOG_ERROR("Failed to find space for SO at creation");
			deleteSO(i);
			return false;
		}

	}
	return true;
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
		MGFLOG_ERROR("MGFramework::deletePE was given a bad index: " << index)
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
				// Overwrite pe(i) with pe(i+1)
				m_PE[i].copy(&m_PE[i+1]);
			}
			else if(i==getNumberOfPE()-1)
			{
				//No need to actually delete the PE since we will not access it if it's outside getNumberOfPE()...
			}
			else
			{
				MGFLOG_ERROR("MGFramework::deletePE was not able to find the given index: " << index)
				return;
			}
		}
		m_NPE = getNumberOfPE()-1;
	}
}


int MGFramework::getNumberOfUsedCommands()
{
	int n=0;
	for(int i=MGComponent_UNDEFINED; i<MGComponent_NUMBEROFCOMMANDIDENTIFIERS; ++i)
	{
		if(m_UsedCommands[i])
		{
			n++;
		}
		MGFLOG_INFO("" << i << ": " << toString(m_UsedCommands[i]).c_str());
	}
	return n;
}



void MGFramework::quit()
{ 
	m_Quit = true; 
	m_Map.printStatisticsCounters();
	std::cout << "Execution time: " << MGF_GetExecTimeMS() << std::endl;
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

#ifndef MGF_DEBUGGING_ENABLED
void MGFramework::drawText(SDL_Surface* screen, const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB)
{
	/*TTF_Font* */m_Font = TTF_OpenFont("ARIAL.TTF", size);
	SDL_Color foregroundColor = { fR, fG, fB };
	SDL_Color backgroundColor = { bR, bG, bB };
	SDL_Surface* textSurface = TTF_RenderText_Shaded(m_Font, string, foregroundColor, backgroundColor);
	SDL_Rect textLocation = { x, y, 0, 0 };
	SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(m_Font);
}
#endif


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


bool MGFramework::isNumericalInt(const string &s)
{
	if(s.size()==0)
	{
		return false;
	}
	else if(s.size()==1)
	{
		return s[0]>='0' && s[0]<='9';
	}
	else
	{
		for(unsigned int i=1;i<s.size(); ++i)
		{
			if(s[i]<'0' || s[i]>'9')
			{
				return false;
			}
		}
		if(s[0]<'0' || s[0]>'9')
		{
			if(s[0]!='-')
			{
				return false;
			}
		}
		return true;
	}
	MGFLOG_STATIC_ERROR("MGFramework::isNumericalInt failed to parse string");
}


int MGFramework::staticToInt(const string &s)
{
	if(isNumericalInt(s))
	{
		std::istringstream buffer(s);
		int value;
		buffer >> value;
		return value;
	}
	MGFLOG_STATIC_ERROR("MGFramework::toInt failed to convert string to integer: " << s);
	return 0;
}

int MGFramework::toInt(const string &s, MGSymbolTable *sym)
{
	if(isNumericalInt(s))
	{
		std::istringstream buffer(s);
		int value;
		buffer >> value;
		return value;
	}
	else
	{
		if(s == string("on"))
		{
			return MGF_TRUE;
		}
		else if(s == string("off"))
		{
			return MGF_FALSE;
		}
		else if(s == string("random_mo"))
		{
			if(getNumberOfMO()==0)
			{
				MGFLOG_ERROR("MGFramework::toInt was called with 'random_mo' when no MO existed");
			}
			else
			{
				return randomN(getNumberOfMO());
			}
		}
		else if(s == string("random_pe"))
		{
			if(getNumberOfPE()==0)
			{
				MGFLOG_ERROR("MGFramework::toInt was called with 'random_pe' when no PE existed");
			}
			else
			{
				return randomN(getNumberOfPE());
			}
		}
		else if(s == string("random_x"))
		{
			return randomN(m_Map.getWidth());
		}
		else if(s == string("random_y"))
		{
			return randomN(m_Map.getHeight());
		}
		else if(s == string("getnumberofmarkedmo"))
		{
			return getNumberOfMarkedMO();
		}
		else if(s == string("getnumberofmo"))
		{
			return getNumberOfMO();
		}
		else if(s == string("getnumberofso"))
		{
			return getNumberOfSO();
		}
		else if(s == string("getnumberofpe"))
		{
			return getNumberOfPE();
		}
		else if(s == string("getnumberofcommands"))
		{
			return getNumberOfCommands();
		}
		else if(s == string("getnumberofusedcommands"))
		{
			return getNumberOfUsedCommands();
		}
		else if(s == string("getfps"))
		{
			return getFPS();
		}
		else
		{
			// First look in the symbol table of local variables..
			if(sym)
			{
				for (std::deque<MGSymbolTable::MGSymbolTablePair>::iterator it=sym->table.begin(); it != sym->table.end(); ++it)
				{
					if((*it).symbol == s)
					{
						return (*it).value;
					}
				}				
			}
			// Then the one of global..
			for (std::deque<MGSymbolTable::MGSymbolTablePair>::iterator it=m_SymbolTable->table.begin(); it != m_SymbolTable->table.end(); ++it)
			{
				if((*it).symbol == s)
				{
					return (*it).value;
				}
			}
		}
	}
	MGFLOG_ERROR("MGFramework::toInt failed to convert string to integer: " << s);
	return 0;
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


bool MGFramework::okMGFrameworkSyntax(const std::vector<std::string> &v_s)
{
	if(v_s.size()>0 && v_s[v_s.size()-1]!=string(""))
	{
		return true;
	}
	return false;
}


void MGFramework::addConsoleCommandToQueue(const char *c)
{

}


void MGFramework::createSO(int n)
{
	delete[] m_SO;
	m_NSO=n;
	if(getNumberOfSO() > 0)
	{
		m_SO = new MGStationaryObject[getNumberOfSO()];
	}
	else
	{
		m_SO = NULL;
	}
}

void MGFramework::addSO(int n)
{
	MGStationaryObject *oldSO = new MGStationaryObject[getNumberOfSO()];
	int nOld=getNumberOfSO();
	for(int i=0; i<nOld; i++)
	{
		oldSO[i].copy(&m_SO[i]);
	}
	delete[] m_SO;
	m_NSO=nOld+n;
	m_SO = new MGStationaryObject[getNumberOfSO()];
	for(int i=0; i<nOld; i++)
	{
		m_SO[i].copy(&oldSO[i]);
	}
}

void MGFramework::deleteSO(int index)
{
	if(index < 0 || index >= getNumberOfSO())
	{
		MGFLOG_ERROR("MGFramework::deleteSO was given a bad index: " << index)
	}
	else
	{
		m_Map.unOccupy(m_SO[index].getTileX(), m_SO[index].getTileY());
		for(int i=0; i<getNumberOfSO(); ++i)
		{
			if(i<index)
			{
				// Do nothing...
			}
			else if(i>=index && i<getNumberOfSO()-1)
			{
				// Overwrite so(i) with so(i+1)
				m_SO[i].copy(&m_SO[i+1]);
			}
			else if(i==getNumberOfSO()-1)
			{
				//No need to actually delete the SO since we will not access it if it's outside getNumberOfSO()...
			}
			else
			{
				MGFLOG_ERROR("MGFramework::deleteSO was not able to find the given index: " << index)
				return;
			}
		}
		m_NSO = getNumberOfSO()-1;
	}
}


#ifndef MGF_DEBUGGING_ENABLED
int runMGFrameworkSocketTerminal(void *fm)
{
	MGFramework *mgf = (MGFramework *)fm;
	int PORTNR = mgf->getPort();
	mgf->logIfEnabled((std::string("Opening socket terminal... port ") + MGFramework::toString(mgf->getPort())).c_str());
	//mgf->logIfEnabled((MGFramework::toString(mgf->getPort())).c_str());
	
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
				mgf->runConsoleCommand(buf, mgf, NULL);
				if(send(fd_new, "ok\n\r", 4, 0) == SOCKET_ERROR)
				{
					mgf->logIfEnabled("Server: Failed sending an answer to client request");
					return 1;
				}
			}
			if(nZerosInARow>4)
			{
				connectionOpen=false;
			}
		}
		WSACleanup();
	}
	mgf->logIfEnabled("Socket terminal closed...");
	return 0;
}
#endif