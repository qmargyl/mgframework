#ifndef _MG_FRAMEWORK_H
#define _MG_FRAMEWORK_H

//#define MGF_DEBUGGING_ENABLED		// This flag disables the socket terminal and TTF as these are not possible to compile for Debug.


// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#ifndef MGF_DEBUGGING_ENABLED
  #include "SDL_ttf.h"
#endif

// MGF header files
#include "mgcomponent.h"
#include "mgwindow.h"
#include "mgmap.h"
#include "mgmovingobject.h"
#include "mgperiodicevent.h"
#include "mgsymboltable.h"
#include "mgstationaryobject.h"


// Version format is <major release>.<minor release>.<features added>.<bug fixes>
#define MGFRAMEWORKVERSION "1.0.38.14"

// Configurable defines...
#define MGF_SCRIPTLINE_MAXLENGTH	256
#define MGF_LOGLINE_MAXLENGTH		1024
#define MGF_MOPOSITIONINGATTEMPTS	100
#define MGF_SOPOSITIONINGATTEMPTS	100




enum eMGComponentConsoleCommand{
	MGComponent_UNDEFINED = 0,

	//MGFramework basic commands
	MGComponent_HELP,										//help (Prints help information to console)
	MGComponent_EXIT,										//exit (Exits console, resumes graphics execution) 
	MGComponent_EXIT_APPLICATION,							//exit application (Terminates application)

	//General getter
	MGComponent_INT,

	//Expect function for testing and generating errors
	MGComponent_EXPECT_INT_INT,

	//MGFramework commands
	MGComponent_ADD_MO_INT_PARAMLIST,
	MGComponent_ADD_PE_INT_PARAMLIST,
	MGComponent_DELETE_ALL_MO_PARAMLIST,
	MGComponent_DELETE_MO_INT,
	MGComponent_DELETE_ALL_PE_PARAMLIST,
	MGComponent_ADD_SO_INT_PARAMLIST,
	MGComponent_DELETE_ALL_SO_PARAMLIST,
	MGComponent_DELETE_SO_INT,
	MGComponent_RUNFRAMES_INT,
	MGComponent_RUNONEFRAME,
	MGComponent_SETFPS_INT,
	MGComponent_OPEN_TERMINALSERVER,
	MGComponent_CLOSE_TERMINALSERVER,

	//Settings flags
	MGComponent_LOGGING_BOOL,
	MGComponent_MINIMAP_BOOL,
	MGComponent_INPUT_BOOL,
	MGComponent_DYNAMICFPS_BOOL,

	//Operators
	MGComponent_SYMBOL_ASSIGNTO_INT,						//var1 = 32, var2 = getnumberofmo, etc

	//MGPeriodicEvent forward flags
	MGComponent_PE_INT_X,
	MGComponent_PE_ALL_X,

	//MGPeriodicEvent commands
	MGComponent_PE_INT_HELP,
	MGComponent_PE_INT_ACTIVATE,
	MGComponent_PE_ALL_ACTIVATE_PARAMLIST,
	MGComponent_PE_INT_DEACTIVATE,
	MGComponent_PE_ALL_DEACTIVATE_PARAMLIST,
	MGComponent_PE_INT_SETUPTIMER_INT,
	MGComponent_PE_INT_LOGGING_ON,
	MGComponent_PE_INT_LOGGING_OFF,
	MGComponent_PE_INT_STOREFILENAME_FILENAME,				//pe <int> << | storefilename <file> (Stores the file name of the script file to be executed periodically)

	//MGMovingObject forward flags
	MGComponent_MO_INT_X,									//mo <int> ... (Forwards any MO command to an MO)
	MGComponent_MO_MARKED_X,								//mo marked ... (Forwards any MO command to all marked MOs)
	MGComponent_MO_ALL_X,									//mo all ... (Forwards any MO command to all MOs)

