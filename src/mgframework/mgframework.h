#ifndef _MG_FRAMEWORK_H
#define _MG_FRAMEWORK_H

#include "mgwindow.h"
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include "mgmap.h"
#include "SDL_ttf.h"
#include <vector>
#include <string>
#include "mgcomponent.h"
#include "mgmovingobject.h"
#include "mgperiodicevent.h"


// Version format is <major release>.<minor release>.<features added>.<bug fixes>
#define MGFRAMEWORKVERSION "1.0.21.0"

// Configurable defines...
#define MGF_SCRIPTLINE_MAXLENGTH	256
#define MGF_LOGLINE_MAXLENGTH		1024
#define MGF_DEBUGGING_ENABLED


// Macros...
#define MGFTIMESTAMP(x) (x/3600000) << ":" << (x%3600000)/60000 << ":" << (x%60000)/1000 << ":" << (x%1000) 

#define MGFLOG_WARNING(x)	if(loggingEnabled()){ Uint32 t = SDL_GetTicks(); std::cout << "[" << MGFTIMESTAMP(t) << "] " << __FILE__ << ":" << __LINE__ << " (ID:" << getID() << ") WARNING: ";  x; }
#define MGFLOG_INFO(x)		if(loggingEnabled()){ Uint32 t = SDL_GetTicks(); std::cout << "[" << MGFTIMESTAMP(t) << "] " << __FILE__ << ":" << __LINE__ << " (ID:" << getID() << ") INFO: ";  x; }
#define MGFLOG_ERROR(x)							{ Uint32 t = SDL_GetTicks(); std::cout << "[" << MGFTIMESTAMP(t) << "] " << __FILE__ << ":" << __LINE__ << " (ID:" << getID() << ") ERROR: ";  x; }
//MGFCONSRETURN

enum eMGComponentConsoleCommand{
	MGComponent_UNDEFINED = 0,

	//MGFramework commands
	MGComponent_HELP,
	MGComponent_EXIT,
	MGComponent_EXIT_APPLICATION,

	MGComponent_GETFPS,
	MGComponent_GETNUMBEROFMARKEDMO,
	MGComponent_GETNUMBEROFMO,

	MGComponent_EXPECT_GETNUMBEROFMARKEDMO_INT,
	MGComponent_EXPECT_GETNUMBEROFMO_INT,

	MGComponent_CREATE_MO_INT_PARAMLIST,
	MGComponent_ADD_MO_INT_PARAMLIST,
	MGComponent_CREATE_PE_INT,
	MGComponent_ADD_PE_INT,
	MGComponent_DELETE_ALL_MO_PARAMLIST,
	MGComponent_RUNFRAMES_INT,
	MGComponent_SETFPS_INT,
	MGComponent_OPEN_TERMINALSERVER,
	MGComponent_CLOSE_TERMINALSERVER,
	MGComponent_MINIMAP_ON,
	MGComponent_MINIMAP_OFF,
	MGComponent_LOGGING_ON,
	MGComponent_LOGGING_OFF,
	MGComponent_INPUT_OFF,
	MGComponent_INPUT_ON,

	//MGPeriodicEvent commands
	MGComponent_PE_INT_X,
	MGComponent_PE_ALL_X,
	MGComponent_PE_INT_HELP,
	MGComponent_PE_INT_ACTIVATE,
	MGComponent_PE_INT_DEACTIVATE,
	MGComponent_PE_INT_SETUPTIMER_INT,
	MGComponent_PE_INT_LOGGING_ON,
	MGComponent_PE_INT_LOGGING_OFF,
	MGComponent_PE_INT_STOREFILENAME_FILENAME,

	//MGMovingObject commands
	MGComponent_MO_INT_X,
	MGComponent_MO_INT_MARK,
	MGComponent_MO_ALL_MARK,
	MGComponent_MO_INT_UNMARK,
	MGComponent_MO_ALL_UNMARK,
	MGComponent_MO_MARKED_UNMARK,
	MGComponent_MO_INT_GETDISTANCE,
	MGComponent_MO_INT_GETLOCATION,
	MGComponent_MO_INT_GETDESTINATION,
	MGComponent_MO_INT_GETSPEED,
	MGComponent_MO_INT_HELP,
	MGComponent_MO_MARKED_X,
	MGComponent_MO_ALL_X,
	MGComponent_MO_INT_SETDESTINATION_INT_INT,
	MGComponent_MO_ALL_SETDESTINATION_INT_INT,
	MGComponent_MO_MARKED_SETDESTINATION_INT_INT,
	MGComponent_MO_INT_LOGGING_OFF,
	MGComponent_MO_INT_LOGGING_ON,
	MGComponent_MO_ALL_LOGGING_OFF,
	MGComponent_MO_ALL_LOGGING_ON,

