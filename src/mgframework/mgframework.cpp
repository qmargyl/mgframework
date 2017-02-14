#include "mgframework.h"
#include <iostream>
#include <fstream>
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
	m_FeatureConsoleEnabled(true),
	m_WindowPropertiesSet(false),
	m_MiniMapEnabled(true),
	m_MGFInstanceType(MGFSINGLEPLAYERINSTANCE),
	m_PlayerNumber(MGF_NOPLAYER),
	m_ActualFrameTime(16),
	m_FPS(60),
	m_DelayTime(0),
	m_FeatureMouseScrollingEnabled(true),
	m_FrameCountdownEnabled(false),
	m_FrameNumber(0),
	m_MarkedMOs(0),
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
	m_FeatureCenterOnMO(-1),
	m_SymbolTable(NULL),
	m_SymbolTableTransfer(NULL)
{
	std::srand((int)std::time(0));
	m_SymbolTable = new MGSymbolTable();
	m_SymbolTableTransfer = new MGSymbolTable();
}

MGFramework::~MGFramework()
{
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
					SDL_Keycode sym = event.key.keysym.sym;
					// Enable typing in the console if ESC is pressed and feature is enabled..
					if(featureConsoleEnabled() && sym == SDLK_ESCAPE)
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
					SDL_Keycode sym = event.key.keysym.sym;
					MGFLOG_INFO("SDL_KEYUP" << std::endl << "  " << SDL_GetKeyName(sym))
					m_Keys[sym] = 0;
				}
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				// Calculate index, x and y for the tile that was clicked in the map.
				int iClick = m_Map.getTileIndex(event.button.x, event.button.y);
				if(iClick >= 0)
				{
					int xClick = m_Map.getTileX(iClick);
					int yClick = m_Map.getTileY(iClick);
					
					if(((int) event.button.button) == 1)
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
						MGFLOG_INFO("\nFraming activated: index = " 
							<< iClick << ", x = " << xClick << ", y = " << yClick
							<< ", FrameStartX = " << getFrameStartX() << ", FrameStartY = " << getFrameStartY())
					}
					else if(((int) event.button.button) == 3)
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
							it->setPath(m_Map.calculatePath(MGFBASICPATH1, it->getNextTileX(), it->getNextTileY(), xClick, yClick));
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
				if(((int) event.button.button) == 1)
				{
					int endClickX = m_Map.getTileX(m_Map.getTileIndex(getFrameEndX(), getFrameEndY()));
					int endClickY = m_Map.getTileY(m_Map.getTileIndex(getFrameEndX(), getFrameEndY()));
					int startClickX = m_Map.getTileX(m_Map.getTileIndex(getFrameStartX(), getFrameStartY()));
					int startClickY = m_Map.getTileY(m_Map.getTileIndex(getFrameStartX(), getFrameStartY()));
					// TODO: When implementing support for too large frames, this has to be changed as it aborts on negative indexes now.
					if(endClickX > 0 && endClickY > 0 && startClickX > 0 && startClickY > 0)
					{
						for(int x = std::min(startClickX, endClickX); x <= std::max(startClickX, endClickX); x++)
						{
							for(int y = std::min(startClickY, endClickY); y <= std::max(startClickY, endClickY); y++)
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
					}
					MGFLOG_INFO("\nFraming will be deactivated: " 
						<< "fx1 = " << startClickX << ", fy1 = " << startClickY
						<< ", fx2 = " << endClickX << ", fy2 = " << endClickY
						<< ", FrameStartX = " << getFrameStartX() << ", FrameStartY = " << getFrameStartY()
						<< ", FrameEndX = " << getFrameEndX() << ", FrameEndY = " << getFrameEndY())
					deactivateFraming();
				}
				else if(((int) event.button.button) == 3)
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

							if(fColon != std::string::npos)
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

	// Frame loop (processEvents, handleGameLogics, draw)
	while(processEvents())
	{
		// Calculate the current frame time (and implicitly FPS)..
		frameStartTime = getWindow()->getExecTimeMS();

		// Handle all calculations and draw current frame..


		// If frame countdown is enabled (we are counting down frames to go into console again)
		// and the counter is zero, activate the console.
		if(frameCountdownEnabled())
		{
			static int nFrames = 0;
			if(getFrameNumber() == 0)
			{
				std::cout << "Frame countdown results: " << std::endl;
				std::cout << "\tNumber of frames executed: " << nFrames << std::endl;
				nFrames = 0;
				if(featureConsoleEnabled() && !runOneFrame)
				{
					activateConsole();
				}
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
		m_DelayTime = (1000 / getDesiredFPS()) - (getWindow()->getExecTimeMS() - frameStartTime);
		getWindow()->sleep(m_DelayTime);
		m_ActualFrameTime = getWindow()->getExecTimeMS() - frameStartTime;

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
	// Update periodic event to trigger rare events
	for(std::list<MGPeriodicEvent>::iterator it = m_PE.begin(); it != m_PE.end(); it++)
	{
		if(it->update())
		{
			MGFLOG_INFO("PE triggered an update");
			it->runFile1(this);
		}
	}

	// Update all moving objects
	for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
	{
		it->update(this);
	}

	getWindow()->resetDrawnSpritesCounter();
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


void MGFramework::activateFraming(int x, int y)
{
	setFrameStartX(x); 
	setFrameStartY(y); 
	setFrameEndX(x); 
	setFrameEndY(y); 
	m_FramingOngoing = true;
}


void MGFramework::deactivateFraming()
{ 
	m_FramingOngoing = false;
	setRenderAllTiles();
}


void MGFramework::updateFraming(int x, int y)
{
	setFrameEndX(x); 
	setFrameEndY(y);
	setRenderAllTiles();
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

		case MGComponent_DUMP:
		{
			dump();
			return true;
		}

		case MGComponent_MAP_X:
		{
			return m_Map.runConsoleCommand(c, this, s);
		}


		case MGComponent_WINDOW_FULLSCREEN_BOOL:
		{
			if(!toBool(cmdvec[2], s))
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
			unsigned int peIndex = (unsigned int)toInt(cmdvec[1], s);
			unsigned int i = 0;
			if(peIndex < getNumberOfPE())
			{
				for(std::list<MGPeriodicEvent>::iterator it = m_PE.begin(); it != m_PE.end(); it++)
				{
					if(i == peIndex)
					{
						return it->runConsoleCommand(c, this, s);
					}
					else
					{
						i++;
					}
				}
			}
			MGFLOG_WARNING("Console command was not forwarded to PE " << peIndex); 
			return true;
		}

		case MGComponent_PE_ALL_X:
		{
			for(std::list<MGPeriodicEvent>::iterator it = m_PE.begin(); it != m_PE.end(); it++)
			{
				it->runConsoleCommand(c, this, s);
			}
			return true;
		}

		case MGComponent_SO_INT_X:
		{
			unsigned int soIndex = (unsigned int)toInt(cmdvec[1], s);
			unsigned int i = 0;
			if(soIndex < getNumberOfSO())
			{
				for(std::list<MGStationaryObject>::iterator it = m_SO.begin(); it != m_SO.end(); it++)
				{
					if(i == soIndex)
					{
						return it->runConsoleCommand(c, this, s);
					}
					else
					{
						i++;
					}
				}
			}
			MGFLOG_WARNING("Console command was not forwarded to SO, SOIndex:" << soIndex << ", NumberOfSO:" << getNumberOfSO() << ", i:" << i); 
			return true;
		}

		case MGComponent_SO_ALL_X:
		{
			for(std::list<MGStationaryObject>::iterator it = m_SO.begin(); it != m_SO.end(); it++)
			{
				it->runConsoleCommand(c, this, s);
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
				// Delete only MOs connected to a specific owner
				MGFLOG_INFO("Deleting PEs owned by " << owner);
				for(std::list<MGPeriodicEvent>::iterator it = m_PE.begin(); it != m_PE.end(); )
				{
					MGFLOG_INFO("PE owned by " << it->getOwner());
					if(it->getOwner() == owner)
					{
						MGFLOG_INFO("Deleting one out of " << m_PE.size() << " PEs");
						if(it == m_PE.begin())
						{
							deletePE(it);
							it = m_PE.begin();
						}
						else
						{
							deletePE(it);
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
				//Deleting ALL PEs is faster than deleting them per owner 
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
				// Delete only SOs connected to a specific owner
				MGFLOG_INFO("Deleting SOs owned by " << owner);
				for(std::list<MGStationaryObject>::iterator it = m_SO.begin(); it != m_SO.end(); )
				{
					MGFLOG_INFO("SO owned by " << it->getOwner());
					if(it->getOwner() == owner)
					{
						MGFLOG_INFO("Deleting one out of " << m_SO.size() << " SOs");
						if(it == m_SO.begin())
						{
							deleteSO(it);
							it = m_SO.begin();
						}
						else
						{
							deleteSO(it);
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
			int nBefore = getNumberOfSO();
			int n = toInt(cmdvec[2], s);
			int x = -1; // Invalid default value
			int y = -1; // Invalid default value
			int t = 0;  // Type default value
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
				else if(cmdvec[i] == "-type" && cmdvec.size() > (i + 1))
				{
					t = toInt(cmdvec[i + 1], s);
					++i;
				}
				else
				{
					MGFLOG_ERROR("Error in command (add so <n>), bad parameter list");
					n = 0; // Abort SO creation..
				}
			}

			if(n > 0)
			{
				MGFLOG_INFO("Adding SOs: " << n << ", nBefore = " << nBefore);
				addSO(n);
			}
			else
			{
				MGFLOG_ERROR("Error in command (add so <n>)");
				return true;
			}

			// Loop from first new SO to the end.
			std::list<MGStationaryObject>::iterator it = m_SO.begin();
			for(int i = 0; i < nBefore; i++)
			{
				it++;
			}
			for(; it != m_SO.end(); )
			{
				// If setup returns true we step iterator, otherwise erase has already stepped it
				MGFLOG_INFO("SetupSO x=" << x << ", y=" << y << ", t=" << t);
				if(setupSO(it, x, y, t))
				{
					it++;
				}
			}

			m_SO.sort(); // Allows drawing the objects from screen top to bottom
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

			// Loop from first new PE to the end.
			std::list<MGPeriodicEvent>::iterator it = m_PE.begin();
			for(int i = 0; i < nBefore; i++)
			{
				it++;
			}
			for(; it != m_PE.end(); it++)
			{
				it->setOwner(owner);
			}
			return true;
		}

		case MGComponent_OPEN_TERMINALSERVER:
		{
			MGFLOG_INFO("Opening terminal server...");
#ifndef MGF_DISABLE_WINSOCK
			openSocketTerminal();
			if(getInstanceType() == MGFSERVERINSTANCE)
			{
				m_SocketTerminal = SDL_CreateThread(MGSC::startServerThread, "TerminalServer", this);
			}
			else if(getInstanceType() == MGFCLIENTINSTANCE)
			{
				m_SocketTerminal = SDL_CreateThread(MGSC::startClientThread, "TerminalClient", this);
			}
			else if(getInstanceType() == MGFSINGLEPLAYERINSTANCE)
			{
				m_SocketTerminal = NULL;
			}
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
			unsigned int i = 0;
			if(moIndex < getNumberOfMO())
			{
				for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
				{
					if(i == moIndex)
					{
						return it->runConsoleCommand(c, this, s);
					}
					else
					{
						i++;
					}
				}
			}
			MGFLOG_WARNING("Console command was not forwarded to MO, MOIndex:" << moIndex << ", NumberOfMO:" << getNumberOfMO() << ", i:" << i); 
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
			unsigned int soIndex = (unsigned int)toInt(cmdvec[2], s);
			if(soIndex < getNumberOfSO())
			{
				std::list<MGStationaryObject>::iterator it = m_SO.begin();
				for(unsigned int i = 0; i < soIndex; i++)
				{
					it++;
				}
				deleteSO(it);
			}
			return true;
		}

		case MGComponent_LOGGING_BOOL:
		{
			if(toBool(cmdvec[1], s))
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
			if(toBool(cmdvec[1], s))
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
			if(toBool(cmdvec[1], s))
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


eMGComponentConsoleCommand MGFramework::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) const
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
		else if(cmdvec[0] == "dump")
		{
			return MGComponent_DUMP;
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


unsigned int MGFramework::getFPS() const
{
	if(m_ActualFrameTime > 0) return (unsigned int)(1000 / m_ActualFrameTime);
	return getDesiredFPS();
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


unsigned int MGFramework::getDesiredFPS() const
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

	if(tmpTotal > m_Map.getWidth() * m_Map.getHeight())
	{
		MGFLOG_ERROR(	"MGFramework::addMO cannot create " << tmpTotal << " MO on a " 
						<< m_Map.getWidth() << " by " << m_Map.getHeight() << " map")
		return;
	}

	for(int i = 0; i < n; i++)
	{
		MGMovingObject mo;
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
		it->setNextTileXY(it->getTileX(), it->getTileY(), this);
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


bool MGFramework::setupSO(std::list<MGStationaryObject>::iterator it, int x, int y, int t, bool occupyMap)
{
	if(it == m_SO.end())
	{
		return false;
	}

	// Until these are passed as parameters together with owner
	int x1 = 0;
	int y1 = 0;
	int x2 = m_Map.getWidth();
	int y2 = m_Map.getHeight();

	if(x < 0) x = randomN(x2 - x1) + x1;
	if(y < 0) y = randomN(y2 - y1) + y1;
	bool successful = false;

	for(int q = 0; q < MGF_SOPOSITIONINGATTEMPTS; ++q)
	{
		if(occupyMap && m_Map.occupant(x,y) != 0)
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
	// Find the first available (x,y) since the (random) placement failed.
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
		it->setTileXY(x, y, this, occupyMap);
		it->setType(t);
		// This is done by setTileXY -- m_Map.occupy(it->getTileX(), it->getTileY(), it->getID());
		if(isSelectiveTileRenderingActive()) m_Map.markForRendering(it->getTileX(), it->getTileY());
	}
	else
	{
		MGFLOG_ERROR("Failed to find space for SO at creation");
		deleteSO(it);
		return false;
	}
	return true;
}


void MGFramework::deleteAllPE()
{
	m_PE.clear();
}


void MGFramework::addPE(int n)
{
	for(int i = 0; i < n; i++)
	{
		MGPeriodicEvent pe;
		m_PE.push_back(pe);
	}
}


void MGFramework::deletePE(std::list<MGPeriodicEvent>::iterator it)
{
	if(it != m_PE.end())
	{
		m_PE.erase(it);
	}
}


void MGFramework::quit()
{ 
	m_Quit = true; 
	std::cout << "Execution time: " << getWindow()->getExecTimeMS() << std::endl;
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

bool MGFramework::toBool(const std::string &s, MGSymbolTable *sym) const
{
	if(s == std::string("on"))
	{
		return true;
	}
	else if(s == std::string("off"))
	{
		return false;
	}
	else if(toInt(s, sym) != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int MGFramework::toInt(const std::string &s, MGSymbolTable *sym) const
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


bool MGFramework::initializeWinsock(WORD wVersionRequested)
{
#ifndef MGF_DISABLE_WINSOCK
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);

	if(err != 0)
	{
		return false; // Tell the user that we couldn't find a usable winsock.dll 
	}
	if(LOBYTE(wsaData.wVersion ) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		return false;
	}
	//Everything is fine: proceed
#endif
	return true;
}


bool MGFramework::okMGFrameworkSyntax(const std::vector<std::string> &v_s) const
{
	// XXX: This function is just an embryo...
	if(v_s.size() > 0 && v_s[v_s.size() - 1] != std::string(""))
	{
		return true;
	}
	return false;
}


void MGFramework::deleteAllSO()
{
	// Since all SO are deleted we can unoccupy all their tiles..
	for(std::list<MGStationaryObject>::iterator it = m_SO.begin(); it != m_SO.end(); it++)
	{
		m_Map.unOccupy(it->getTileX(), it->getTileY());
		if(isSelectiveTileRenderingActive())
		{
			m_Map.markForRendering(it->getTileX(), it->getTileY());
		}
	}
	
	m_SO.clear();
	setRenderAllTiles();
}

void MGFramework::addSO(int n)
{
	int tmpTotal = n + getNumberOfSO();

	if(tmpTotal > m_Map.getWidth() * m_Map.getHeight())
	{
		MGFLOG_ERROR(	"MGFramework::addSO cannot create " << tmpTotal << " SO on a " 
						<< m_Map.getWidth() << " by " << m_Map.getHeight() << " map")
		return;
	}

	for(int i = 0; i < n; i++)
	{
		MGStationaryObject so;
		m_SO.push_back(so);
	}

	setRenderAllTiles();
}

void MGFramework::deleteSO(std::list<MGStationaryObject>::iterator it)
{
	if(it != m_SO.end())
	{
		m_Map.unOccupy(it->getTileX(), it->getTileY());
		if(isSelectiveTileRenderingActive()) m_Map.markForRendering(it->getTileX(), it->getTileY());
		m_SO.erase(it);
		setRenderAllTiles();
	}
}

void MGFramework::countUnMark()
{
	if(m_MarkedMOs != 0)
	{
		m_MarkedMOs--;
	}
	else
	{
		MGFLOG_ERROR("MGFramework::countUnMark decreased number of marked MO below zero");
	}
}

void MGFramework::increaseDensityOfStationaryObjects(int stationaryObjectType, int neighboursThreshold)
{
	if(neighboursThreshold < 1 || neighboursThreshold > 8 || (int)getNumberOfSO() < neighboursThreshold)
	{
		MGFLOG_ERROR(	"MGFramework::increaseDensityOfStationaryObjects cannot run algorithm on " 
						<< getNumberOfSO() << " existing SO with a neighbour limit of " << neighboursThreshold);
		return;
	}
	int nAddedSO = 0;
	int* forest = new int[m_Map.getWidth() * m_Map.getHeight()];
	for(int y = 0; y < m_Map.getHeight(); y++)
	{
		for(int x = 0; x < m_Map.getWidth(); x++)
		{
			forest[y * m_Map.getWidth() + x] = 0;
		}
	}
	for(std::list<MGStationaryObject>::iterator it = m_SO.begin(); it != m_SO.end(); it++)
	{
		if(it->getType() == stationaryObjectType)
		{
			forest[it->getTileY() * m_Map.getWidth() + it->getTileX()] = 1;
		}
	}
	
	for(int y = 1; y < m_Map.getHeight() - 1; y++)
	{
		for(int x = 1; x < m_Map.getWidth() - 1; x++)
		{
			if(m_Map.occupant(x, y) == 0)
			{
				// If x,y has three or more neighbours, create SO at x,y
				int neighbours = 	forest[(y - 1) * m_Map.getWidth() + (x - 1)] +
									forest[(y - 1) * m_Map.getWidth() + x] + 
									forest[(y - 1) * m_Map.getWidth() + (x + 1)] +
									forest[y * m_Map.getWidth() + (x - 1)] + 
									forest[y * m_Map.getWidth() + (x + 1)] +
									forest[(y + 1) * m_Map.getWidth() + (x - 1)] +
									forest[(y + 1) * m_Map.getWidth() + x] +
									forest[(y + 1) * m_Map.getWidth() + (x + 1)];
				if(neighbours > neighboursThreshold)
				{
					addSO(1);
					std::list<MGStationaryObject>::iterator it = m_SO.end();
					it--;
					if(setupSO(it, x, y, stationaryObjectType))
					{
						nAddedSO++;
					}
					else
					{
						MGFLOG_ERROR(	"MGFramework::increaseDensityOfStationaryObjects failed to setup new SO of type "
										<< stationaryObjectType << " at x=" << x << ", y=" << y);
						MGFLOG_ERROR("MGFramework::increaseDensityOfStationaryObjects might have corrupted the SO list");
						delete[] forest;
						return;
					}
				}
			}
		}
	}
	m_SO.sort();
	MGFLOG_INFO("MGFramework::increaseDensityOfStationaryObjects added " << nAddedSO << " SO of type " << stationaryObjectType);
	delete[] forest;
}

void MGFramework::fillInStationaryObjectClusters(int stationaryObjectType)
{
	if((int)getNumberOfSO() < 4)
	{
		MGFLOG_ERROR(	"MGFramework::fillInStationaryObjectClusters cannot run algorithm on " 
						<< getNumberOfSO() << " existing SO");
		return;
	}
	int nAddedSO = 0;
	int* forest = new int[m_Map.getWidth() * m_Map.getHeight()];
	for(int y = 0; y < m_Map.getHeight(); y++)
	{
		for(int x = 0; x < m_Map.getWidth(); x++)
		{
			forest[y * m_Map.getWidth() + x] = 0;
		}
	}
	for(std::list<MGStationaryObject>::iterator it = m_SO.begin(); it != m_SO.end(); it++)
	{
		if(it->getType() == stationaryObjectType)
		{
			forest[it->getTileY() * m_Map.getWidth() + it->getTileX()] = 1;
		}
	}
	for(int y = 0; y < m_Map.getHeight() - 1; y++)
	{
		for(int x = 0; x < m_Map.getWidth() - 1; x++)
		{
			if(	forest[y * m_Map.getWidth() + x] == 1 &&
				forest[y * m_Map.getWidth() + (x + 1)] == 1 &&
				forest[(y + 1) * m_Map.getWidth() + x] == 1 &&
				forest[(y + 1) * m_Map.getWidth() + (x + 1)] == 1)
			{
				addSO(1);
				std::list<MGStationaryObject>::iterator it = m_SO.end();
				it--;
				if(setupSO(it, x, y, stationaryObjectType, false))
				{
					nAddedSO++;
					it->setOffsetXY(m_Map.getTileWidth() / 2, m_Map.getTileHeight() / 2);
				}
				else
				{
					MGFLOG_ERROR(	"MGFramework::fillInStationaryObjectClusters failed to setup new SO of type "
									<< stationaryObjectType << " at x=" << x << ", y=" << y);
					MGFLOG_ERROR("MGFramework::fillInStationaryObjectClusers might have corrupted the SO list");
					delete[] forest;
					return;
				}
			}
		}
	}
	m_SO.sort();
	MGFLOG_INFO("MGFramework::fillInStationaryObjectClusters added " << nAddedSO << " SO of type " << stationaryObjectType);
	delete[] forest;
}

void MGFramework::setStationaryObjectTexHandles(int stationaryObjectType, std::vector<MGTexHandle*> tHVec)
{
	for(std::list<MGStationaryObject>::iterator it = m_SO.begin(); it != m_SO.end(); it++)
	{
		if(it->getType() == stationaryObjectType)
		{
			it->setTexHandle(tHVec);
		}
	}
}

void MGFramework::drawBasicMiniMap(int distFromUpRightX, int distFromUpRightY)
{
	if(featureMiniMapEnabled())
	{
		int miniX, miniY;
		getWindow()->drawFilledRectangleRGB(getWindow()->getWidth() - m_Map.getWidth() - distFromUpRightX,
											distFromUpRightY,
											getWindow()->getWidth() - distFromUpRightX,
											m_Map.getHeight() + distFromUpRightY,
											0x00, 0x00, 0x00);
		for(int x = 0; x < m_Map.getWidth(); x++)
		{
			miniX = x * m_Map.getTileWidth() + m_Map.getScrollX();
			for(int y = 0; y < m_Map.getHeight(); y++)
			{
				// Only draw the tiles actually visible in the window...
				miniY = y * m_Map.getTileHeight() + m_Map.getScrollY();
				if(  (miniX <= getWindow()->getWidth()) &&
					 (miniX >= 0) &&
					 (miniY <= getWindow()->getHeight()) &&
					 (miniY >= 0)  )
				{
					getWindow()->putPixelRGB(	x + getWindow()->getWidth() - m_Map.getWidth() - distFromUpRightX,
												y + distFromUpRightY,
												0x3F, 0x3F, 0x3F);
				}
			}
		}
		// Draw all moving objects on the mini map..
		for(std::list<MGMovingObject>::iterator it = m_MO.begin(); it != m_MO.end(); it++)
		{
			getWindow()->putPixelRGB(	it->getTileX() + getWindow()->getWidth() - m_Map.getWidth() - distFromUpRightX,
										it->getTileY() + distFromUpRightY,
										0xFF, 0x00, 0x00);
		}
	}
}

void MGFramework::drawAllSOWithTexHandles()
{
		int sX, sY;
		for(std::list<MGStationaryObject>::iterator it = m_SO.begin(); it != m_SO.end(); it++)
		{
			sX = it->getTileX() * m_Map.getTileWidth() + m_Map.getScrollX() + it->getXOffset();
			sY = it->getTileY() * m_Map.getTileHeight() + m_Map.getScrollY() + it->getYOffset() - 16;
			// Only draw visible stationary objects...
			if(detectCollisionRectangle(sX, sY, sX + m_Map.getTileWidth(), sY + m_Map.getTileHeight(), 0, 0, getWindow()->getWidth(), getWindow()->getHeight()))
			{
				if(it->getTexHandle())
				{
					getWindow()->drawSprite(*(it->getTexHandle()), 0, 0, sX, sY, m_Map.getTileWidth(), m_Map.getTileHeight() + 16);
				}
				else
				{
					getWindow()->drawFilledRectangleRGB(sX, sY, sX + m_Map.getTileWidth(), sY + m_Map.getTileHeight() + 16, 0, 255, 0);
				}
			}
		}
}

void MGFramework::dump(std::string addToName)
{
	std::string fileName = "dump_" + addToName + toString(getWindow()->getExecTimeMS()) + ".html";
	std::ofstream outFile;
	outFile.open(fileName.c_str(), std::ios_base::out);

	outFile << "<html><head><link rel=stylesheet href=mgf.css type=text/css><title>MG Framework dump</title></head><body>" << std::endl;

	// Printing some memory consumption related information
	outFile << "<h3>Memory consumption information</h3>" << std::endl;
	outFile << "<p>" << std::endl;
	outFile << "sizeof(MGFramework): " << sizeof(MGFramework) << "<br>" << std::endl;
	outFile << "sizeof(MGMovingObject): " << sizeof(MGMovingObject) << "<br>" << std::endl;
	outFile << "sizeof(MGStationaryObject): " << sizeof(MGStationaryObject) << "<br>" << std::endl;
	outFile << "sizeof(MGPeriodicEvent): " << sizeof(MGPeriodicEvent) << "<br>" << std::endl;
	outFile << "sizeof(MGMap): " << sizeof(MGMap) << "<br>" << std::endl;
	outFile << "KPI1 (sizeof(1 x MGMap(100, 100) + 100 x MGMovingObject + 1 x MGFramework)): "
		<< sizeof(MGFramework) + 100 * sizeof(MGMovingObject)
		+ sizeof(MGMap)
		+ (100 * 100) * sizeof(int) // occupied
		+ (100 * 100) * sizeof(unsigned int) // tile property
		+ (100 * 100) * sizeof(bool); // marked for rendering
	outFile << "<br>" << std::endl;
	outFile << "</p>" << std::endl;

	// Dumping framework parameters:
	outFile << "<h3>Framework parameters</h3>" << std::endl;
	outFile << "<p>" << std::endl;
	outFile << "CurrentTime(ms): " << getWindow()->getExecTimeMS() << "<br>" << std::endl;
	outFile << "NumberOfMO: " << getNumberOfMO() << "<br>" << std::endl;
	outFile << "NumberOfPE: " << getNumberOfPE() << "<br>" << std::endl;
	outFile << "NumberOfSO: " << getNumberOfSO() << "<br>" << std::endl;
	outFile << "<br>" << std::endl;
	outFile << "</p>" << std::endl;

	// Dumping all currently existing MOs:
	outFile << "<h3>List of currently existing Moving Objects</h3>" << std::endl;
	outFile << "<p>" << std::endl;
	for(std::list<MGMovingObject>::const_iterator it = m_MO.begin(); it != m_MO.end(); it++)
	{
		outFile << "[MO] ";
		outFile << "ID: " << it->getID();
		outFile << ", State: " << it->toString();
		outFile << ", TileX: " << it->getTileX();
		outFile << ", TileY: " << it->getTileY();
		outFile << ", NextTileX: " << it->getNextTileX();
		outFile << ", NextTileY: " << it->getNextTileY();
		outFile << ", Xoff: " << it->getXOffset();
		outFile << ", Yoff: " << it->getYOffset();
		outFile << "<br>" << std::endl;
	}
	outFile << "</p>" << std::endl;
	outFile << "</body></html>" << std::endl;
}
