#include "mgframework.h"
#include <iostream>
#include <algorithm> 
#include <cmath>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "mgpathitem.h"

#ifndef UNITTEST_LINUX
#include <winsock.h>
#endif

//Windows macros overriding std if not undefined
#undef min
#undef max


MGFramework::MGFramework():
	m_InputEnabled(true),
	m_Quit(false),
	m_TypingEnabled(true),
	m_WindowPropertiesSet(false),
	m_MiniMapEnabled(true),
	m_MGFInstanceType(MGFSINGLEPLAYERINSTANCE),
	m_PlayerNumber(MGF_NOPLAYER),
	m_FrameTime(16),
	m_ActualFrameTime(16),
	m_FPS(60),
	m_DelayTime(0),
	m_DynamicFPSEnabled(true),
	m_FeatureMouseScrollingEnabled(true),
	m_FrameCountdownEnabled(false),
	m_FrameNumber(0),
	m_MarkedMOs(0),
	m_NPE(0),
	m_NSO(0),
	m_KeepSocketTerminalOpen(false),
	m_Port(0),
	m_FramingOngoing(false),
	m_XFrameStart(0),
	m_YFrameStart(0),
	m_XFrameEnd(0),
	m_YFrameEnd(0),
	m_OnlySelectOwnedMO(false),
	m_CommandReturnVal(0),
	m_SelectiveTileRendering(false),
	m_RenderAll(true),
	m_NDrawnTiles(0),
	m_FeatureCenterOnMO(-1),
	m_PE(NULL),
	m_SO(NULL),
	m_SymbolTable(NULL),
	m_SymbolTableTransfer(NULL)
{
	setDesiredFPS(20);
	std::srand((int)std::time(0));

	m_SymbolTable = new MGSymbolTable();
	m_SymbolTableTransfer = new MGSymbolTable();
}

MGFramework::~MGFramework()
{
	if(m_PE) delete[] m_PE;
	m_PE = NULL;
	if(m_SO) delete[] m_SO;
	m_SO = NULL;
	if(m_SymbolTable) delete m_SymbolTable;
	m_SymbolTable = NULL;
	if(m_SymbolTableTransfer) delete m_SymbolTableTransfer;
	m_SymbolTableTransfer = NULL;
}


bool MGFramework::processEvents()
{
#ifndef UNITTEST_LINUX
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
			case SDL_QUIT:
			{
				MGFLOG_INFO("SDL_QUIT")
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
						for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
						{
							if(it->isMarked())
							{
								it->unMark();
								countUnMark();
							}
						}

						activateFraming(event.button.x, event.button.y);
						MGFLOG_INFO("Map (left click): index = " << iClick << ", x = " << xClick << ", y = " << yClick)
					}
					else if (((int) event.button.button) == 3)
					{
						// Right button
						MGFLOG_INFO("Map (right click): index = " << iClick << ", x = " << xClick << ", y = " << yClick)
						if(featureMouseScrollingEnabled())
						{
							m_Map.mouseScrollingClick(event.button.x, event.button.y);
						}
						else if(featureCenterOnMO() && (getNumberOfMO() > centerMOIndex()) )
						{
							std::list<MGMovingObject>::iterator it = m_MO.begin();
							for(unsigned int i = 0; i < centerMOIndex(); i++)
							{
								it++;
							}
							// TODO: Invent a way so that path finding algorithm can be configured also for center on MO feature
							it->setPath(m_Map.calculatePath(MGFBASICPATH1, it->getTileX(), it->getTileY(), xClick, yClick));
						}
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
					for(int x=std::min(startClickX, endClickX); x<=std::max(startClickX, endClickX); x++)
					{
						for(int y=std::min(startClickY, endClickY); y<=std::max(startClickY, endClickY); y++)
						{
							for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
							{
								if(it->getTileX() == x && it->getTileY() == y)
								{
									if(!it->isMarked())
									{
										if(featureOnlySelectOwnedMOEnabled())
										{
											if(it->getOwner() == getClientPlayer())
											{
												it->mark();
												countMark();
											}
										}
										else
										{
											it->mark();
											countMark();
										}
									}
								}
							}
						}
					}
					deactivateFraming();
				}
				else if (((int) event.button.button) == 3)
				{
					// Order marked MOs to go to mouse location
					if(getNumberOfMarkedMO() > 0)
					{
						int iClick = m_Map.getTileIndex(event.button.x, event.button.y);
						if(iClick >= 0)
						{
							int xClick = m_Map.getTileX(iClick);
							int yClick = m_Map.getTileY(iClick);
							char c[64];
							// TODO: Make it possible to configure path finding algorithm to be used in the framework as well
							sprintf(c, "mo marked setdestination %d %d -basicpath", xClick, yClick);
							runConsoleCommand(c, this, NULL);
						}
					}
					if(featureMouseScrollingEnabled())
					{
						m_Map.mouseScrollingRelease(event.button.x, event.button.y);
					}
				}
				break;
			}

			case SDL_MOUSEMOTION:
			{
				if(m_Map.mouseScrollingUpdate(event.motion.x, event.motion.y))
				{
					// Set flag to trigger complete re-render
					setRenderAllTiles();
				}
				if(isFramingOngoing())
				{
					updateFraming(event.motion.x, event.motion.y);
				}
				break;
			}

			// Default case for unknown events
			default:
			{
				break;
			}
		}
	}
#endif
	return true;
}

