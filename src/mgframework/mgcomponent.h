#ifndef _MG_COMPONENT_H
#define _MG_COMPONENT_H

#include <vector>

typedef unsigned short      WORD;

enum eMGComponentConsoleCommand;

enum eMGFInstanceType{
	MGFSERVERINSTANCE = 0,
	MGFCLIENTINSTANCE,
	MGFSINGLEPLAYERINSTANCE
};

#define MGF_NOPLAYER	0
#define MGF_PLAYER1		1
#define MGF_PLAYER2		2

class MGFramework;

class MGComponent
{
private:

	static int m_IDGenerator;
	unsigned int m_Owner;
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
		: m_LoggingEnabled (false),
		  m_Owner (MGF_NOPLAYER)
	{
		setID();
//		disableLogging();
//		m_Owner = MGF_NOPLAYER;
	}

	// Force all classes inheriting MGComponent to take console commands. 
	virtual bool runConsoleCommand(const char *c, MGFramework *w) = 0;

	int getID(){ return m_ID;}

	// Configuration of Logging...
	void enableLogging(){m_LoggingEnabled = true;}
	void disableLogging(){m_LoggingEnabled = false;}
	bool loggingEnabled(){return m_LoggingEnabled;}

	// Component owner...
	void setOwner(unsigned int owner){m_Owner = owner;}
	unsigned int getOwner(){return m_Owner;}
	bool isOwned(){return m_Owner != MGF_NOPLAYER;}

};

#endif