#ifndef _PROJECT_2_H
#define _PROJECT_2_H

#include "mgframework/mgframework.h"

class Project2 : public MGFramework
{
	private:
		virtual void handleGameLogics(); // MGFramework
		virtual void draw(); // MGFramework

		// Application specific graphics
		MGTexHandle m_Floor;
		MGTexHandle m_MovingObject;
		MGTexHandle m_StationaryObject;
		MGTexHandle m_Mark;

	public:
		Project2();
		virtual bool init(int w, int h, int tw, int th); // MGFramework
};

#endif
