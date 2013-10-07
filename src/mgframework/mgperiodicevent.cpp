#include "mgframework.h"
#include "mgperiodicevent.h"
#include <SDL/SDL.h>




MGPeriodicEvent::MGPeriodicEvent()
	: m_Period(1000), 
	m_StartTime(0), 
	m_isActive(false), 
	m_FileName1(NULL)
{
	registerMemoryAllocation(sizeof(MGPeriodicEvent));
}

MGPeriodicEvent::~MGPeriodicEvent()
{
	registerMemoryDeallocation(sizeof(MGPeriodicEvent));
	delete [] m_FileName1;
}

void MGPeriodicEvent::setupTimer(int ms)
{
	m_Period = ms;
}

bool MGPeriodicEvent::update()
{
	if (!isActive())
	{
		return false;
	}
	if (timeLeft()<=0)
	{
		m_StartTime = SDL_GetTicks();
		return true;
	}
	else
	{
		return false;
	}
}

void MGPeriodicEvent::activate()
{
	m_isActive = true;
	m_StartTime = SDL_GetTicks();
}

void MGPeriodicEvent::deactivate()
{
	m_isActive = false;
}

bool MGPeriodicEvent::isActive()
{
	return m_isActive;
}

int MGPeriodicEvent::timeLeft()
{
	return m_Period - (SDL_GetTicks() - m_StartTime);
}


void MGPeriodicEvent::copy(const MGPeriodicEvent *src)
{
	//Copy ALL class variables!
	m_Period = src->m_Period;
	m_StartTime = src->m_StartTime;
	m_isActive = src->m_isActive;
	m_Owner = src->m_Owner;
	m_LoggingEnabled = src->m_LoggingEnabled;
	m_FileName1 = src->m_FileName1;
	m_ID = src->m_ID;
}

bool MGPeriodicEvent::runConsoleCommand(const char *c, MGFramework *w)
{
	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
		{
			MGFLOG_ERROR("MGPeriodicEvent::runConsoleCommand received MGComponent_UNDEFINED from MGPeriodicEvent::detectMGComponentConsoleCommand"); 
			break;
		}

		case MGComponent_PE_INT_HELP:
		{
			w->registerUsedCommand(MGComponent_PE_INT_HELP);
			std::cout << "Help for PE console commands: " << std::endl;
			return true;
		}

		case MGComponent_PE_INT_ACTIVATE:
		{
			w->registerUsedCommand(MGComponent_PE_INT_ACTIVATE);
			activate();
			return true;
		}

		case MGComponent_PE_INT_DEACTIVATE:
		{
			w->registerUsedCommand(MGComponent_PE_INT_DEACTIVATE);
			deactivate();
			return true;
		}

		case MGComponent_PE_INT_SETUPTIMER_INT:
		{
			w->registerUsedCommand(MGComponent_PE_INT_SETUPTIMER_INT);
			int ms=w->toInt(cmdvec[3]);
			setupTimer(ms);
			return true;
		}

		case MGComponent_PE_INT_LOGGING_ON:
		{
			w->registerUsedCommand(MGComponent_PE_INT_LOGGING_ON);
			enableLogging();
			MGFLOG_INFO("Logging enabled.");
			return true;
		}

		case MGComponent_PE_INT_LOGGING_OFF:
		{
			w->registerUsedCommand(MGComponent_PE_INT_LOGGING_OFF);
			MGFLOG_INFO("Logging disabled.");
			disableLogging();
			return true;
		}

		case MGComponent_PE_INT_STOREFILENAME_FILENAME:
		{
			w->registerUsedCommand(MGComponent_PE_INT_STOREFILENAME_FILENAME);
			MGFLOG_INFO("MGPeriodicEvent::runConsoleCommand: Storing file name " << cmdvec[3].c_str());
			setFileName1(cmdvec[3].c_str());
			return true;
		}

		case MGComponent_PE_ALL_ACTIVATE_PARAMLIST:
		{
			w->registerUsedCommand(MGComponent_PE_ALL_ACTIVATE_PARAMLIST);
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					int owner = w->MGFramework::toInt(cmdvec[i+1]);
					++i;
					if(getOwner() == owner)
					{
						activate();
					}
				}
				else
				{
					MGFLOG_ERROR("Error in command (pe all activate <paramlist>), bad parameter list");
				}
			}
			return true;
		}


		case MGComponent_PE_ALL_DEACTIVATE_PARAMLIST:
		{
			w->registerUsedCommand(MGComponent_PE_ALL_DEACTIVATE_PARAMLIST);
			for(unsigned int i = 3; i < cmdvec.size(); ++i)
			{
				if(cmdvec[i]=="-owner" && cmdvec.size() > (i + 1))
				{
					int owner = w->MGFramework::toInt(cmdvec[i+1]);
					++i;
					if(getOwner() == owner)
					{
						deactivate();
					}
				}
				else
				{
					MGFLOG_ERROR("Error in command (pe all deactivate <paramlist>), bad parameter list");
				}
			}
			return true;
		}

		default:
			break;
	}

	std::cout << "Unknown command" << std::endl;
	return true;
}


