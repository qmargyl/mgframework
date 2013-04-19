#include "project2.h"
#include "mgframework/mgframework.h"

using std::string;

int main(int argc, char **argv)
{
	//string programTitle; // Used for window title..
	
	//Create a framework instance and set default configuration
	MGFramework* p2 = new Project2();
	p2->setInstanceType(MGFSINGLEPLAYERINSTANCE);
	p2->enableMiniMap();
	p2->disableLogging();

	if(argc==1){
		// No parameters were given
	}
	else{
		for(int i=1; i<argc; i++){
			if(strcmp(argv[i], "-logging")==0){
				p2->enableLogging();
			}
			else if(strcmp(argv[i], "-help")==0){
				std::cout << std::endl << argv[0] << " supports the following parameters:" << std::endl;
				std::cout << "-logging" << std::endl;
				std::cout << " Enables debug logging to console. -debug gives same effect." << std::endl;

				goto EXIT_MAIN_RIGHT_AWAY;
			}
			else if(strcmp(argv[i], "-server")==0){
				p2->setInstanceType(MGFSERVERINSTANCE);
			}
			else if(strcmp(argv[i], "-client")==0){
				p2->setInstanceType(MGFCLIENTINSTANCE);
			}
			else{
				// Unknown parameter.

			}
		}
	}


	// Initialize the framework.
	//programTitle = string("Project2 (example application) ") + string(p2->getProgramVersion());
	if(p2->getInstanceType()==MGFSINGLEPLAYERINSTANCE)
	{
		p2->setWindowProperties(1024, 768, 32, false, 
			string("Project2 (single player example application) ") + string(p2->getProgramVersion()));
	}
	else if(p2->getInstanceType()==MGFCLIENTINSTANCE)
	{
		p2->setWindowProperties(640, 480, 32, false, 
			string("Project2 (client example application) ") + string(p2->getProgramVersion()));
	}
	else if(p2->getInstanceType()==MGFSERVERINSTANCE)
	{
		p2->setWindowProperties(800, 600, 32, false, 
			string("Project2 (server example application) ") + string(p2->getProgramVersion()));
	}

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