	//MGMap commands
	MGComponent_MAP_X,
	MGComponent_MAP_HELP,
	MGComponent_MAP_PATH_INT_INT_INT_INT,

	//MGWindow commands
	MGComponent_WINDOW_X,
	MGComponent_WINDOW_HELP,
	MGComponent_WINDOW_FULLSCREEN_ON,
	MGComponent_WINDOW_LOGGING_OFF,
	MGComponent_WINDOW_LOGGING_ON

};


#ifndef MGF_DEBUGGING_ENABLED
int runMGFrameworkSocketTerminal(void *fm);
#endif

class MGFramework :public MGComponent
{
	private:
		// Enable/disable input functionality
		bool m_InputEnabled;
		void enableInput(){ m_InputEnabled = true;}
		void disableInput(){ m_InputEnabled = false;}
		bool isInputEnabled(){ return m_InputEnabled;}

		// Exit application functionality
		bool m_Quit;
		void quit(){ m_Quit = true; }
		bool getQuitFlag(){ return m_Quit; }

		bool m_TypingEnabled;		// Setting allows typing commands to console.
		bool m_WindowPropertiesSet; // Used to determine if all Windows properties have been set.
		bool m_MiniMapEnabled;		// Enables the mini map implementation.

		eMGFInstanceType m_MGFInstanceType;

		// FPS related
		Uint32 m_FrameTime;			// Holds current frame time
		Uint32 m_FPS;				// Holds desired FPS
		Sint32 m_DelayTime;			// Holds delay in ms for last frame

		// Countdown feature needs a flag and a counter.
		bool m_FrameCountdownEnabled;
		int m_FrameNumber;
		bool frameCountdownEnabled(){return m_FrameCountdownEnabled;}
		int getFrameNumber(){return m_FrameNumber;}
		void setFrameNumber(int f){m_FrameNumber = f;}
		void enableFrameCountdown(){m_FrameCountdownEnabled = true;}
		void disableFrameCountdown(){m_FrameCountdownEnabled = false;}
		void countdownFrame(int howmany){m_FrameNumber -= howmany;}

		//Console activation related
		void activateConsole();

		//MO related
		int m_NMO; // Number of Moving Objects
		int m_MarkedMOs;

		//PE related
		int m_NPE; // Number of Periodic Events

		//Socket terminal related.
		SDL_Thread *m_SocketTerminal;
		bool m_KeepSocketTerminalOpen;
		void openSocketTerminal(){m_KeepSocketTerminalOpen = true;}
		void closeSocketTerminal(){m_KeepSocketTerminalOpen = false;}

		// Frame selection related (click drag and release)
		bool m_FramingOngoing;
		int m_XFrameStart;
		int m_YFrameStart;
		int m_XFrameEnd;
		int m_YFrameEnd;
		void setFrameStartX(int x){ m_XFrameStart=x;}
		void setFrameStartY(int y){ m_YFrameStart=y;}
		void setFrameEndX(int x){ m_XFrameEnd=x;}
		void setFrameEndY(int y){ m_YFrameEnd=y;}
		void activateFraming(int x, int y){setFrameStartX(x); setFrameStartY(y); setFrameEndX(x); setFrameEndY(y); m_FramingOngoing=true;}
		void deactivateFraming(){ m_FramingOngoing = false;}
		void updateFraming(int x, int y){setFrameEndX(x); setFrameEndY(y);}

	protected:
		MGWindow m_Window;		// The framework window
		
		MGMovingObject *m_MO;	// Moving Objects
		MGPeriodicEvent *m_PE;  // Periodic Events

		// MO related
		void createMO(int n);
		void addMO(int n);
		int getNumberOfMO(){ return biggest(m_NMO, 0);}
		void deleteMO(int index);

		// PE related
		void createPE(int n);
		void addPE(int n);
		int getNumberOfPE(){ return m_NPE;}

		// Event related
		unsigned int m_Keys[SDLK_LAST];	// Stores keys that are pressed
		virtual bool processEvents();

		// Force a derived sub-class to implement this as it is not framework related.
		virtual void draw() = 0;
		virtual void handleGameLogics() = 0;

		virtual void resize(int x, int y);	// Not implemented yet