int MGFramework::parse(const char *sFileName)
{
	int returnValue = 0;
	if(sFileName == NULL)
	{
		return returnValue;
	}

	MGFLOG_INFO("MGFramework::parse was called with argument " << sFileName);
	char *functionName = NULL;
	char scriptFileName[128];
	bool foundFunction = false;
	bool globalScope = false;
	int lineNumber = 0;
	int skipToEndIf = 0;
	int insideIf = 0;
	int insideElse = 0;
	MGSymbolTable *symbols = new MGSymbolTable();

	// Copy all symbols in transfer table to local symbol table (symbols), then delete them in the transfer table.
	for (std::deque<MGSymbolTable::MGSymbolTablePair>::iterator it = m_SymbolTableTransfer->table.begin(); it != m_SymbolTableTransfer->table.end(); ++it)
	{
		symbols->addSymbol(it->symbol, it->value);
	}
	m_SymbolTableTransfer->clear();

	strcpy(scriptFileName, sFileName);

	std::size_t foundColon = std::string(scriptFileName).find(std::string(":"));
	if (foundColon != std::string::npos && foundColon > 0)
	{
		functionName = scriptFileName + foundColon + 1;
		scriptFileName[foundColon] = '\0';
		MGFLOG_INFO("MGFramework::parse will parse file " << scriptFileName << ", function " << functionName);
		globalScope = false;
	}
	else
	{
		globalScope = true;
	}

	FILE *sf = fopen(scriptFileName, "rt");
	if(sf == NULL)
	{
		MGFLOG_ERROR("MGFramework::parse failed to open script file " << scriptFileName);
	}
	else
	{
		char scriptLine[MGF_SCRIPTLINE_MAXLENGTH] = "";
		char *neof = NULL;
		MGFLOG_INFO("MGFramework::parse starting to parse script file " << scriptFileName);
		if(functionName != NULL)
		{
			MGFLOG_INFO("MGFramework::parse starting to execute function " << functionName);
		}
		else
		{
			foundFunction = true;
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
				if(!foundFunction)
				{
					MGFLOG_ERROR("MGFramework::parse found unexpected end of file");
				}
				else
				{
					MGFLOG_INFO("MGFramework::parse detected end of file");
				}
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

				// Create vector of strings (tokens) representing the current script line
				std::string s_scriptLine(scriptLine);
				std::vector<std::string> v_scriptLine = MGComponent::symbols(scriptLine);

				if(foundFunction)
				{
					if(v_scriptLine.size() == 1 && v_scriptLine[0] == "end")
					{
						if(insideIf || insideElse)
						{
							// Print an error and then carry on with ending the function..
							MGFLOG_ERROR("MGFramework::parse found unexpected 'end'");
							break;
						}
						else if(functionName != NULL)
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
						if(functionName != NULL)
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
					else if(v_scriptLine.size() == 4 && v_scriptLine[0] == "if")
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
							break;
						}
					}
					else if(v_scriptLine.size() == 1 && v_scriptLine[0] == "endif")
					{
						if(skipToEndIf == 0 && insideIf == 0 && insideElse == 0)
						{
							MGFLOG_ERROR("Unexpected 'endif'");
							break;
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
					else if(skipToEndIf == 0 && v_scriptLine.size() == 2 && v_scriptLine[0] == "print")
					{
						std::cout << v_scriptLine[1].c_str() << std::endl;
					}
					else if(skipToEndIf == 0 && okMGFrameworkSyntax(v_scriptLine))
					{
						// function call..
						//MGFLOG_INFO("Tokens: " << v_scriptLine.size());
						if(v_scriptLine.size() > 1 && v_scriptLine[0]=="call")
						{
							// call filename:func -param paramvalue ... 
							std::size_t fColon = v_scriptLine[1].find(std::string(":"));

							// Loop through any parameters and save them to symbols?
							for(unsigned int i = 2; i < v_scriptLine.size(); ++i)
							{
								if(i+1 >= v_scriptLine.size())
								{
									// ERROR
									MGFLOG_ERROR("MGFramework::parse found no function parameter value");
								}
								else
								{
									if(v_scriptLine[i][0] != '-')
									{
										// ERROR
										MGFLOG_ERROR("MGFramework::parse found function parameter with no '-'");
									}
									else
									{
										std::string parameter("");
										char c[2] = {0, 0};
										for(unsigned int n = 1; n < strlen(v_scriptLine[i].c_str()); ++n)
										{
											c[0] = v_scriptLine[i][n];
											parameter += std::string(c);
										}
										if(parameter.length() < 1)
										{
											MGFLOG_ERROR("MGFramework::parse found bad function parameter name");
										}
										else
										{
											m_SymbolTableTransfer->addSymbol(parameter, toInt(v_scriptLine[i + 1], symbols));
											i++;
										}
									}
								}
							}

							if (fColon!=std::string::npos)
							{
								MGFLOG_INFO("MGFramework::parse calling " << v_scriptLine[1].c_str());
								symbolAssignTo(	v_scriptLine[1], 
												MGComponent::toString(parse(v_scriptLine[1].c_str())), 
												symbols);
							}
							else
							{
								MGFLOG_INFO("MGFramework::parse calling " << (std::string(scriptFileName) + std::string(":") + v_scriptLine[1]).c_str());
								symbolAssignTo(	v_scriptLine[1], 
												MGComponent::toString(parse((std::string(scriptFileName) + std::string(":") + v_scriptLine[1]).c_str())), 
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
							std::string cmd = std::string("");
							for(unsigned int i = 0; i < v_scriptLine.size(); ++i)
							{
								if(v_scriptLine[i] != std::string(""))
								{
									cmd += v_scriptLine[i];
									if(i < v_scriptLine.size() - 1)
									{
										cmd += std::string(" ");
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
				else if(functionName != NULL && v_scriptLine.size() == 2 && v_scriptLine[0] == "function" && v_scriptLine[1] == std::string(functionName))
				{
					foundFunction = true;
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




void MGFramework::run(const char *scriptFileName, bool runOneFrame)
{
	(void)parse(scriptFileName); // Return value not needed when called from run(...)

	unsigned int frameStartTime = 0; 
	if(!runOneFrame) m_DelayTime = 0;
	// Assume an initial value of m_FrameTime of 1000/getDesiredFPS().
	unsigned int lastFrameTime = MGF_GetExecTimeMS() - (1000 / getDesiredFPS()); // MGF_GetExecTimeMS() - lastFrameTime cannot be zero.

	// Frame loop (processEvents, handleGameLogics, draw)
	while(processEvents())
	{
		// Calculate the current frame time (and implicitly FPS)..
		m_FrameTime = MGF_GetExecTimeMS() - lastFrameTime; //number of ms from last frame was handled
		lastFrameTime = MGF_GetExecTimeMS();
		frameStartTime = MGF_GetExecTimeMS();

		// Handle all calculations and draw current frame..


		//If frame countdown is enabled (a.k.a. we are counting down frames to go into console again)
		//and the counter is zero, activate the console.
		if(frameCountdownEnabled())
		{
			static int nFrames = 0;
			if(getFrameNumber() == 0)
			{
				std::cout << "Frame countdown results: " << std::endl;
				std::cout << "\tNumber of frames executed: " << nFrames << std::endl;
				nFrames = 0;
				if(!runOneFrame)activateConsole();
			}
			else if (getFrameNumber() > 0)
			{
				countdownFrame(1);
				nFrames ++;
			}
			else
			{
				MGFLOG_ERROR("getFrameNumber() returned a negative value");
			}
		}

		// General game logics..
		handleMGFGameLogics();

		// Application specific game logics..
		handleGameLogics();

		// A server instance of the framework has no graphics.
		if(getInstanceType() != MGFSERVERINSTANCE) 
		{
			draw();
			getWindow()->flipSurface();
		}

		// Sleep if there is time to spare..
		m_DelayTime = (1000 / getDesiredFPS()) - (MGF_GetExecTimeMS() - frameStartTime);
		if(m_DelayTime > 0)
		{
			//TODO: Change to SDL sleep method but move it into MGWindow class to collect all SDL dependencies there.
#ifndef UNITTEST_LINUX
			Sleep((DWORD)m_DelayTime);
#endif
		}
		m_ActualFrameTime = MGF_GetExecTimeMS() - frameStartTime;

		// All tiles have been rendered at least once - start selective tile rendering
		unsetRenderAllTiles();

		// Mainly used for testing, only execute one frame if instructed
		if(runOneFrame)
		{
			break;
		}
	}
}


void MGFramework::handleMGFGameLogics()
{
	// Handle all events in the event queue
	// Not implemented yet

	// Update periodic event to trigger rare events
	for(int i = 0; i < getNumberOfPE(); i++)
	{
		if(m_PE[i].update())
		{
			MGFLOG_INFO("PE triggered an update");
			m_PE[i].runFile1(this);
		}
	}

	// Update all moving objects
	for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
	{
		it->update(this);
	}

	// Example of how FPS can be controlled dynamically
	if(featureDynamicFPSEnabled())
	{
		if(getLastFrameDelayTime() > 10)
		{
			setDesiredFPS(getFPS() + 1);
		}
		else if(getLastFrameDelayTime() < 7)
		{
			setDesiredFPS(std::max(1, (int)(getFPS() - 1)));
		}
	}

	resetDrawnTilesCounter();
}


void MGFramework::activateConsole()
{
	disableFrameCountdown();
	enableTyping();
	std::string cLine;
	do
	{
		std::cout << "mgf> ";
		std::getline(std::cin, cLine);
	}
	while(runConsoleCommand(cLine.c_str(), this, NULL));
	disableTyping();
}



void MGFramework::symbolAssignTo(std::string sym, std::string val, MGSymbolTable *s)
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
}

void MGFramework::symbolAssignTo(const std::vector<std::string> &cmdvec, MGSymbolTable *s)
{
	if(cmdvec.size() < 3)
	{
		MGFLOG_ERROR("MGFramework::symbolAssignTo too few tokens");
	}
	else if(cmdvec.size() == 3)
	{
		symbolAssignTo(cmdvec[0], cmdvec[2], s);
	}
	else
	{
		std::string str;
		str.clear();
		for(unsigned int i = 2; i < cmdvec.size(); ++i)
		{
			str += cmdvec[i];
		}
		MGFLOG_ERROR("MGFramework::symbolAssignTo non-supported right-hand-side (" << cmdvec.size() - 2 << "): " << str);
	}
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
			MGFLOG_ERROR("MGFramework::runConsoleCommand received MGComponent_UNDEFINED"); 
			break;
		}

		case MGComponent_RUNONEFRAME:
		{
			run(NULL, true);
			return true;
		}

		case MGComponent_MAP_X:
		{
			return m_Map.runConsoleCommand(c, this, s);
		}


		case MGComponent_WINDOW_FULLSCREEN_BOOL:
		{
			int onOrOff = toInt(cmdvec[2], s);
			if(onOrOff == MGF_FALSE)
			{
				getWindow()->deactivateFullscreen();
				return true;
			}
			else
			{
				getWindow()->activateFullscreen();
				return false; // Deactivate console
			}
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
			for(int i = 0; i < getNumberOfPE(); i++)
			{
				m_PE[i].runConsoleCommand(c, this, s);
			}
			return true;
		}

		case MGComponent_SO_INT_X:
		{
			int soIndex = toInt(cmdvec[1], s);
			if(soIndex >= 0 && soIndex < getNumberOfSO())
			{
				return m_SO[toInt(cmdvec[1], s)].runConsoleCommand(c, this, s);
			}
			MGFLOG_WARNING("Console command was not forwarded to SO " << soIndex); 
			return true;
		}

		case MGComponent_SO_ALL_X:
		{
			for(int i = 0; i < getNumberOfSO(); i++)
			{
				m_SO[i].runConsoleCommand(c, this, s);
			}
			return true;
		}

		case MGComponent_SETFPS_INT:
		{
			setDesiredFPS(toInt(cmdvec[1], s));
			return true;
		}

		case MGComponent_DELETE_ALL_MO_PARAMLIST:
		{
			unsigned int owner = 0;
			bool ownerParamSet=false;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i] == "-owner" && cmdvec.size() > (i + 1))
				{
					owner = (unsigned int)toInt(cmdvec[i + 1], s);
					ownerParamSet = true;
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
				for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); )
				{
					MGFLOG_INFO("MO owned by " << it->getOwner());
					if(it->getOwner() == owner)
					{
						MGFLOG_INFO("Deleting one out of " << m_MO.size() << " MOs");
						if(it == m_MO.begin())
						{
							deleteMO(it);
							it = m_MO.begin();
						}
						else
						{
							deleteMO(it);
							it--;
						}
					}
					else
					{
						it++;
					}
				}
			}
			else
			{
				//Deleting ALL MOs is faster than deleting them per owner 
				MGFLOG_INFO("Creating zero MO...");
				deleteAllMO();
			}
			return true;
		}



		case MGComponent_DELETE_ALL_PE_PARAMLIST:
		{
			int owner = 0;
			bool ownerParamSet = false;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i] == "-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i + 1], s);
					ownerParamSet = true;
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
				for(int i = getNumberOfPE() - 1; i >= 0; --i)
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
				deleteAllPE();
			}
			return true;
		}



		case MGComponent_DELETE_ALL_SO_PARAMLIST:
		{
			int owner = 0;
			bool ownerParamSet = false;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i] == "-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i + 1], s);
					ownerParamSet = true;
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
				for(int i = getNumberOfSO() - 1; i >= 0; --i)
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
				deleteAllSO();
			}
			return true;
		}


		case MGComponent_ADD_MO_INT_PARAMLIST:
		{
			int nBefore=getNumberOfMO();
			int n = toInt(cmdvec[2], s);
			int owner = 0;
			int x = -1; // Invalid default value
			int y = -1; // Invalid default value
			int speed = 2; // Tiles per second.
			int x1 = 0;
			int y1 = 0;
			int x2 = m_Map.getWidth();
			int y2 = m_Map.getHeight();

			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i] == "-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i + 1], s);
					++i;
				}
				else if(cmdvec[i] == "-x" && cmdvec.size() > (i + 1))
				{
					x = toInt(cmdvec[i + 1], s);
					++i;
					if(n != 1)
					{
						MGFLOG_ERROR("Parameter -x can only be set when adding one MO");
						n = 0; // Abort MO creation..
					}
					
				}
				else if(cmdvec[i] == "-y" && cmdvec.size() > (i + 1))
				{
					y = toInt(cmdvec[i + 1], s);
					++i;
					if(n != 1)
					{
						MGFLOG_ERROR("Parameter -y can only be set when adding one MO");
						n = 0; // Abort MO creation..
					}
					
				}
				else if(cmdvec[i] == "-speed" && cmdvec.size() > (i + 1))
				{
					speed = toInt(cmdvec[i + 1], s);
					++i;
				}
				else if(cmdvec[i] == "-area_square" && cmdvec.size() > (i + 4))
				{
					x1 = toInt(cmdvec[i + 1], s);
					y1 = toInt(cmdvec[i + 2], s);
					x2 = toInt(cmdvec[i + 3], s);
					y2 = toInt(cmdvec[i + 4], s);
					i += 4;
				}
				else
				{
					MGFLOG_ERROR("Error in command (add mo <n>), bad parameter list");
					n = 0; // Abort MO creation..
				}
			}

			if(n > 0)
			{
				MGFLOG_INFO("Adding MOs: " << n << ", nBefore = " << nBefore);
				addMO(n);
			}
			else
			{
				MGFLOG_ERROR("Error in command (add mo <n>)");
				return true;
			}

			// Loop from first new MO to the end.
			std::list<MGMovingObject>::iterator it = m_MO.begin();
			for(int i = 0; i < nBefore; i++)
			{
				it++;
			}
			for(; it != m_MO.end(); )
			{
				// If setup returns true we step iterator, otherwise erase has already stepped it
				MGFLOG_INFO("SetupMO, owner: " << owner);
				if(setupMO(it, x, y, owner, speed, x1, y1, x2, y2))
				{
					it++;
				}
			}
			return true;
		}



		case MGComponent_ADD_SO_INT_PARAMLIST:
		{
			int nBefore=getNumberOfSO();
			int n = toInt(cmdvec[2], s);
			int x = -1; // Invalid default value
			int y = -1; // Invalid default value
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i] == "-x" && cmdvec.size() > (i + 1))
				{
					x = toInt(cmdvec[i + 1], s);
					++i;
					if(n != 1)
					{
						MGFLOG_ERROR("Parameter -x can only be set when adding one SO");
						n = 0; // Abort SO creation..
					}
					
				}
				else if(cmdvec[i] == "-y" && cmdvec.size() > (i + 1))
				{
					y = toInt(cmdvec[i + 1], s);
					++i;
					if(n != 1)
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
			if(n > 0)
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
				for(int i = nBefore; i < getNumberOfSO(); i++)
				{
					// If setup fails we must setup the same index again 
					// since the failing SO has been deleted.
					if(!setupSO(i, x, y)) --i;
				}
			}
			else
			{
				MGFLOG_ERROR("SO storage is undefined");
			}
			return true;
		}


		case MGComponent_ADD_PE_INT_PARAMLIST:
		{
			int nBefore = getNumberOfPE();
			int n = toInt(cmdvec[2], s);
			int owner = 0;
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i] == "-owner" && cmdvec.size() > (i + 1))
				{
					owner = toInt(cmdvec[i + 1], s);
					++i;
				}
				else
				{
					MGFLOG_ERROR("Error in command (add pe <n>), bad parameter list");
					n = 0; // Abort PE creation..
				}
			}
			if(n > 0)
			{
				addPE(n);
			}
			else
			{
				MGFLOG_ERROR("Error in command (add pe <n>)");
				return true;
			}
			for(int i = nBefore; i < getNumberOfPE(); i++)
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


		case MGComponent_OPEN_TERMINALSERVER:
		{
			MGFLOG_INFO("Opening terminal server...");
#ifndef MGF_DISABLE_WINSOCK
			openSocketTerminal();
			m_SocketTerminal = SDL_CreateThread(runMGFrameworkSocketTerminal, this);
#endif
			return true;
		}

		case MGComponent_CLOSE_TERMINALSERVER:
		{
			MGFLOG_INFO("Closing terminal server...");
#ifndef MGF_DISABLE_WINSOCK
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
			unsigned int moIndex = (unsigned int)toInt(cmdvec[1], s);
			if(moIndex >= 0 && moIndex < getNumberOfMO())
			{
				int i = 0;
				for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
				{
					if(i == toInt(cmdvec[1], s))
					{
						return it->runConsoleCommand(c, this, s);
					}
					else
					{
						i++;
					}
				}
			}
			MGFLOG_WARNING("Console command was not forwarded to MO " << moIndex); 
			return true;
		}

		case MGComponent_MO_MARKED_X:
		{
			for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
			{
				if(it->isMarked())
				{
					it->runConsoleCommand(c, this, s);
				}
			}
			return true;
		}

		case MGComponent_MO_ALL_X:
		{
			for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
			{
				it->runConsoleCommand(c, this, s);
			}
			return true;
		}

		case MGComponent_DELETE_MO_INT:
		{
			unsigned int moIndex = (unsigned int)toInt(cmdvec[2], s);
			if(moIndex < getNumberOfMO())
			{
				std::list<MGMovingObject>::iterator it = m_MO.begin();
				for(unsigned int i = 0; i < moIndex; i++)
				{
					it++;
				}
				deleteMO(it);
			}
			return true;
		}

		case MGComponent_DELETE_SO_INT:
		{
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
			int miniOn = toInt(cmdvec[1], s);
			if(miniOn == MGF_TRUE)
			{
				enableFeatureMiniMap();
				MGFLOG_INFO("Mini map enabled.");
			}
			else
			{
				disableFeatureMiniMap();
				MGFLOG_INFO("Mini map disabled.");
			}
			return true;
		}

		case MGComponent_DYNAMICFPS_BOOL:
		{
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
			enableFrameCountdown();
			setFrameNumber(toInt(cmdvec[1], s));
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
			std::cout << "add <mo|pe> <n> - Adds <n> objects. <n> is an integer. Only MGMovingObject" << std::endl;
			std::cout << "          (mo) and MGPeriodicEvent (pe) are supported at this point."  << std::endl;
			std::cout << "open <terminalserver> - Opens the terminal server for framework commands " << std::endl;
			std::cout << "          over TCP/IP." << std::endl;
			std::cout << "close <terminalserver> - Closes the terminal server for framework commands " << std::endl;
			std::cout << "          over TCP/IP." << std::endl;

			(void)m_Map.runConsoleCommand("map help", this, NULL);
			if(getNumberOfMO() > 0)(void)(m_MO.begin()->runConsoleCommand("mo 0 help", this, NULL));

			return true;
		}

		case MGComponent_INPUT_BOOL:
		{
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
			MGFLOG_INFO("" << toInt(cmdvec[0], s));
			return true;
		}

		case MGComponent_SYMBOL_ASSIGNTO_INT:
		{
			// TODO: Why is this not ok?
			// setCommandReturnValue(toInt(cmdvec[2], s));
			symbolAssignTo(cmdvec, s);
			return true;
		}

		default:
			MGFLOG_ERROR("MGFramework::detectComponentConsoleCommand returned a bad value"); 
			return true;
	}

	std::cout << "Unknown command" << std::endl;
	return true;
}


