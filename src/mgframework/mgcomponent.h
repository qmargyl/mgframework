#ifndef _MG_COMPONENT_H
#define _MG_COMPONENT_H

#include <vector>


enum eMGComponentConsoleCommand;

/*
enum eMGComponentConsoleCommand{
	MGComponent_UNDEFINED = 0,

	//Framework commands
	MGComponent_HELP,
	MGComponent_EXIT,
	MGComponent_LOGGING,
	MGComponent_GETFPS,
	MGComponent_GETNUMBEROFMARKEDMO,
	MGComponent_GETNUMBEROFMO,
	MGComponent_CREATE_MO_INT,
	MGComponent_ADD_MO_INT,
	MGComponent_CREATE_PE_INT,
	MGComponent_ADD_PE_INT,
	MGComponent_RUNFRAMES_INT,
	MGComponent_SETFPS_INT,
	MGComponent_OPEN_TERMINALSERVER,
	MGComponent_CLOSE_TERMINALSERVER,
	MGComponent_MINIMAP_ON,
	MGComponent_MINIMAP_OFF,
	MGComponent_LOGGING_ON,
	MGComponent_LOGGING_OFF,

	//Periodic Event commands
	MGComponent_PE_ALL_X,
	MGComponent_PE_INT_HELP,
	MGComponent_PE_INT_ACTIVATE,
	MGComponent_PE_INT_DEACTIVATE,
	MGComponent_PE_INT_SETUPTIMER_INT,
	MGComponent_PE_INT_LOGGING_ON,
	MGComponent_PE_INT_LOGGING_OFF
};
*/

class MGFramework;

class MGComponent
{
private:
	static int m_IDGenerator;
	bool m_LoggingEnabled;		// Setting allows extended logging to console.

	void setID(int id){ m_ID=id;}
	int generateID(){ return m_IDGenerator++;}

protected:
	int m_ID;

	void setID()
	{
		setID(generateID());
	}

	virtual eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) = 0;

public:
	MGComponent()
	{
		setID();
		disableLogging();
	}

	// Force all classes inheriting MGComponent to take console commands. 
	virtual bool runConsoleCommand(const char *c, MGFramework *w) = 0;

	int getID(){ return m_ID;}

	// Configuration of Logging...
	void enableLogging(){m_LoggingEnabled = true;}
	void disableLogging(){m_LoggingEnabled = false;}
	bool loggingEnabled(){return m_LoggingEnabled;}
};

#endif