	//MGMovingObject commands
	MGComponent_MO_INT_MARK,
	MGComponent_MO_INT_UNMARK,
	MGComponent_MO_INT_GETLOCATION,
	MGComponent_MO_INT_GETDESTINATION,
	MGComponent_MO_INT_GETSPEED,
	MGComponent_MO_INT_HELP,
	MGComponent_MO_INT_SETDESTINATION_INT_INT,
	MGComponent_MO_INT_LOGGING_OFF,
	MGComponent_MO_INT_LOGGING_ON,
	MGComponent_MO_INT_HISTORY_BOOL,
	MGComponent_MO_INT_EXPECT_GETLOCATION_INT_INT,
	// TODO: Implement these..
	// MGComponent_MO_INT_SETSPEED_INT,
	// MGComponent_MO_INT_EXPECT_GETSPEED_INT,

	//MGMap forward flags
	MGComponent_MAP_X,

	//MGMap commands
	MGComponent_MAP_HELP,
	MGComponent_MAP_PATH_INT_INT_INT_INT,
	MGComponent_MAP_SETSIZE_INT_INT_INT_INT,				//map setsize <x> <y> <tx> <ty> (Sets the size of the map to x*y tiles, each tx*ty pixels big)
	MGComponent_MAP_LOGGING_ON,
	MGComponent_MAP_LOGGING_OFF,

	//MGWindow forward flags
	MGComponent_WINDOW_X,

	//MGWindow commands
	MGComponent_WINDOW_HELP,
	MGComponent_WINDOW_FULLSCREEN_ON,
	MGComponent_WINDOW_FULLSCREEN_OFF,
	MGComponent_WINDOW_LOGGING_OFF,
	MGComponent_WINDOW_LOGGING_ON,

	// MGStationaryObject forward flags
	MGComponent_SO_INT_X,
	MGComponent_SO_ALL_X,

	// MGStationaryObject commands
	MGComponent_SO_INT_GETLOCATION,
	MGComponent_SO_INT_HELP,
	MGComponent_SO_INT_LOGGING_ON,
	MGComponent_SO_INT_LOGGING_OFF,
	MGComponent_SO_ALL_LOGGING_ON,
	MGComponent_SO_ALL_LOGGING_OFF,


	//Counter for number of command identifiers, not an actual command.
	MGComponent_NUMBEROFCOMMANDIDENTIFIERS

};




class PathItem
{
private:
	int m_X;
	int m_Y;
	double m_Heuristic;

public:
	PathItem();
	PathItem(int x, int y){m_X=x; m_Y=y; m_Heuristic=1; };
	PathItem(int x, int y, double h){m_X=x; m_Y=y; m_Heuristic=h; };
	~PathItem(){/*std::cout << "PathItem::~PathItem()\n";*/};
	int getX(){ return m_X;}
	int getY(){ return m_Y;}
	double getH(){ return m_Heuristic;}
	void setH(double h){ m_Heuristic = h; }
	void setPI(int x, int y, int h){ m_X=x; m_Y=y; m_Heuristic=h;}
	bool equalCoordinate(PathItem* pi){ return (getX()==pi->getX()) && (getY()==pi->getY());}
};


#ifndef MGF_DEBUGGING_ENABLED
int runMGFrameworkSocketTerminal(void *fm);
#endif

class MGFramework :public MGComponent
{
	private:
		// Test framework functionality
		bool m_UsedCommands[MGComponent_NUMBEROFCOMMANDIDENTIFIERS];

		// Enable/disable input functionality
		bool m_InputEnabled;

		// Exit application functionality
		bool m_Quit;

		bool m_TypingEnabled;		// Setting allows typing commands to console.
		bool m_WindowPropertiesSet; // Used to determine if all Windows properties have been set.
		bool m_MiniMapEnabled;		// Enables the mini map implementation.

		eMGFInstanceType m_MGFInstanceType;

		// FPS related
		Uint32 m_FrameTime;			// Holds current frame time
		Uint32 m_ActualFrameTime;	// Calculated for each frame
		Uint32 m_FPS;				// Holds desired FPS
		Sint32 m_DelayTime;			// Holds delay in ms for last frame
		bool m_DynamicFPSEnabled;

		// Countdown feature needs a flag and a counter.
		bool m_FrameCountdownEnabled;
		int m_FrameNumber;

