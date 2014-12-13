#ifndef _MG_SC_H
#define _MG_SC_H

#include <cstdio>

#define MGSC_CONNECTION_REQ		1
#define MGSC_CONNECTION_CFM		2

struct MGSCConnectionReq
{
	int mgfVersion;
};

struct MGSCConnectionCfm
{
	int mgfVersion;
};

struct MGSCMessage
{
	char sigNo;
	char length;
	char* data;

	MGSCMessage(char sigNumber)
	:	sigNo(sigNumber),
		length(sizeof(char*)),
		data(NULL)
	{
		switch(sigNo)
		{
			case MGSC_CONNECTION_REQ:
			{
				length = static_cast<char>(sizeof(MGSCConnectionReq));
				break;
			}
			case MGSC_CONNECTION_CFM:
			{
				length = static_cast<char>(sizeof(MGSCConnectionCfm));
				break;
			}
			default:
			{
				// Keep initialized values
				// Even for an unknown sigNo, data will be allocated.
			}
		}
		data = new char[length];
	}

	~MGSCMessage()
	{
		delete[] data;
	}
};

class MGSC
{

public:
	MGSC();
	static int startServerThread(void *fm);
	static int startClientThread(void *fm);
	void sendConnectionReq(int mgfVer);
};

#endif