eMGComponentConsoleCommand MGFramework::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	if(cmdvec.size() == 1)
	{
		if(cmdvec[0] == "exit")
		{
			return MGComponent_EXIT;
		}
		else if(cmdvec[0] == "help")
		{
			return MGComponent_HELP;
		}
		else if(cmdvec[0] == "runoneframe")
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
		if(cmdvec[0] == "setfps")
		{
			return MGComponent_SETFPS_INT;
		}
		else if(cmdvec[0] == "open" && cmdvec[1] == "terminalserver")
		{
			return MGComponent_OPEN_TERMINALSERVER;
		}
		else if(cmdvec[0] == "close" && cmdvec[1] == "terminalserver")
		{
			return MGComponent_CLOSE_TERMINALSERVER;
		}
		else if(cmdvec[0] == "logging")
		{
			return MGComponent_LOGGING_BOOL;
		}
		else if(cmdvec[0] == "minimap")
		{
			return MGComponent_MINIMAP_BOOL;
		}
		else if(cmdvec[0] == "dynamicfps")
		{
			return MGComponent_DYNAMICFPS_BOOL;
		}
		else if(cmdvec[0] == "runframes")
		{
			return MGComponent_RUNFRAMES_INT;
		}
		else if(cmdvec[0] == "exit" && cmdvec[1] == "application")
		{
			return MGComponent_EXIT_APPLICATION;
		}
		else if(cmdvec[0] == "input")
		{
			return MGComponent_INPUT_BOOL;
		}
	}
	else if(cmdvec.size() > 2)
	{
		if(cmdvec[0] == "add" && cmdvec[1] == "mo")
		{
			return MGComponent_ADD_MO_INT_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0] == "add" && cmdvec[1] == "so")
		{
			return MGComponent_ADD_SO_INT_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0] == "add" && cmdvec[1] == "pe")
		{
			return MGComponent_ADD_PE_INT_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0] == "mo" && cmdvec[1] == "marked")
		{
			return MGComponent_MO_MARKED_X;
		}
		else if(cmdvec[0] == "mo" && cmdvec[1] == "all")
		{
			return MGComponent_MO_ALL_X;
		}
		else if(cmdvec[0] == "mo")
		{
			return MGComponent_MO_INT_X;
		}
		else if(cmdvec[0] == "pe" && cmdvec[1] == "all")
		{
			return MGComponent_PE_ALL_X;
		}
		else if(cmdvec[0] == "pe")
		{
			return MGComponent_PE_INT_X;
		}
		else if(cmdvec[0] == "delete" && cmdvec[1] == "all" && cmdvec[2] == "mo" )
		{
			return MGComponent_DELETE_ALL_MO_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0] == "delete" && cmdvec[1] == "all" && cmdvec[2] == "so" )
		{
			return MGComponent_DELETE_ALL_SO_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0] == "delete" && cmdvec[1] == "all" && cmdvec[2] == "pe" )
		{
			return MGComponent_DELETE_ALL_PE_PARAMLIST; // Zero or more parameters..
		}
		else if(cmdvec[0] == "expect")
		{
			return MGComponent_EXPECT_INT_INT;
		}
		else if(cmdvec[0] == "delete" && cmdvec[1] == "mo")
		{
			return MGComponent_DELETE_MO_INT;
		}
		else if(cmdvec[0] == "delete" && cmdvec[1] == "so")
		{
			return MGComponent_DELETE_SO_INT;
		}
		else if(cmdvec[0] == "so" && cmdvec[1] == "all")
		{
			return MGComponent_SO_ALL_X;
		}
		else if(cmdvec[0] == "so")
		{
			return MGComponent_SO_INT_X;
		}
		else if(cmdvec[1] == "=" || cmdvec[1] == "assignto")
		{
			return MGComponent_SYMBOL_ASSIGNTO_INT;
		}
		else if(cmdvec[0] == "window" && cmdvec[1] == "fullscreen")
		{
			return MGComponent_WINDOW_FULLSCREEN_BOOL;
		}
	}

	if(cmdvec.size() > 1)
	{
		if(cmdvec[0] == "map")
		{
			return MGComponent_MAP_X;
		}
	}

	// MGFramework failed to detect a proper command in the given string..
	return MGComponent_UNDEFINED;
}