		//MO related
		int m_NMO; // Number of Moving Objects
		int m_MarkedMOs;

		//PE related
		int m_NPE; // Number of Periodic Events

		//SO related
		int m_NSO; // Number of Stationary Objects

		//Socket terminal related.
		SDL_Thread *m_SocketTerminal;
		bool m_KeepSocketTerminalOpen;
		int m_Port;

		// Frame selection related (click drag and release)
		bool m_FramingOngoing;
		int m_XFrameStart;
		int m_YFrameStart;
		int m_XFrameEnd;
		int m_YFrameEnd;

		// Symbol related
		int m_CommandReturnVal;
		void symbolAssignTo(string sym, string val, MGSymbolTable *s);

		// Command queue related
		std::vector<std::string> m_CommandQueue;

		// Selective Tile Rendering
		bool m_SelectiveTileRendering;
		bool m_RenderAll;
		int m_NDrawnTiles;
		void resetDrawnTilesCounter(){ m_NDrawnTiles = 0; }
		void increaseDrawnTilesCounter(){ ++m_NDrawnTiles; }

		// Font
#ifndef MGF_DEBUGGING_ENABLED
		TTF_Font* m_Font;
#endif

	protected:
		MGWindow m_Window;				// The framework window
		MGMovingObject *m_MO;			// Moving Objects
		MGPeriodicEvent *m_PE;			// Periodic Events
		MGStationaryObject *m_SO;		// Stationary Objects
		MGSymbolTable *m_SymbolTable;	// Symbols
		MGSymbolTable *m_SymbolTableTransfer;	// Symbols transferred during a function call

		// Test framework functionality
		int getNumberOfUsedCommands();
		int getNumberOfCommands(){ return (int) MGComponent_NUMBEROFCOMMANDIDENTIFIERS; }

		// Enable/disable input functionality
		void enableInput(){ m_InputEnabled = true;}
		void disableInput(){ m_InputEnabled = false;}
		bool isInputEnabled(){ return m_InputEnabled;}

		// FPS
		bool getDynamicFPSEnabled(){ return m_DynamicFPSEnabled; }
		void setDynamicFPSEnabled(bool val){ m_DynamicFPSEnabled = val; }

		// Exit application functionality
		void quit();
		bool getQuitFlag(){ return m_Quit; }

		// Countdown feature needs a flag and a counter.
		bool frameCountdownEnabled(){return m_FrameCountdownEnabled;}
		int getFrameNumber(){return m_FrameNumber;}
		void setFrameNumber(int f){m_FrameNumber = f;}
		void enableFrameCountdown(){m_FrameCountdownEnabled = true;}
		void disableFrameCountdown(){m_FrameCountdownEnabled = false;}
		void countdownFrame(int howmany){m_FrameNumber -= howmany;}

		// Console activation related
		void activateConsole();

		//Socket terminal related.
		void openSocketTerminal(){m_KeepSocketTerminalOpen = true;}
		void closeSocketTerminal(){m_KeepSocketTerminalOpen = false;}

		// Frame selection related (click drag and release)
		void setFrameStartX(int x){ m_XFrameStart=x;}
		void setFrameStartY(int y){ m_YFrameStart=y;}
		void setFrameEndX(int x){ m_XFrameEnd=x;}
		void setFrameEndY(int y){ m_YFrameEnd=y;}

		void activateFraming(int x, int y)
		{
			setFrameStartX(x); 
			setFrameStartY(y); 
			setFrameEndX(x); 
			setFrameEndY(y); 
			m_FramingOngoing=true;
		}

		void deactivateFraming()
		{ 
			m_FramingOngoing = false;
			setRenderAllTiles();
		}

		void updateFraming(int x, int y)
		{
			setFrameEndX(x); 
			setFrameEndY(y);
			setRenderAllTiles();
		}

		// Execution, game logics..
		void handleMGFGameLogics();

