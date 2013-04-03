#include "project2.h"
#include "mgframework/mgframework.h"

using std::string;

int main(int argc, char **argv)
{
	string programTitle; // Used for window title..
	
	//Create a framework instance and set default configuration
	MGFramework* p2 = new Project2();

	if(argc==1){
		// No parameters were given
	}
	else{
		for(int i=1; i<argc; i++){
			if(strcmp(argv[i], "-debug")==0 || strcmp(argv[i], "-logging")==0){
				p2->enableLogging();
			}
			else if(strcmp(argv[i], "-help")==0){
				std::cout << std::endl << argv[0] << " supports the following parameters:" << std::endl;
				std::cout << "-debug" << std::endl;
				std::cout << " Enables debug logging to console. -logging gives same effect." << std::endl;
				std::cout << "-logging" << std::endl;
				std::cout << " Enables debug logging to console. -debug gives same effect." << std::endl;
				std::cout << "-minimap" << std::endl;
				std::cout << " Enables the mini map." << std::endl;
				std::cout << "-nominimap" << std::endl;
				std::cout << " Disables the mini map." << std::endl;
				std::cout << "-version" << std::endl;
				std::cout << " Prints program version to console." << std::endl;

				goto EXIT_MAIN_RIGHT_AWAY;
			}
			else if(strcmp(argv[i], "-minimap")==0){
				p2->enableMiniMap();
			}
			else if(strcmp(argv[i], "-nominimap")==0){
				p2->disableMiniMap();
			}
			else if(strcmp(argv[i], "-version")==0){
				std::cout << "Project2 (example application) " << p2->getProgramVersion() << std::endl;
				std::cout << "Based on MG Framework " << MGFRAMEWORKVERSION << " by Martin Gyllensten" << std::endl;
			}
			else{
				// Unknown parameter.

			}
		}
	}


	// Initialize the framework.
	programTitle = string("Project2 (example application) ") + string(p2->getProgramVersion());
	p2->setWindowProperties(1024, 768, 32, false, programTitle);

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
	
	//If we can't initialize, or the framework has quit, we delete the instance and return.

EXIT_MAIN_RIGHT_AWAY:

	delete p2;
		
	return 0;
};
