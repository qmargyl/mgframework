#ifndef _BOTWARS_H
#define _BOTWARS_H

#include "../mgframework/mgframework.h"

#define BOTWARSVERSION "0.0.0.3"

class BotWars : public MGFramework
{
	private:

		// These methods have to be implemented in a class derived from MGFramework.
		virtual void handleGameLogics();
		virtual void draw();

		// Graphics
		SDL_Surface *m_Floor;
		SDL_Surface *m_MovingObject;
		SDL_Surface *m_StationaryObject;
		SDL_Surface *m_Mark;

		int m_NBots;
		void addBots(int n, unsigned int team, const char* script);


	protected:

		bool processEvents();

	public:

		BotWars();
		virtual bool init(int w, int h, int tw, int th); // Has to be implemented in a class derived from MGFramework

		static const char *getBotWarsVersion(){return BOTWARSVERSION;}
};

#endif
