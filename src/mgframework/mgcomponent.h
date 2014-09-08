#ifndef _MG_COMPONENT_H
#define _MG_COMPONENT_H

#include <time.h>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

// Disable features by #define
//#define MGF_DISABLE_TTF
#define MGF_DISABLE_WINSOCK

// Macros...
#define MGF_TRUE 1
#define MGF_FALSE 0

// These can be set to disable warning, info and error traces on a global level 
//#define MGFLOG_WARNING(x)
//#define MGFLOG_INFO(x)
//#define MGFLOG_ERROR(x)
//#define MGFLOG_STATIC_WARNING(x)
//#define MGFLOG_STATIC_INFO(x)
//#define MGFLOG_STATIC_ERROR(x)

#ifndef MGFLOG_WARNING
#define MGFLOG_WARNING(x)								{ float t = MGF_GetExecTimeS(); std::cout << "[" << std::setfill('0') << std::setw(8) << t << "] " << __FILE__ << ":" << __LINE__ << " (ID:" << getID() << ") WARNING: " << x << std::endl; }
#endif
#ifndef MGFLOG_INFO
#define MGFLOG_INFO(x)				if(loggingEnabled()){ float t = MGF_GetExecTimeS(); std::cout << "[" << std::setfill('0') << std::setw(8) << t << "] " << __FILE__ << ":" << __LINE__ << " (ID:" << getID() << ") INFO: " << x << std::endl; }
#endif
#ifndef MGFLOG_ERROR
#define MGFLOG_ERROR(x)									{ float t = MGF_GetExecTimeS(); std::cout << "[" << std::setfill('0') << std::setw(8) << t << "] " << __FILE__ << ":" << __LINE__ << " (ID:" << getID() << ") ERROR: " << x << std::endl; }
#endif

#ifndef MGFLOG_STATIC_WARNING
#define MGFLOG_STATIC_WARNING(x)						{ float t = MGF_GetExecTimeS(); std::cout << "[" << std::setfill('0') << std::setw(8) << t << "] " << __FILE__ << ":" << __LINE__ << " WARNING: " << x << std::endl; }
#endif
#ifndef MGFLOG_STATIC_INFO
#define MGFLOG_STATIC_INFO(x)		if(loggingEnabled()){ float t = MGF_GetExecTimeS(); std::cout << "[" << std::setfill('0') << std::setw(8) << t << "] " << __FILE__ << ":" << __LINE__ << " INFO: " << x << std::endl; }
#endif
#ifndef MGFLOG_STATIC_ERROR
#define MGFLOG_STATIC_ERROR(x)							{ float t = MGF_GetExecTimeS(); std::cout << "[" << std::setfill('0') << std::setw(8) << t << "] " << __FILE__ << ":" << __LINE__ << " ERROR: " << x << std::endl; }
#endif

typedef unsigned short      WORD;
#ifndef DWORD
typedef unsigned long int   DWORD;
#endif

#ifndef UNITTEST_LINUX
enum eMGComponentConsoleCommand;
#else
enum eMGComponentConsoleCommand : unsigned int;
#endif

enum eMGFInstanceType{
	MGFSERVERINSTANCE = 0,
	MGFCLIENTINSTANCE,
	MGFSINGLEPLAYERINSTANCE
};

#define MGF_NOPLAYER  0
#define MGF_PLAYER1   1
#define MGF_PLAYER2   2
#define MGF_PLAYER3   3
#define MGF_PLAYER4   4
#define MGF_PLAYER5   5
#define MGF_PLAYER6   6
#define MGF_PLAYER7   7
#define MGF_PLAYER8   8

class MGFramework;
class MGSymbolTable;

class MGComponent
{
private:
	static int m_IDGenerator;
	static size_t m_allocatedMemory;

	// Methods for setting the object ID..
	void setID(int id){ m_ID = id; }
	int generateID(){ return m_IDGenerator++; }

protected:

	int m_ID;
	unsigned int m_Owner;
	bool m_LoggingEnabled;		// Setting allows extended logging to console.

	void setID()
	{
		setID(generateID());
	}

	virtual eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) = 0;

public:
	MGComponent()
		: m_ID(0),
		  m_Owner(MGF_NOPLAYER),
		  m_LoggingEnabled(false)
	{
		setID();
	}

	virtual bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s) = 0;

	int getID(){ return m_ID;}

	// Configuration of Logging...
	void enableLogging(){ m_LoggingEnabled = true; }
	void disableLogging(){ m_LoggingEnabled = false; }
	bool loggingEnabled(){ return m_LoggingEnabled; }

	// Component owner...
	void setOwner(unsigned int owner){ m_Owner = owner; }
	unsigned int getOwner(){ return m_Owner; }
	bool isOwned(){ return m_Owner != MGF_NOPLAYER; }

	// Utility functions
	static std::string toString(bool b){ if(b) return std::string("true"); return std::string("false"); }
	static int randomN(int upperLimit){ if(upperLimit == 0) return 0; return std::rand() % upperLimit; }
	static void randomize(int seed){ std::srand(seed); }
	static double distance(int x1, int y1, int x2, int y2){ return sqrt((double)(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)))); }
	static std::string toString(int number);
	static std::vector<std::string> split(char *str, const char *c);
	static std::vector<std::string> symbols(char *str);
	static bool oneOf(int x, int a1, int a2){ if(x == a1) return true; if(x == a2) return true; return false; }
	static bool detectCollisionRectangle(int x1, int y1, int x2, int y2, int a1, int b1, int a2, int b2);
	static bool detectCollisionPointRectangle(int px, int py, int x1, int y1, int x2, int y2);
	static float MGF_GetExecTimeS(){ return ((float)clock()) / CLOCKS_PER_SEC; }
	static unsigned int MGF_GetExecTimeMS(){ return (unsigned int)(1000.0 * MGF_GetExecTimeS()); }
};

#endif
