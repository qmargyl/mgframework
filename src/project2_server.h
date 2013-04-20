#ifndef _PROJECT_2_SERVER_H
#define _PROJECT_2_SERVER_H

#include "mgframework/mgframework.h"

class Project2Server : public MGFramework
{
	private:

		// These methods have to be implemented in a class derived from MGFramework.
		virtual void handleGameLogics();
		virtual void draw();

		// Graphics (which is not part of the framework) should be added here.
		SDL_Surface *m_Floor;
		SDL_Surface *m_MovingObject;
		SDL_Surface *m_Mark;

	public:
		Project2Server();
		virtual bool init(int w, int h, int tw, int th); // Has to be implemented in a class derived from MGFramework
};

#endif