unsigned int MGFramework::getFPS()
{
	/*
	// XXX: Will this give more accurate timing?
	if(m_ActualFrameTime>0) return (unsigned int)(1000/m_ActualFrameTime);
	return getDesiredFPS();
	*/

	if(m_FrameTime > 0)
	{
		unsigned int result = (unsigned int)(1000 / m_FrameTime);
		if((result > 0) && (result < (2 * getDesiredFPS())))
		{
			return result;
		}
		else
		{
			MGFLOG_WARNING("FPS incorrectly calculated (MGFramework::getIntFPS()): " << "m_FrameTime:" << m_FrameTime << ", result:" << result);
			return getDesiredFPS();
		}
	}
	else
	{
		MGFLOG_WARNING("FPS incorrectly calculated (MGFramework::getIntFPS()): " << m_FrameTime);
		return getDesiredFPS();
	}
}


void MGFramework::setDesiredFPS(unsigned int f)
{
	if(f >= 1) // Don't allow an FPS lower than 1
	{
		m_FPS = f;
	}
	else
	{
		MGFLOG_WARNING("FPS incorrectly calculated (void MGFramework::setDesiredFPS(f)): " << f);
	}
}


unsigned int MGFramework::getDesiredFPS()
{
	if(m_FPS >= 1) // Don't allow an FPS lower than 1
	{
		return m_FPS;
	}
	else
	{
		MGFLOG_WARNING("FPS incorrectly calculated (MGFramework::getDesiredFPS()): " << m_FPS << ", returning 1");
		return 1;
	}
}


