#ifndef _MG_COMPONENT_H
#define _MG_COMPONENT_H

#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <SDL/SDL.h>


// Macros...
#define MGF_TRUE 1
#define MGF_FALSE 0
#define MGFTIMESTAMP(x) std::setfill('0') << std::setw(2) << (x/3600000) << ":" << std::setfill('0') << std::setw(2) << (x%3600000)/60000 << ":" << std::setfill('0') << std::setw(2) << (x%60000)/1000 << ":" << std::setfill('0') << std::setw(3) << (x%1000) 

#define MGFLOG_WARNING(x)						{ Uint32 t = SDL_GetTicks(); std::cout << "[" << MGFTIMESTAMP(t) << "] " << __FILE__ << ":" << __LINE__ << " (ID:" << getID() << ") WARNING: "	<< x << std::endl; }
#define MGFLOG_INFO(x)		if(loggingEnabled()){ Uint32 t = SDL_GetTicks(); std::cout << "[" << MGFTIMESTAMP(t) << "] " << __FILE__ << ":" << __LINE__ << " (ID:" << getID() << ") INFO: "		<< x << std::endl; }
#define MGFLOG_ERROR(x)							{ Uint32 t = SDL_GetTicks(); std::cout << "[" << MGFTIMESTAMP(t) << "] " << __FILE__ << ":" << __LINE__ << " (ID:" << getID() << ") ERROR: "	<< x << std::endl; }

#define MGFLOG_STATIC_WARNING(x)						{ Uint32 t = SDL_GetTicks(); std::cout << "[" << MGFTIMESTAMP(t) << "] " << __FILE__ << ":" << __LINE__ << " WARNING: "	<< x << std::endl; }
#define MGFLOG_STATIC_INFO(x)		if(loggingEnabled()){ Uint32 t = SDL_GetTicks(); std::cout << "[" << MGFTIMESTAMP(t) << "] " << __FILE__ << ":" << __LINE__ << " INFO: "		<< x << std::endl; }
#define MGFLOG_STATIC_ERROR(x)							{ Uint32 t = SDL_GetTicks(); std::cout << "[" << MGFTIMESTAMP(t) << "] " << __FILE__ << ":" << __LINE__ << " ERROR: "	<< x << std::endl; }

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
	static size_t m_allocatedMemory;

	// Methods for setting the object ID..
	void setID(int id){ m_ID=id;}
	int generateID(){ return m_IDGenerator++;}

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
		: m_LoggingEnabled (false),
		  m_Owner (MGF_NOPLAYER)
	{
		setID();
	}

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


	// Utility functions
	static std::string toString(bool b){ if(b) return std::string("true"); return std::string("false"); }
	static int randomN(int upperLimit){return std::rand()%upperLimit;}
	static double distance(int x1, int y1, int x2, int y2){ return sqrt((double)(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)))); }
	static std::string toString(int number);
	static std::vector<std::string> split(std::string str, char c);
	static bool oneOf(int x, int a1, int a2){ if(x==a1) return true;  if(x==a2) return true; return false;}
	static int smallest(int a, int b){ if(a<b) return a; return b;}
	static int smallest(int a, int b, int c){ return smallest(a, smallest(b, c)); }
	static int smallest(int a, int b, int c, int d){ return smallest(smallest(a, b), smallest(c, d)); }
	static int smallest(int a, int b, int c, int d, int e){ return smallest(smallest(a, b), smallest(c, d, e)); }
	static int smallest(int a, int b, int c, int d, int e, int f){ return smallest(smallest(a, b, c), smallest(d, e, f)); }
	static int smallest(int a, int b, int c, int d, int e, int f, int g){ return smallest(smallest(a, b, c), smallest(d, e, f, g)); }
	static int smallest(int a, int b, int c, int d, int e, int f, int g, int h){ return smallest(smallest(a, b, c, d), smallest(e, f, g, h)); }
	static int biggest(int a, int b){ if(a>b) return a; return b;}
	static bool detectCollisionRectangle(int x1, int y1, int x2, int y2, int a1, int b1, int a2, int b2);
	static bool detectCollisionPointRectangle(int px, int py, int x1, int y1, int x2, int y2);

};

#endif