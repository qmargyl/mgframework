#ifndef _PROJECT_2_H
#define _PROJECT_2_H

#include "mgframework/mgframework.h"

class Project2 : public MGFramework
{
	private:
		virtual void handleGameLogics(); // MGFramework
		virtual void draw(); // MGFramework

		// Graphics (which is not part of the framework) should be added here.
		SDL_Texture *m_Floor;
		SDL_Texture *m_MovingObject;
		SDL_Texture *m_StationaryObject;
		SDL_Texture *m_Mark;
		// TODO: Remove the SDL dependency:
		//       MGWindow::MGTextureHandle m_Floor;

	public:
		Project2();
		virtual bool init(int w, int h, int tw, int th); // MGFramework
};

#endif
