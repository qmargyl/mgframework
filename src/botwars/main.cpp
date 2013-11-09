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
			else
			{
				// Unknown parameter.
			}
		}
	}


	// Create and initialize the framework...

	if(instanceType==MGFSINGLEPLAYERINSTANCE)
	{
		bw = new BotWars();
		bw->setWindowProperties(MGWindow_RES_800_600, 32, false, 
			string("BotWars ") + string(BotWars::getBotWarsVersion()) + 
			string(" based on MGF ") + string(bw->getMGFrameworkVersion()));
	}
	else if(instanceType==MGFCLIENTINSTANCE)
	{
		// Add separate class later...
		bw = new BotWars();
		bw->setWindowProperties(640, 480, 32, false, 
			string("BotWars Client ") + string(BotWars::getBotWarsVersion()) + 
			string(" based on MGF ") + string(bw->getMGFrameworkVersion()));
	}
	else if(instanceType==MGFSERVERINSTANCE)
	{
		bws = new BotWarsServer();
		bws->setWindowProperties(800, 600, 32, false, 
			string("BotWars Server ") + string(BotWars::getBotWarsVersion()) + 
			string(" based on MGF ") + string(bws->getMGFrameworkVersion()));
	}


	if(instanceType==MGFSERVERINSTANCE)
	{
		if(loggingOn) bws->enableLogging();

		if(logEval)
		{
			// In case of using the framework for log evaluation, don't init and run.
			bws->logEval(logEvalFileName);
		}
		else if(bw->windowPropertiesSet())
		{
			// If initialization is ok, run the framework...
			if(bws->init(128, 128, 32, 32))
			{
				bws->run(scriptFileName);
			}
		}
	}
	else
	{
		if(loggingOn) bw->enableLogging();

		if(logEval)
		{
			// In case of using the framework for log evaluation, don't init and run.
			bw->logEval(logEvalFileName);
		}
		else if(bw->windowPropertiesSet())
		{
			// If initialization is ok, run the framework...
			if(bw->init(128, 128, 32, 32))
			{
				bw->run(scriptFileName);
			}
		}
	}

EXIT_MAIN_RIGHT_AWAY:

	if(bw) delete bw;
	if(bws) delete bws;
		
	return 0;
};