		// Frame selection related (click drag and release)
		bool isFramingOngoing(){ return m_FramingOngoing;}
		int getFrameStartX(){ return m_XFrameStart;}
		int getFrameStartY(){ return m_YFrameStart;}
		int getFrameEndX(){ return m_XFrameEnd;}
		int getFrameEndY(){ return m_YFrameEnd;}

		// Graphics related, depending on SDL
		SDL_Surface *getSurface(){return m_Window.m_Screen;}
		void drawSprite(SDL_Surface* imageSurface, SDL_Surface* screenSurface, int srcX, int srcY, int dstX, int dstY, int width, int height);
		SDL_Surface *loadBMPImage( std::string filename );
#ifndef MGF_DEBUGGING_ENABLED
		void drawText(SDL_Surface* screen, const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB);
#endif
		void putPixel32(SDL_Surface *surface, int x, int y, Uint32 pixel);
		Uint32 getPixel32(SDL_Surface *surface, int x, int y);
		void drawCircle32(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel);
		void drawFillCircle32(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel);
		void vLine32(SDL_Surface *surface, int x, int y, int length, Uint32 pixel);
		void hLine32(SDL_Surface *surface, int x, int y, int length, Uint32 pixel);

		// Controlling game speed and execution
		Uint32 getFPS();
		void setDesiredFPS(Uint32 f);
		Uint32 getDesiredFPS();
		Sint32 getLastFrameDelayTime(){return m_DelayTime;}	// A kind of measurement for how much time is left for additional calculations between frames.

		// Console activation related
		void enableTyping(){m_TypingEnabled = true;}
		void disableTyping(){m_TypingEnabled = false;}
		bool typingEnabled(){return m_TypingEnabled;}

	public:
		MGFramework();
		virtual ~MGFramework();

		MGMap m_Map;			// Map that holds game logics needs to be accessed when graphics are drawn, and by moving objects.

		// Initialization
		virtual bool init(int w, int h, int tw, int th) = 0;	// Force a derived sub-class to implement this as it is not framework related.
		bool setWindowProperties(int width, int height, int bpp, bool fullscreen, const string& title);
		bool setWindowProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const string& title);
		void unsetWindowProperties(){m_WindowPropertiesSet = false;}
		bool windowPropertiesSet(){return m_WindowPropertiesSet;}

		// Client/Server
		void setInstanceType(eMGFInstanceType it){ m_MGFInstanceType = it;}
		eMGFInstanceType getInstanceType(){return m_MGFInstanceType;}

		// Console command handling
		bool runConsoleCommand(const char *c, MGFramework *w);
		eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);

		//Socket terminal related
		bool socketTerminalOpen(){return m_KeepSocketTerminalOpen;}
		void logIfEnabled(const char *log){MGFLOG_INFO(std::cout << "" << log << std::endl;)}
		void addConsoleCommandToQueue(const char *c);

		// Program version
		const char *getMGFrameworkVersion(){return MGFRAMEWORKVERSION;}

		// Execute the framework
		void run(const char *scriptFileName);

		// Parsing script files containing mgf commands
		void parse(const char *scriptFileName);

		// Evaluating log files to PASS/FAIL
		void logEval(const char *logFileName);

		// Mini map
		void enableMiniMap(){m_MiniMapEnabled = true;}
		void disableMiniMap(){m_MiniMapEnabled = false;}
		bool miniMapEnabled(){return m_MiniMapEnabled;}

		// MO selection related
		void countMark(){m_MarkedMOs++;}
		void countUnMark(){m_MarkedMOs--;}
		int getNumberOfMarkedMO(){ return smallest(m_MarkedMOs, getNumberOfMO());}

		// Utility functions
		static string toString(int number);
		static double distance(int x1, int y1, int x2, int y2);
		static std::vector<std::string> split(std::string str, char c);
		static int toInt(const string &s);
		static int randomN(int upperLimit){return std::rand()%upperLimit;}
		static bool detectCollisionRectangle(int x1, int y1, int x2, int y2, int a1, int b1, int a2, int b2);
		static bool detectCollisionPointRectangle(int px, int py, int x1, int y1, int x2, int y2);
#ifndef MGF_DEBUGGING_ENABLED
		static int initializeWinsock(WORD wVersionRequested);
#endif
		static bool okMGFrameworkSyntax(const char *c);
		static bool oneOf(int x, int a1, int a2){ if(x==a1) return true;  if(x==a2) return true; return false;}
		static int smallest(int a, int b){ if(a<b) return a; return b;}
		static int biggest(int a, int b){ if(a>b) return a; return b;}
};

#endif

