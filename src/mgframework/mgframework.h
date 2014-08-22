#ifndef _MG_FRAMEWORK_H
#define _MG_FRAMEWORK_H

#include "mgcomponent.h"
#include "mgwindow.h"
#include "mgmap.h"
#include "mgmovingobject.h"
#include "mgperiodicevent.h"
#include "mgsymboltable.h"
#include "mgstationaryobject.h"
#include "mgfversion.h"


// Configurable defines...
#define MGF_SCRIPTLINE_MAXLENGTH	256
#define MGF_MOPOSITIONINGATTEMPTS	100
#define MGF_SOPOSITIONINGATTEMPTS	100


enum eMGComponentConsoleCommand : unsigned int {
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
	MGComponent_ADD_SO_INT_PARAMLIST,
	MGComponent_DELETE_ALL_MO_PARAMLIST,
	MGComponent_DELETE_MO_INT,
	MGComponent_DELETE_ALL_PE_PARAMLIST,
	MGComponent_DELETE_ALL_SO_PARAMLIST,
	MGComponent_DELETE_SO_INT,
	MGComponent_RUNFRAMES_INT,
	MGComponent_RUNONEFRAME,
	MGComponent_SETFPS_INT,
	MGComponent_OPEN_TERMINALSERVER,
	MGComponent_CLOSE_TERMINALSERVER,

	//Settings
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



int runMGFrameworkSocketTerminal(void *fm);


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

		// Instance related
		eMGFInstanceType m_MGFInstanceType;
		int m_PlayerNumber;

		// FPS related
		unsigned int m_FrameTime;			// Holds current frame time
		unsigned int m_ActualFrameTime;	// Calculated for each frame
		unsigned int m_FPS;				// Holds desired FPS
		int m_DelayTime;			// Holds delay in ms for last frame
		bool m_DynamicFPSEnabled;	// Feature activation

		// Mouse scrolling
		bool m_FeatureMouseScrollingEnabled;	// Feature activation

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
#ifndef UNITTEST_LINUX
		SDL_Thread *m_SocketTerminal;
#endif
		bool m_KeepSocketTerminalOpen;
		int m_Port;

		// Frame selection related (click drag and release)
		bool m_FramingOngoing;
		int m_XFrameStart;
		int m_YFrameStart;
		int m_XFrameEnd;
		int m_YFrameEnd;
		bool m_OnlySelectOwnedMO;

		// Symbol related
		int m_CommandReturnVal;
		void symbolAssignTo(string sym, string val, MGSymbolTable *s);
		void symbolAssignTo(const std::vector<std::string> &cmdvec, MGSymbolTable *s);

		// Command queue related
		std::vector<std::string> m_CommandQueue;

		// Selective Tile Rendering
		bool m_SelectiveTileRendering;
		bool m_RenderAll;
		int m_NDrawnTiles;
		inline void resetDrawnTilesCounter(){ m_NDrawnTiles = 0; }
		inline void increaseDrawnTilesCounter(){ ++m_NDrawnTiles; }

		// Center on MO
		int m_FeatureCenterOnMO;

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

// ***	// Feature Mouse scrolling
		bool featureMouseScrollingEnabled(){ return m_FeatureMouseScrollingEnabled; }
		void enableFeatureMouseScrolling(){ m_FeatureMouseScrollingEnabled = true; }
		void disableFeatureMouseScrolling(){ m_FeatureMouseScrollingEnabled = false; }

// ***	// Feature Center on MO
		bool featureCenterOnMO(){ return (m_FeatureCenterOnMO > -1); }
		int centerMOIndex(){ return m_FeatureCenterOnMO; }
		void enableFeatureCenterOnMO(int moindex = 0){ m_FeatureCenterOnMO = moindex; }
		void disableFeatureCenterOnMO(){ m_FeatureCenterOnMO = -1; }

// ***	// Feature Dynamic FPS
		bool featureDynamicFPSEnabled(){ return m_DynamicFPSEnabled; }
		void setDynamicFPSEnabled(bool val){ m_DynamicFPSEnabled = val; }

