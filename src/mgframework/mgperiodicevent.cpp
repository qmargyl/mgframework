#include "mgframework.h"
#include "mgperiodicevent.h"
#include <SDL/SDL.h>




MGPeriodicEvent::MGPeriodicEvent()
: m_Period(1000), m_StartTime(0), m_isActive(false) 
{

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
	m_Period = src->m_Period;
	m_StartTime = src->m_StartTime;
	m_isActive = src->m_isActive;
}

bool MGPeriodicEvent::runConsoleCommand(const char *c, MGFramework *w)
{
	MGFLOG(std::cout << "MGPeriodicEvent::runConsoleCommand(" << c << ")" << std::endl;);

	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');

	switch(detectMGComponentConsoleCommand(cmdvec))
	{
		case MGComponent_UNDEFINED:
			MGFPRINT(std::cout << "Error in command (pe ...), MGFPECMD_UNDEFINED received from detectMGComponentConsoleCommand" << std::endl;); 
			break;

		case MGComponent_PE_INT_HELP:
			std::cout << "Help for PE console commands: " << std::endl;
			return true;

		case MGComponent_PE_INT_ACTIVATE:
			activate();
			return true;

		case MGComponent_PE_INT_DEACTIVATE:
			deactivate();
			return true;

		case MGComponent_PE_INT_SETUPTIMER_INT:
		{
			int ms=MGFramework::toInt(cmdvec[3]);
			setupTimer(ms);
			return true;
		}

		case MGComponent_PE_INT_LOGGING_ON:
			enableLogging();
			MGFPRINT(std::cout << "Logging enabled." << std::endl;);
			return true;

		case MGComponent_PE_INT_LOGGING_OFF:
			disableLogging();
			MGFPRINT(std::cout << "Logging disabled." << std::endl;);
			return true;

		default:
			break;
	}

	return true;
}


eMGComponentConsoleCommand MGPeriodicEvent::detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec)
{
	if(cmdvec.size() < 3)
	{
		return MGComponent_UNDEFINED;
	}
	else if(cmdvec.size() == 3)
	{
		if(cmdvec[0]=="pe" && cmdvec[2]=="HELP")
		{
			return MGComponent_PE_INT_HELP;
		}
		else if(cmdvec[0]=="pe" && cmdvec[2]=="ACTIVATE")
		{
			return MGComponent_PE_INT_ACTIVATE;
		}
		else if(cmdvec[0]=="pe" && cmdvec[2]=="DEACTIVATE")
		{
			return MGComponent_PE_INT_DEACTIVATE;
		}

	}
	else if(cmdvec.size() == 4)
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
	}

	// MGPeriodicEvent failed to detect a proper command in the given string..
	return MGComponent_UNDEFINED;
}