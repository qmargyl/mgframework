#ifndef _BOTWARS_H
#define _BOTWARS_H

#include "../mgframework/mgframework.h"

#define BOTWARSVERSION "0.0.0.2"

class BotWars : public MGFramework
{
	private:

		// These methods have to be implemented in a class derived from MGFramework.
		virtual void handleGameLogics();
		virtual void draw();

		// Graphics (which is not part of the framework) should be added here.
		SDL_Surface *m_Floor;
		SDL_Surface *m_MovingObject;
		SDL_Surface *m_StationaryObject;
		SDL_Surface *m_Mark;

	public:
		BotWars();
		virtual bool init(int w, int h, int tw, int th); // Has to be implemented in a class derived from MGFramework

		static const char *getBotWarsVersion(){return BOTWARSVERSION;}
};

#endif