bool MGFramework::setWindowProperties(int width, int height, int bpp, bool fullscreen, const std::string &title, IMGWindow *win)
{
	m_Window = win;
	if(m_Window != NULL)
	{
		getWindow()->setProperties(width, height, bpp, fullscreen, title);
		m_WindowPropertiesSet = true;
		return true;
	}
	return false;
}


bool MGFramework::setWindowProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const std::string &title, IMGWindow *win)
{
	m_Window = win;
	if(m_Window != NULL)
	{
		getWindow()->setProperties(screenResolution, bpp, fullscreen, title);
		m_WindowPropertiesSet = true;
		return true;
	}
	return false;
}


void MGFramework::deleteAllMO()
{
	// Since all MO are deleted we can unoccupy all their tiles..
	for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
	{
		m_Map.unOccupy(it->getTileX(), it->getTileY());
		if(isSelectiveTileRenderingActive())
		{
			m_Map.markForRendering(it->getTileX(), it->getTileY());
		}
	}
	
	m_MO.clear();
	m_MarkedMOs = 0;
	setRenderAllTiles();
}


void MGFramework::addMO(int n)
{
	int tmpTotal = n + getNumberOfMO();

	if(tmpTotal > m_Map.getWidth()*m_Map.getHeight())
	{
		MGFLOG_ERROR(	"MGFramework::addMO cannot create " << tmpTotal << " MO on a " 
						<< m_Map.getWidth() << " by " << m_Map.getHeight() << " map")
		return;
	}

	for(int i = 0; i < n; i++)
	{
		MGMovingObject mo;
		mo.initialize();
		m_MO.push_back(mo);
	}

	setRenderAllTiles();
}


