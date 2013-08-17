#ifndef _MG_PERIODICEVENT_H
#define _MG_PERIODICEVENT_H
#include "mgcomponent.h"




class MGPeriodicEvent :public MGComponent
{
private:
	int m_Period;
	int m_StartTime;
	bool m_isActive;

public:
	MGPeriodicEvent();
	void setupTimer(int ms);	// Allows configuration of the period of the event
	bool update();				// To be called from the smallest game cycle. Returns true if it has triggered, otherwise false.
	void activate();
	void deactivate();
	bool isActive();
	int timeLeft();

    void copy(const MGPeriodicEvent *src);

	bool runConsoleCommand(const char *c, MGFramework *w);
	eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);

};




#endif