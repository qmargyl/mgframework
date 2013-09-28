#include "project2.h"
#include "project2_server.h"
#include "mgframework/mgframework.h"

using std::string;

int main(int argc, char **argv)
{
	MGFramework* p2;

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
		p2 = new Project2();
		p2->setWindowProperties(MGWindow_RES_800_600, 32, false, 
			string("Project2 (single player example application) based on MGF ") + string(p2->getMGFrameworkVersion()));
	}
	else if(instanceType==MGFCLIENTINSTANCE)
	{
		// Add separate class later...
		p2 = new Project2();
		p2->setWindowProperties(640, 480, 32, false, 
			string("Project2 (client example application) based on MGF ") + string(p2->getMGFrameworkVersion()));
	}
	else if(instanceType==MGFSERVERINSTANCE)
	{
		p2 = new Project2Server();
		p2->setWindowProperties(800, 600, 32, false, 
			string("Project2 (server example application) based on MGF ") + string(p2->getMGFrameworkVersion()));
	}

	if(loggingOn) p2->enableLogging();

	if(logEval)
	{
		// In case of using the framework for log evaluation, don't init and run.
		p2->logEval(logEvalFileName);
	}
	else if(p2->windowPropertiesSet())
	{
		// If initialization is ok, run the framework...
		if(p2->init(128, 128, 32, 32))
		{
			p2->run(scriptFileName);
		}
		else
		{

		}
	}
	else 
	{

	}

EXIT_MAIN_RIGHT_AWAY:

	delete p2;
		
	return 0;
};
