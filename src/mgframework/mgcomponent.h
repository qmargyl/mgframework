#ifndef _MG_COMPONENT_H
#define _MG_COMPONENT_H

#include <vector>

enum eMGComponentConsoleCommand;

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