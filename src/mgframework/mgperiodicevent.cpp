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
	std::cout << "MGPeriodicEvent::runConsoleCommand(" << c << ")" << std::endl;

	std::string cmd(c);
	std::vector<std::string> cmdvec = MGFramework::split(cmd, ' ');

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
	}

	std::cout << "Error in command (pe ...)" << std::endl;


	return true;
}