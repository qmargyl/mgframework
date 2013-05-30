#include "project2.h"
#include "project2_server.h"
#include "mgframework/mgframework.h"

using std::string;

int main(int argc, char **argv)
{
	MGFramework* p2;

	eMGFInstanceType instanceType = MGFSINGLEPLAYERINSTANCE;
	bool loggingOn = false;

	if(argc==1){
		// No parameters were given
	}
	else{
		for(int i=1; i<argc; i++){
			if(strcmp(argv[i], "-logging")==0){
				loggingOn = true;
			}
			else if(strcmp(argv[i], "-help")==0){
				std::cout << std::endl << argv[0] << " supports the following parameters:" << std::endl;
				std::cout << "-logging" << std::endl;
				std::cout << " Enables debug logging to console. -debug gives same effect." << std::endl;

				goto EXIT_MAIN_RIGHT_AWAY;
			}
			else if(strcmp(argv[i], "-server")==0){
				instanceType = MGFSERVERINSTANCE;
			}
			else if(strcmp(argv[i], "-client")==0){
				instanceType = MGFCLIENTINSTANCE;
			}
			else{
				// Unknown parameter.
			}
		}
	}


	// Create and initialize the framework...

	if(instanceType==MGFSINGLEPLAYERINSTANCE)
	{
		p2 = new Project2();
		p2->setWindowProperties(1024, 768, 32, false, 
			string("Project2 (single player example application) ") + string(p2->getProgramVersion()));
	}
	else if(instanceType==MGFCLIENTINSTANCE)
	{
		// Add separate class later...
		p2 = new Project2();
		p2->setWindowProperties(640, 480, 32, false, 
			string("Project2 (client example application) ") + string(p2->getProgramVersion()));
	}
	else if(instanceType==MGFSERVERINSTANCE)
	{
		p2 = new Project2Server();
		p2->setWindowProperties(800, 600, 32, false, 
			string("Project2 (server example application) ") + string(p2->getProgramVersion()));
	}

	if(loggingOn) p2->enableLogging();

	if(p2->windowPropertiesSet())
	{
		// If initialization is ok, run the framework...
		if(p2->init(50, 50, 32, 32))
		{
			p2->run();
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
