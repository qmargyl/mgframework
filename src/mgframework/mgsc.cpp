#include <cstring>
#include "mgsc.h"
#include "mgframework.h"
#include <winsock.h>

int MGSC::startServerThread(void *fm)
{
#ifndef MGF_DISABLE_WINSOCK
	MGFramework *mgf = (MGFramework *)fm;
	int PORTNR = mgf->getPort();
	std::cout << "Opening socket terminal, port=" << mgf->getPort() << std::endl;
	
	bool connectionOpen = true;
	int nZerosInARow = 0;

	while(mgf->socketTerminalOpen())
	{
		if(!mgf->initializeWinsock(MAKEWORD(1, 1))) 
		{
			std::cout << "ERROR: Initializing Winsock failed" << std::endl;
			return 1;
		}

		SOCKET fd, fd_new; 	// "file" descriptors for the network sockets
		SOCKADDR_IN saddr_me;

		if((fd = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
		{
			std::cout << "ERROR: Server: Socket not connected" << std::endl;
			return 1;
		}

		saddr_me.sin_family = AF_INET;
		saddr_me.sin_addr.s_addr = htonl(INADDR_ANY);
		saddr_me.sin_port = htons(PORTNR);

		if(bind(fd, (LPSOCKADDR) &saddr_me, sizeof(saddr_me)) == SOCKET_ERROR)
		{
			std::cout << "ERROR: Server: Bind failure" << std::endl;
			return 1;
		}

		if(listen(fd, 1) == SOCKET_ERROR)
		{
			std::cout << "ERROR: Server: Listen failure";
			return 1;
		}

		if((fd_new = accept(fd, NULL, NULL)) == INVALID_SOCKET)
		{
			std::cout << "ERROR: Server: Accept failure" << std::endl;
			return 1;
		}

		while(connectionOpen)
		{
			char buf[256];
			for (int i = 0; i < 256; i++)
			{
				buf[i] = 0;
			}
			if(recv(fd_new, buf, sizeof(buf), 0) == SOCKET_ERROR)
			{
				std::cout << "ERROR: Server: Receive failure" << std::endl;
				return 1;
			}

			// Now buf contains the request string.

			int lBuf = (int)strlen(buf);
			if(lBuf == 0)
			{
				nZerosInARow++;
			}
			else
			{
				nZerosInARow = 0;
				mgf->runConsoleCommand(buf, mgf, NULL);
				if(send(fd_new, "ok\n\r", 4, 0) == SOCKET_ERROR)
				{
					std::cout << "ERROR: Server: Failed sending an answer to client request" << std::endl;
					return 1;
				}
			}
			if(nZerosInARow>4)
			{
				connectionOpen = false;
			}
		}
		WSACleanup();
	}
	std::cout << "Socket terminal closed..." << std::endl;
#endif
	return 0;
}

int MGSC::startClientThread(void *fm)
{
#ifndef MGF_DISABLE_WINSOCK
#endif
	return 0;
}

void MGSC::sendConnectionReq(int mgfVer)
{
	MGSCConnectionReq payload;
	payload.mgfVersion = mgfVer;
	MGSCMessage* msg = new MGSCMessage(MGSC_CONNECTION_REQ);
	memcpy(msg->data, &payload, sizeof(MGSCConnectionReq));
	//send(getServerIP(), getServerPort(), msg);
	delete msg;
}