void MGFramework::deleteMO(std::list<MGMovingObject>::iterator it)
{
	if(it != m_MO.end())
	{
		m_Map.unOccupy(it->getTileX(), it->getTileY());
		if(isSelectiveTileRenderingActive()) m_Map.markForRendering(it->getTileX(), it->getTileY());
		if(it->isMarked())
		{
			m_MarkedMOs--;
		}
		m_MO.erase(it);
		setRenderAllTiles();
	}
}


bool MGFramework::setupMO(std::list<MGMovingObject>::iterator it, int x, int y, unsigned int owner, int speed, int x1, int y1, int x2, int y2)
{
	if(it == m_MO.end())
	{
		return false;
	}
	if(x < 0) x = randomN(x2 - x1) + x1;
	if(y < 0) y = randomN(y2 - y1) + y1;
	bool successful = false;

	for(int q = 0; q < MGF_MOPOSITIONINGATTEMPTS; ++q)
	{
		if(m_Map.occupant(x,y) != 0)
		{
			x = randomN(x2 - x1) + x1;
			y = randomN(y2 - y1) + y1;
		}
		else
		{
			successful = true;
			break;
		}
	}
	// Find the first available (x,y) since the random placement failed.
	if(!successful)
	{
		for(int t = 0; t < m_Map.getWidth() * m_Map.getHeight(); ++t)
		{
			if(m_Map.occupant(m_Map.getTileX(t), m_Map.getTileY(t)) == 0)
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
		it->setTileXY(x, y, this);
		it->setDestTileXY(it->getTileX(), it->getTileY());
		it->setSpeed(1.0 / (double)speed, m_Map.getTileHeight()); // speed = 2 means 2 tiles per second
		it->setOwner(owner);
		m_Map.occupy(it->getTileX(), it->getTileY(), it->getID());
		if(isSelectiveTileRenderingActive()) m_Map.markForRendering(it->getTileX(), it->getTileY());
	}
	else
	{
		MGFLOG_ERROR("Failed to find space for MO at creation");
		deleteMO(it);
		return false;
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
		if(x < 0)
		{
			x = randomN(m_Map.getWidth());
		}
		if(y < 0)
		{
			y = randomN(m_Map.getHeight());
		}
		bool successful=false;

		for(int q = 0; q < MGF_SOPOSITIONINGATTEMPTS; ++q)
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
			for(int t = 0; t < m_Map.getWidth() * m_Map.getHeight(); ++t)
			{
				if(m_Map.occupant(m_Map.getTileX(t), m_Map.getTileY(t)) == 0)
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
			if(isSelectiveTileRenderingActive()) m_Map.markForRendering(m_SO[i].getTileX(), m_SO[i].getTileY());
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


void MGFramework::deleteAllPE()
{
	if(m_PE) delete[] m_PE;
	m_PE = NULL;
	m_NPE = 0;
}


void MGFramework::addPE(int n)
{
	MGPeriodicEvent *oldPE = new MGPeriodicEvent[getNumberOfPE()];
	int nOld = getNumberOfPE();
	for(int i = 0; i < nOld; i++)
	{
		oldPE[i].copy(&m_PE[i]);
	}
	if(m_PE) delete[] m_PE;
	m_PE = NULL;
	m_NPE = nOld + n;
	m_PE = new MGPeriodicEvent[getNumberOfPE()];
	for(int i = 0; i < nOld; i++)
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
		for(int i = index; i < getNumberOfPE() - 1; ++i)
		{
			// Overwrite pe(i) with pe(i+1)
			m_PE[i].copy(&m_PE[i + 1]);
		}
		m_NPE = getNumberOfPE() - 1;
	}
}


void MGFramework::quit()
{ 
	m_Quit = true; 
	m_Map.printStatisticsCounters();
	std::cout << "Execution time: " << MGF_GetExecTimeMS() << std::endl;
}


void MGFramework::drawTile(void* imageSurface, int srcX, int srcY, int dstX, int dstY, int tileW, int tileH)
{
	increaseDrawnTilesCounter();
	getWindow()->drawSprite(imageSurface, srcX, srcY, dstX, dstY, tileW, tileH);
}


void MGFramework::drawTile(void* imageSurface, int srcX, int srcY, int dstX, int dstY)
{
	increaseDrawnTilesCounter();
	getWindow()->drawSprite(imageSurface, srcX, srcY, dstX, dstY, m_Map.getTileWidth(), m_Map.getTileHeight());
}


bool MGFramework::isNumericalInt(const std::string &s)
{
	if(s.size() == 0)
	{
		return false;
	}
	else if(s.size() == 1)
	{
		return s[0] >= '0' && s[0] <= '9';
	}
	else
	{
		for(unsigned int i = 1; i < s.size(); ++i)
		{
			if(s[i] < '0' || s[i] > '9')
			{
				return false;
			}
		}
		if(s[0] < '0' || s[0] > '9')
		{
			if(s[0] != '-')
			{
				return false;
			}
		}
		return true;
	}
	MGFLOG_STATIC_ERROR("MGFramework::isNumericalInt failed to parse string");
}

int MGFramework::staticToInt(const std::string &s)
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

int MGFramework::toInt(const std::string &s, MGSymbolTable *sym)
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
		if(s == std::string("$cmd"))
		{
			return m_CommandReturnVal;
		}
		else if(s == std::string("on"))
		{
			return MGF_TRUE;
		}
		else if(s == std::string("off"))
		{
			return MGF_FALSE;
		}
		else if(s == std::string("random_mo"))
		{
			if(getNumberOfMO() == 0)
			{
				MGFLOG_ERROR("MGFramework::toInt was called with 'random_mo' when no MO existed");
			}
			else
			{
				return randomN(getNumberOfMO());
			}
		}
		else if(s == std::string("random_pe"))
		{
			if(getNumberOfPE() == 0)
			{
				MGFLOG_ERROR("MGFramework::toInt was called with 'random_pe' when no PE existed");
			}
			else
			{
				return randomN(getNumberOfPE());
			}
		}
		else if(s == std::string("random_x"))
		{
			return randomN(m_Map.getWidth());
		}
		else if(s == std::string("random_y"))
		{
			return randomN(m_Map.getHeight());
		}
		else if(s == std::string("getnumberofmarkedmo"))
		{
			return getNumberOfMarkedMO();
		}
		else if(s == std::string("getnumberofmo"))
		{
			return getNumberOfMO();
		}
		else if(s == std::string("getnumberofso"))
		{
			return getNumberOfSO();
		}
		else if(s == std::string("getnumberofpe"))
		{
			return getNumberOfPE();
		}
		else if(s == std::string("getfps"))
		{
			return getFPS();
		}
		else
		{
			// First look in the symbol table of local variables..
			if(sym)
			{
				// TODO: Access the symbol through public getters and not the table member (which should be made private)
				for (std::deque<MGSymbolTable::MGSymbolTablePair>::iterator it = sym->table.begin(); it != sym->table.end(); ++it)
				{
					if(it->symbol == s)
					{
						return it->value;
					}
				}				
			}
			// Then the one of global..
			// TODO: Access the symbol through public getters and not the table member (which should be made private)
			for (std::deque<MGSymbolTable::MGSymbolTablePair>::iterator it = m_SymbolTable->table.begin(); it != m_SymbolTable->table.end(); ++it)
			{
				if(it->symbol == s)
				{
					return it->value;
				}
			}
		}
	}
	MGFLOG_ERROR("MGFramework::toInt failed to convert string to integer: " << s);
	if(sym)
	{
		sym->printTable();
	}
	return 0;
}


int MGFramework::initializeWinsock(WORD wVersionRequested)
{
#ifndef MGF_DISABLE_WINSOCK
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);

	if(err != 0)
	{
		return 0; // Tell the user that we couldn't find a usable winsock.dll 
	}
	if(LOBYTE(wsaData.wVersion ) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		return 0;
	}
	//Everything is fine: proceed
#endif
	return 1;
}


