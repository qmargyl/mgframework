#ifndef _BOTWARSSERVER_H
#define _BOTWARSSERVER_H

#include "../mgframework/mgframework.h"

#define MAXNUMBEROFCLIENTS	8

struct BotWarsClientHandle{
	bool active;
	char ip[16];
	int port;
};

class BotWarsServer : public MGFramework
{
	private:
		BotWarsClientHandle clients[MAXNUMBEROFCLIENTS];

		virtual void handleGameLogics();
		virtual void draw();

	public:
		BotWarsServer();
		virtual bool init(int w, int h, int tw, int th);
};

#endif
