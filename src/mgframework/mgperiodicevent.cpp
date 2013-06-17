#include "mgframework.h"
#include "mgperiodicevent.h"
#include <SDL/SDL.h>




MGPeriodicEvent::MGPeriodicEvent()
: m_Period(1000), m_StartTime(0), m_isActive(false) 
{

}


eMGFPEConsoleCommand MGPeriodicEvent::detectPEConsoleCommand(const std::vector<std::string> &cmdvec)
{
	return MGFPECMD_UNDEFINED;
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

//	MGFPECMD_UNDEFINED = 0,
//	MGFPECMD_HELP,
//	MGFPECMD_ACTIVATE,
//	MGFPECMD_DEACTIVATE,
//	MGFPECMD_SETUPTIMER,
//	MGFPECMD_LOGGING_ON,
//	MGFPECMD_LOGGING_OFF

	switch(detectPEConsoleCommand(cmdvec))
	{
		case MGFPECMD_UNDEFINED:
			MGFLOG(std::cout << "Console command not identified as PE command: " << c << std::endl;); 
			break;

		default:
			break;
	}

	if(cmdvec.size() == 3)
	{
		if(cmdvec[2]=="help")
		{
			return true;
		}
		else if(cmdvec[2]=="activate")
		{
			activate();
			return true;
		}
		else if(cmdvec[2]=="deactivate")
		{
			deactivate();
			return true;
		}

	}
	else if(cmdvec.size() == 4)
	{
		if(cmdvec[2]=="setuptimer")
		{
			int ms=MGFramework::toInt(cmdvec[3]);
			setupTimer(ms);
			return true;
		}
		else if(cmdvec[2]=="logging" && cmdvec[3]=="on")
		{
			enableLogging();
			MGFPRINT(std::cout << "Logging enabled." << std::endl;);
			return true;
		}
		else if(cmdvec[2]=="Logging" && cmdvec[3]=="off")
		{
			disableLogging();
			MGFPRINT(std::cout << "Logging disabled." << std::endl;);
			return true;
		}
	}

	MGFPRINT(std::cout << "Error in command (pe ...)" << std::endl;);

	return true;
}


