#ifndef _PROJECT_2_SERVER_H
#define _PROJECT_2_SERVER_H

#include "mgframework/mgframework.h"

class Project2Server : public MGFramework
{
	private:

		virtual void handleGameLogics();
		virtual void draw();

	public:
		Project2Server();
		virtual bool init(int w, int h, int tw, int th);
};

#endif