		// Exit application functionality
		void quit();
		bool getQuitFlag(){ return m_Quit; }

		// Countdown feature needs a flag and a counter.
		inline bool frameCountdownEnabled(){return m_FrameCountdownEnabled; }
		inline int getFrameNumber(){return m_FrameNumber; }
		inline void setFrameNumber(int f){m_FrameNumber = f; }
		inline void enableFrameCountdown(){m_FrameCountdownEnabled = true; }
		inline void disableFrameCountdown(){m_FrameCountdownEnabled = false; }
		inline void countdownFrame(int howmany){m_FrameNumber -= howmany; }

		// Console activation related
		void activateConsole();

		//Socket terminal related.
		void openSocketTerminal(){m_KeepSocketTerminalOpen = true; }
		void closeSocketTerminal(){m_KeepSocketTerminalOpen = false; }

		// Frame selection related (click drag and release)
		inline void setFrameStartX(int x){ m_XFrameStart=x; }
		inline void setFrameStartY(int y){ m_YFrameStart=y; }
		inline void setFrameEndX(int x){ m_XFrameEnd=x; }
		inline void setFrameEndY(int y){ m_YFrameEnd=y; }

		inline void activateFraming(int x, int y)
		{
			setFrameStartX(x); 
			setFrameStartY(y); 
			setFrameEndX(x); 
			setFrameEndY(y); 
			m_FramingOngoing=true;
		}

		inline void deactivateFraming()
		{ 
			m_FramingOngoing = false;
			setRenderAllTiles();
		}

		inline void updateFraming(int x, int y)
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
		int getNumberOfMO(){ return std::max(m_NMO, 0); }
		void deleteMO(int index);		// Deletes the MO with a specified index
		bool setupMO(int i, int x, int y, unsigned int owner, int speed, int x1, int y1, int x2, int y2);		// Setups the MO with a specified index

		// PE related
		void deleteAllPE();
		void addPE(int n);
		int getNumberOfPE(){ return std::max(m_NPE, 0); }
		void deletePE(int index);

		// SO related
		void deleteAllSO();
		void addSO(int n);
		int getNumberOfSO(){ return std::max(m_NSO, 0); }
		void deleteSO(int index);
		bool setupSO(int i, int x, int y);		// Setups the MO with a specified index


		// Event related
#ifndef UNITTEST_LINUX
		unsigned int m_Keys[SDLK_LAST];	// Stores keys that are pressed
#endif
		virtual bool processEvents();

		// Force a derived sub-class to implement this as it is not framework related.
		virtual void draw() = 0;
		virtual void handleGameLogics() = 0;

		// Frame selection related (click drag and release)
		inline bool isFramingOngoing(){ return m_FramingOngoing; }
		inline int getFrameStartX(){ return m_XFrameStart; }
		inline int getFrameStartY(){ return m_YFrameStart; }
		inline int getFrameEndX(){ return m_XFrameEnd; }
		inline int getFrameEndY(){ return m_YFrameEnd; }

// ***	// Feature Selective MO slection
		inline void enableFeatureOnlySelectOwnedMO(){ m_OnlySelectOwnedMO = true; }
		inline void disableFeatureOnlySelectOwnedMO(){ m_OnlySelectOwnedMO = false; }
		inline bool featureOnlySelectOwnedMOEnabled(){ return m_OnlySelectOwnedMO; }


		// Graphics related, based on SDL
#ifndef UNITTEST_LINUX
		// TODO: Shouldn't these be moved to MGWindow??????
		void drawTile(SDL_Surface* imageSurface, int srcX, int srcY, int dstX, int dstY);
		void drawTile(SDL_Surface* imageSurface, int srcX, int srcY, int dstX, int dstY, int tileW, int tileH);
#endif