bool MGFramework::okMGFrameworkSyntax(const std::vector<std::string> &v_s)
{
	// XXX: This function is just an embryo...
	if(v_s.size() > 0 && v_s[v_s.size() - 1] != std::string(""))
	{
		return true;
	}
	return false;
}


void MGFramework::addConsoleCommandToQueue(const char *c)
{

}


void MGFramework::deleteAllSO()
{
	if(m_SO) 
	{
		for(int i = 0; i<getNumberOfSO(); ++i)
		{
			m_Map.unOccupy(m_SO[i].getTileX(), m_SO[i].getTileY());
			if(isSelectiveTileRenderingActive()) m_Map.markForRendering(m_SO[i].getTileX(), m_SO[i].getTileY());
		}

		delete[] m_SO;
		m_SO = NULL;
		m_NSO = 0;
	}
	// Make sure tiles are re-rendered after deleting SOs
	setRenderAllTiles();
}

void MGFramework::addSO(int n)
{
	MGStationaryObject *oldSO = new MGStationaryObject[getNumberOfSO()];
	int nOld=getNumberOfSO();
	for(int i = 0; i < nOld; i++)
	{
		oldSO[i].copy(&m_SO[i]);
	}
	if(m_SO) delete[] m_SO;
	m_SO = NULL;
	m_NSO = nOld + n;
	m_SO = new MGStationaryObject[getNumberOfSO()];
	for(int i = 0; i < nOld; i++)
	{
		m_SO[i].copy(&oldSO[i]);
	}
	// Make sure tiles are re-rendered after creating SOs
	setRenderAllTiles();
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
		if(isSelectiveTileRenderingActive()) m_Map.markForRendering(m_SO[index].getTileX(), m_SO[index].getTileY());

		for(int i = index; i < getNumberOfSO() - 1; ++i)
		{
			// Overwrite so(i) with so(i+1)
			m_SO[i].copy(&m_SO[i + 1]);
		}
		m_NSO = getNumberOfSO() - 1;
	}
	// Make sure tiles are re-rendered after deleting SOs
	setRenderAllTiles();
}