eMGComponentConsoleCommand MGPeriodicEvent::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	if(cmdvec.size() == 3)
	{
		if(cmdvec[0]=="pe" && cmdvec[2]=="help")
		{
			return MGComponent_PE_INT_HELP;
		}
		else if(cmdvec[0]=="pe" && cmdvec[2]=="activate")
		{
			return MGComponent_PE_INT_ACTIVATE;
		}
		else if(cmdvec[0]=="pe" && cmdvec[2]=="deactivate")
		{
			return MGComponent_PE_INT_DEACTIVATE;
		}

	}
	else if(cmdvec.size() >= 4)
	{
		if(cmdvec[0]=="pe" && cmdvec[2]=="setuptimer")
		{
			return MGComponent_PE_INT_SETUPTIMER_INT;
		}
		else if(cmdvec[0]=="pe" && cmdvec[2]=="logging" && cmdvec[3]=="on")
		{
			return MGComponent_PE_INT_LOGGING_ON;
		}
		else if(cmdvec[0]=="pe" && cmdvec[2]=="logging" && cmdvec[3]=="off")
		{
			return MGComponent_PE_INT_LOGGING_OFF;
		}
		else if(cmdvec[0]=="pe" && (cmdvec[2]=="<<" || cmdvec[2]=="storefilename"))
		{
			return MGComponent_PE_INT_STOREFILENAME_FILENAME;
		}
		else if(cmdvec[0]=="pe" && cmdvec[2]=="activate")
		{
			return MGComponent_PE_ALL_ACTIVATE_PARAMLIST;
		}
		else if(cmdvec[0]=="pe" && cmdvec[2]=="deactivate")
		{
			return MGComponent_PE_ALL_DEACTIVATE_PARAMLIST;
		}
	}

	// MGPeriodicEvent failed to detect a proper command in the given string..
	return MGComponent_UNDEFINED;
}

void MGPeriodicEvent::setFileName1(const char *c)
{
	MGFLOG_INFO("MGPeriodicEvent::setFileName1: Allocating " << strlen(c)+1 << " bytes for file name 1");
	delete [] m_FileName1;
	m_FileName1 = new char[strlen(c)+1];
	strcpy(m_FileName1,c);
}

void MGPeriodicEvent::runFile1(MGFramework *w)
{
	if(m_FileName1 != NULL)
	{
		if(w != NULL)
		{
			w->parse(m_FileName1);
		}
		else
		{
			MGFLOG_ERROR("MGPeriodicEvent::runFile1 was called with no callback framework pointer.");
		}
	}
	else
	{
		MGFLOG_ERROR("MGPeriodicEvent::runFile1 was called with no file name set."); 
	}
}