		// MO related
		void deleteAllMO();
		int addMO(int n); // Returns index of first MO added or -1 if there was an error.
		int getNumberOfMO(){ return biggest(m_NMO, 0);}
		void deleteMO(int index);		// Deletes the MO with a specified index
		bool setupMO(int i, int x, int y, unsigned int owner, int speed);		// Setups the MO with a specified index

		// PE related
		void deleteAllPE();
		void addPE(int n);
		int getNumberOfPE(){ return biggest(m_NPE, 0);}
		void deletePE(int index);

		// SO related
		void deleteAllSO();
		void addSO(int n);
		int getNumberOfSO(){ return biggest(m_NSO, 0);}
		void deleteSO(int index);
		bool setupSO(int i, int x, int y);		// Setups the MO with a specified index


		// Event related
		unsigned int m_Keys[SDLK_LAST];	// Stores keys that are pressed
		virtual bool processEvents();

		// Force a derived sub-class to implement this as it is not framework related.
		virtual void draw() = 0;
		virtual void handleGameLogics() = 0;

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
		Sint32 getLastFrameDelayTime(){return m_DelayTime;}	// How much time was left for additional calculations last frame.

		// Console activation related
		void enableTyping(){m_TypingEnabled = true;}
		void disableTyping(){m_TypingEnabled = false;}
		bool typingEnabled(){return m_TypingEnabled;}

		// Rendering optimizations related
		void activateSelectiveTileRendering(){ m_SelectiveTileRendering = true; }
		void deactivateSelectiveTileRendering(){ m_SelectiveTileRendering = false; }
		void setRenderAllTiles(){ m_RenderAll = true; }
		void unsetRenderAllTiles(){ m_RenderAll = false; }
		bool renderAllTiles(){ return m_RenderAll; }
		int getDrawnTilesCounter(){ return m_NDrawnTiles; }

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
		bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
		eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);
		static bool isNumericalInt(const string &s); // returns true if the argument contains only numbers.
		static int staticToInt(const string &s); // returns an int converted from either a constant or a symbol.
		int toInt(const string &s, MGSymbolTable *sym); // returns an int converted from either a constant or a symbol.
		bool okMGFrameworkSyntax(/*const char *c*/const std::vector<std::string> &v_s);

		//Socket terminal related
		bool socketTerminalOpen(){return m_KeepSocketTerminalOpen;}
		void logIfEnabled(const char *log){}//{MGFLOG_INFO("" << log)}
		void addConsoleCommandToQueue(const char *c);
		int getPort(){ return m_Port; }
		void setPort(int p){ m_Port = p; }

		// Randomization
		void randomize(int seed){ std::srand(seed); }

		// Program version
		const char *getMGFrameworkVersion(){return MGFRAMEWORKVERSION;}

		// Execute the framework
		void run(const char *scriptFileName, bool runOneFrame = false);

		// Parsing script files containing mgf commands
		int parse(const char *sFileName);

		// Evaluating log files to PASS/FAIL
		void logEval(const char *logFileName, bool negativeTest=false);

		// Filtering log files
		std::string filterLine(const char* line);
		void logFilter(const char *logFileName);

		// Comparing log files
		void logCompare(const char *logFileName1, const char *logFileName2);

		// Test coverage related
		void registerUsedCommand(eMGComponentConsoleCommand c, int returnValue = 0)
		{
			m_CommandReturnVal = returnValue;
			m_UsedCommands[(int)c]=true; 
		}

		// Mini map
		void enableMiniMap(){m_MiniMapEnabled = true;}
		void disableMiniMap(){m_MiniMapEnabled = false;}
		bool miniMapEnabled(){return m_MiniMapEnabled;}

		// MO selection related
		void countMark(){m_MarkedMOs++;}
		void countUnMark(){m_MarkedMOs--;}
		
		int getNumberOfMarkedMO()
		{ 
			return smallest(m_MarkedMOs, getNumberOfMO());
		}

		// Rendering optimizations related
		bool isSelectiveTileRenderingActive(){ return m_SelectiveTileRendering; }


#ifndef MGF_DEBUGGING_ENABLED
		static int initializeWinsock(WORD wVersionRequested);
#endif


};

#endif