int runMGFrameworkSocketTerminal(void *fm)
{
#ifndef MGF_DISABLE_WINSOCK
	MGFramework *mgf = (MGFramework *)fm;
	int PORTNR = mgf->getPort();
	mgf->logIfEnabled((std::string("Opening socket terminal... port ") + MGFramework::toString(mgf->getPort())).c_str());
	
	bool connectionOpen = true;
	int nZerosInARow = 0;

	while(mgf->socketTerminalOpen())
	{
		if(!mgf->initializeWinsock (MAKEWORD(1, 1) ) ) 
		{
			mgf->logIfEnabled("Error initializing Winsock.");
			return 1;
		}

		SOCKET fd, fd_new; 	// "file" descriptors for the network sockets
		SOCKADDR_IN saddr_me;

		if((fd=socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
		{
			mgf->logIfEnabled("Server: socket not connected ");
			return 1;
		}

		saddr_me.sin_family = AF_INET;
		saddr_me.sin_addr.s_addr= htonl(INADDR_ANY);
		saddr_me.sin_port = htons(PORTNR);

		if(bind(fd, (LPSOCKADDR) &saddr_me, sizeof(saddr_me)) == SOCKET_ERROR)
		{
			mgf->logIfEnabled("Server: bind failure ");
			return 1;
		}

		if(listen(fd, 1) == SOCKET_ERROR)
		{
			mgf->logIfEnabled("Server: listen failure ");
			return 1;
		}

		// the server is now started and ready to accept a command..
		//mgf->logIfEnabled("Waiting for terminal command...");

		if( (fd_new=accept(fd, NULL, NULL)) == INVALID_SOCKET)
		{
			mgf->logIfEnabled("Server: accept failure ");
			return 1;
		}

		while(connectionOpen)
		{
			char buf[256];
			for (int i = 0; i < 256; i++)
			{
				buf[i] = 0;
			}
			if(recv(fd_new, buf, sizeof(buf), 0) == SOCKET_ERROR )
			{
				mgf->logIfEnabled("Server: receiving failure ");
				return 1;
			}

			// Now buf contains the request string.

			int lBuf = (int)strlen(buf);
			if(lBuf == 0)
			{
				nZerosInARow++;
			}
			else
			{
				nZerosInARow = 0;
				mgf->runConsoleCommand(buf, mgf, NULL);
				if(send(fd_new, "ok\n\r", 4, 0) == SOCKET_ERROR)
				{
					mgf->logIfEnabled("Server: Failed sending an answer to client request");
					return 1;
				}
			}
			if(nZerosInARow>4)
			{
				connectionOpen = false;
			}
		}
		WSACleanup();
	}
	mgf->logIfEnabled("Socket terminal closed...");
#endif
	return 0;
}
