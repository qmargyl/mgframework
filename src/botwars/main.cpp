#include "botwars.h"
#include "botwars_server.h"
#include "../mgframework/mgframework.h"

using std::string;

int main(int argc, char **argv)
{
	BotWars* bw=0;
	BotWarsServer* bws=0;

	eMGFInstanceType instanceType = MGFSINGLEPLAYERINSTANCE;
	bool loggingOn = false;
	bool scriptFile = false;
	bool logEval = false;
	char scriptFileName[128] = "";
	char logEvalFileName[128] = "";
	int portN = 0;

	if(argc==1)
	{
		// No parameters were given
	}
	else
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp(argv[i], "-logging")==0)
			{
				loggingOn = true;
			}
			else if(strcmp(argv[i], "-help")==0)
			{
				std::cout << std::endl << argv[0] << " supports the following parameters:" << std::endl;
				std::cout << "-logging" << std::endl;
				std::cout << " Enables debug logging to console." << std::endl;

				goto EXIT_MAIN_RIGHT_AWAY;
			}
			else if(strcmp(argv[i], "-server")==0)
			{
				instanceType = MGFSERVERINSTANCE;
			}
			else if(strcmp(argv[i], "-client")==0)
			{
				instanceType = MGFCLIENTINSTANCE;
			}
			else if(strcmp(argv[i], "-script")==0)
			{
				if(i+1 == argc)
				{
					//No parameter after -script
					goto EXIT_MAIN_RIGHT_AWAY;
				}
				else
				{
					//Store argv[++i] as script file name..
					strcpy(scriptFileName, argv[++i]);
					scriptFile = true;
				}
			}
			else if(strcmp(argv[i], "-log_eval")==0)
			{
				if(i+1 == argc)
				{
					//No parameter after -log_eval
					goto EXIT_MAIN_RIGHT_AWAY;
				}
				else
				{
					//Store argv[++i] as script file name..
					strcpy(logEvalFileName, argv[++i]);
					logEval = true;
					instanceType = MGFSERVERINSTANCE;
				}
			}
			else if(strcmp(argv[i], "-port")==0)
			{
				if(i+1 == argc)
				{
					//No parameter after -port
					goto EXIT_MAIN_RIGHT_AWAY;
				}
				else
				{
					//Store argv[++i] as port number..
					char port[8];
					if(strlen(argv[++i])<8)
					{
						strcpy(port, argv[i]);
						portN = MGFramework::staticToInt(std::string(port));
					}
				}
			}
			else
			{
				// Unknown parameter.
			}
		}
	}

	// Create and initialize the framework...
	// Divide into separate executables later if needed.

	if(instanceType==MGFSINGLEPLAYERINSTANCE)
	{
		bw = new BotWars();
		bw->setPort(portN);
		if(loggingOn) bw->enableLogging();
		bw->setWindowProperties(MGWindow_RES_800_600, 32, false, 
			string("BotWars ") + string(BotWars::getBotWarsVersion()) + 
			string(" based on MGF ") + string(bw->getMGFrameworkVersion()));
		if(logEval)
		{
			bw->logEval(logEvalFileName);
		}
		else if(bw->windowPropertiesSet())
		{
			if(bw->init(64, 64, 32, 32))
			{
				if(scriptFile)
				{
					bw->run(scriptFileName);
				}
				else
				{
					bw->run(NULL);
				}
			}
		}
	}
	else if(instanceType==MGFCLIENTINSTANCE)
	{
		bw = new BotWars();
		bw->setPort(portN);
		if(loggingOn) bw->enableLogging();
		bw->setWindowProperties(640, 480, 32, false, 
			string("BotWars Client ") + string(BotWars::getBotWarsVersion()) + 
			string(" based on MGF ") + string(bw->getMGFrameworkVersion()));
		if(logEval)
		{
			bw->logEval(logEvalFileName);
		}
		else if(bw->windowPropertiesSet())
		{
			if(bw->init(64, 64, 32, 32))
			{
				if(scriptFile)
				{
					bw->run(scriptFileName);
				}
				else
				{
					bw->run(NULL);
				}
			}
		}
	}
	else if(instanceType==MGFSERVERINSTANCE)
	{
		bws = new BotWarsServer();
		bws->setPort(portN);
		if(loggingOn) bws->enableLogging();
		bws->setWindowProperties(800, 600, 32, false, 
			string("BotWars Server ") + string(BotWars::getBotWarsVersion()) + 
			string(" based on MGF ") + string(bws->getMGFrameworkVersion()));
		if(logEval)
		{
			bws->logEval(logEvalFileName);
		}
		else if(bws->windowPropertiesSet())
		{
			if(bws->init(64, 64, 32, 32))
			{
				if(scriptFile)
				{
					bws->run(scriptFileName);
				}
				else
				{
					bws->run(NULL);
				}
			}
		}
	}

EXIT_MAIN_RIGHT_AWAY:

	if(bw) delete bw;
	if(bws) delete bws;
		
	return 0;
};