		// Controlling game speed and execution
		inline unsigned int getFPS();
		inline void setDesiredFPS(unsigned int f);
		inline unsigned int getDesiredFPS();
		inline int getLastFrameDelayTime(){return m_DelayTime;}	// How much time was left for additional calculations last frame.

		// Console activation related
		void enableTyping(){m_TypingEnabled = true;}
		void disableTyping(){m_TypingEnabled = false;}
		bool typingEnabled(){return m_TypingEnabled;}

// ***	// Feature Selective Tile Rendering
		void enableFeatureSelectiveTileRendering(){ m_SelectiveTileRendering = true; }
		void disableFeatureSelectiveTileRendering(){ m_SelectiveTileRendering = false; }
		void setRenderAllTiles(){ m_RenderAll = true; }
		void unsetRenderAllTiles(){ m_RenderAll = false; }
		bool renderAllTiles(){ return m_RenderAll; }
		int getDrawnTilesCounter(){ return m_NDrawnTiles; }

		// Instance related
		void setClientPlayer(int id){ m_PlayerNumber = id; }
		int getClientPlayer(){ return m_PlayerNumber; }

	public:
		MGFramework();
		virtual ~MGFramework();

		MGMap m_Map;			// Map that holds game logics needs to be accessed when graphics are drawn, and by moving objects.

		// Initialization
		virtual bool init(int w, int h, int tw, int th) = 0;	// Force a derived sub-class to implement this as it is not framework related.
		bool setWindowProperties(int width, int height, int bpp, bool fullscreen, const string& title);
		bool setWindowProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const string& title);
		void unsetWindowProperties(){ m_WindowPropertiesSet = false; }
		bool windowPropertiesSet(){ return m_WindowPropertiesSet; }

		// Client/Server
		void setInstanceType(eMGFInstanceType it){ m_MGFInstanceType = it; }
		eMGFInstanceType getInstanceType(){ return m_MGFInstanceType; }

		// Console command handling
		bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
		eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec);
		static bool isNumericalInt(const string &s); // returns true if the argument contains only numbers.
		static int staticToInt(const string &s); // returns an int converted from either a constant or a symbol.
		int toInt(const string &s, MGSymbolTable *sym); // returns an int converted from either a constant or a symbol.
		bool okMGFrameworkSyntax(const std::vector<std::string> &v_s);

		//Socket terminal related
		bool socketTerminalOpen(){return m_KeepSocketTerminalOpen;}
		void logIfEnabled(const char *log){}//{MGFLOG_INFO("" << log)}
		void addConsoleCommandToQueue(const char *c);
		int getPort(){ return m_Port; }
		void setPort(int p){ m_Port = p; }

		// Program version
		const char *getMGFrameworkVersion(){ return MGFRAMEWORKVERSION; }

		// Execute the framework
		void run(const char *scriptFileName, bool runOneFrame = false);

		// Parsing script files containing mgf commands
		int parse(const char *sFileName);

		// Test coverage related
		void registerUsedCommand(eMGComponentConsoleCommand c, int returnValue = 0)
		{
			m_CommandReturnVal = returnValue;
			m_UsedCommands[(int)c] = true; 
		}

// ***	// Feature Mini map
		void enableFeatureMiniMap(){ m_MiniMapEnabled = true; }
		void disableFeatureMiniMap(){ m_MiniMapEnabled = false; }
		bool featureMiniMapEnabled(){ return m_MiniMapEnabled; }

		// MO selection related
		void countMark(){ m_MarkedMOs++; }
		void countUnMark(){ m_MarkedMOs--; }
		
		int getNumberOfMarkedMO()
		{ 
			return std::min(m_MarkedMOs, getNumberOfMO());
		}

		// Rendering optimizations related
		bool isSelectiveTileRenderingActive(){ return m_SelectiveTileRendering; }

		static int initializeWinsock(WORD wVersionRequested);

};

#endif

