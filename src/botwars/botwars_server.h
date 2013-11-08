#ifndef _BOTWARSSERVER_H
#define _BOTWARSSERVER_H

#include "../mgframework/mgframework.h"

class BotWarsServer : public MGFramework
{
	private:

		virtual void handleGameLogics();
		virtual void draw();

	public:
		BotWarsServer();
		virtual bool init(int w, int h, int tw, int th);
};

#endif
