#ifndef _PROJECT_2_H
#define _PROJECT_2_H

#include "mgframework/mgperiodicevent.h"
#include "mgframework/mgframework.h"
#include "mgframework/mgmovingobject.h"

class Project2 : public MGFramework
{
	private:

		// These methods have to be implemented in a class derived from MGFramework.
		virtual void handleGameLogics();
		virtual void draw();

		// Graphics (which is not part of the framework) should be added here.
		SDL_Surface *m_Floor;
		SDL_Surface *m_MovingObject;

		// Data members needed for game logics etc..
		MGPeriodicEvent m_PE;

	public:
		Project2();
		virtual bool init(int w, int h, int tw, int th); // Has to be implemented in a class derived from MGFramework
};

#endif
