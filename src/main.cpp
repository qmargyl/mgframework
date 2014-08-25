#ifndef UNITTEST_LINUX
#include "project2.h"
#include "project2_server.h"
#endif
#include "mgframework/mgframework.h"
#include "mgframework/mgclasstester.h"
#include <string>

int main(int argc, char **argv)
{
	MGFramework* p2 = 0;

	eMGFInstanceType instanceType = MGFSINGLEPLAYERINSTANCE;
	bool loggingOn = false;
	bool logEvalNegative = false;
	bool noRandom = false;
	bool classTest = false;
	std::string scriptFileName;
	std::string logEvalFileName;
	std::string logCompareFileName;
	std::string classTestParam;

	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			if(std::string(argv[i]) == std::string("-logging"))
			{
				loggingOn = true;
			}
			else if(std::string(argv[i]) == std::string("-server"))
			{
				instanceType = MGFSERVERINSTANCE;
			}
			else if(std::string(argv[i]) == std::string("-client"))
			{
				instanceType = MGFCLIENTINSTANCE;
			}
			else if(std::string(argv[i]) == std::string("-script"))
			{
				if(i + 1 == argc)
				{
					goto EXIT_MAIN_RIGHT_AWAY;
				}
				else
				{
					scriptFileName = std::string(argv[++i]);
				}
			}
			else if(std::string(argv[i]) == std::string("-log_eval"))
			{
				if(i + 1 == argc)
				{
					goto EXIT_MAIN_RIGHT_AWAY;
				}
				else
				{
					logEvalFileName = std::string(argv[++i]);
					instanceType = MGFSERVERINSTANCE;
				}
			}
			else if(std::string(argv[i]) == std::string("-log_eval_negative"))
			{
				if(i + 1 == argc)
				{
					goto EXIT_MAIN_RIGHT_AWAY;
				}
				else
				{
					logEvalFileName = std::string(argv[++i]);
					logEvalNegative = true;
					instanceType = MGFSERVERINSTANCE;
				}
			}
			else if(std::string(argv[i]) == std::string("-log_compare"))
			{
				if(i + 1 == argc)
				{
					goto EXIT_MAIN_RIGHT_AWAY;
				}
				else
				{
					logCompareFileName = std::string(argv[++i]);
					instanceType = MGFSERVERINSTANCE;
				}
			}
			else if(std::string(argv[i]) == std::string("-no_random"))
			{
				noRandom = true;
			}
			else if(std::string(argv[i]) == std::string("-classtest"))
			{
				if(i + 1 == argc)
				{
					classTestParam = std::string("all");
				}
				else
				{
					// NOT IMPLEMENTED YET
					classTestParam = std::string(argv[++i]);
				}
				classTest = true;
			}
		}
	}


	if(classTest)
	{
		// Run configured class tests and then exit
		MGClassTester::runAll();
		goto EXIT_MAIN_RIGHT_AWAY;
	}

#ifndef UNITTEST_LINUX
	// Create and initialize the framework...
	switch(instanceType)
	{
		case MGFSINGLEPLAYERINSTANCE:
		{
			p2 = new Project2();
			p2->setWindowProperties(MGWindow_RES_800_600, 32, false, 
				std::string("Project2 (single player test application) based on MGF ") + 
							std::string(p2->getMGFrameworkVersion()));
			break;
		}
		case MGFCLIENTINSTANCE:
		{
			p2 = new Project2();
			p2->setWindowProperties(640, 480, 32, false, 
				std::string("Project2 (client test application) based on MGF ") + 
							std::string(p2->getMGFrameworkVersion()));
			break;
		}
		case MGFSERVERINSTANCE:
		{
			p2 = new Project2Server();
			p2->setWindowProperties(800, 600, 32, false, 
				std::string("Project2 (server test application) based on MGF ") + 
							std::string(p2->getMGFrameworkVersion()));
			break;
		}
	}

	if(loggingOn) p2->enableLogging();

#endif

	if(logEvalFileName != std::string(""))
	{
		// In case of using the framework for log evaluation, don't init and run.
		// Create filtered logs first, then run evaluation..
		MGClassTester::logFilter(logEvalFileName.c_str());
		MGClassTester::logEval(logEvalFileName.c_str(), logEvalNegative);
		if(logCompareFileName != std::string(""))
		{
			// Compares logEvalFileName.filtered to logCompareFileName 
			MGClassTester::logCompare(logEvalFileName.c_str(), logCompareFileName.c_str());
		}
	}
#ifndef UNITTEST_LINUX
	else if(p2->windowPropertiesSet())
	{
		// If initialization is ok, run the framework...
		if(p2->init(128, 128, 32, 32))
		{
			if(scriptFileName != std::string(""))
			{
				if(noRandom)
				{
					p2->randomize(666);
				}
				p2->run(scriptFileName.c_str());
			}
			else
			{
				p2->run(NULL);
			}
		}
		else
		{

		}
	}
#endif
	else 
	{

	}

EXIT_MAIN_RIGHT_AWAY:
	delete p2;	
	return 0;
}
