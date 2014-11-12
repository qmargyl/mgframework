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

#ifndef UNITTEST_LINUX
enum eMGComponentConsoleCommand 
#else
enum eMGComponentConsoleCommand : unsigned int
#endif
{
	MGComponent_UNDEFINED = 0,

	MGComponent_HELP,										//help (Prints help information to console)
	MGComponent_EXIT,										//exit (Exits console, resumes graphics execution) 
	MGComponent_EXIT_APPLICATION,							//exit application (Terminates application)

	// General getter
	MGComponent_INT,

	// Expect function for testing and generating errors (this is only added for testing
	// purposes and should be removed when the design allows it)
	MGComponent_EXPECT_INT_INT,

	// MGFramework commands
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
	MGComponent_OPEN_TERMINALSERVER,
	MGComponent_CLOSE_TERMINALSERVER,
	MGComponent_DUMP,

	// Settings
	MGComponent_SETFPS_INT,
	MGComponent_LOGGING_BOOL,
	MGComponent_MINIMAP_BOOL,
	MGComponent_INPUT_BOOL,
	MGComponent_WINDOW_FULLSCREEN_BOOL,
	// TODO: Implement this to replace all settings commands:
	// MGComponent_CONFIGURE_PARAMLIST,
	// configure -pathfinding <astar/basic> -fullscreen <on/off> -fps <int> -input <on/off> -logging <on/off> -minimap <on/off>

	// Operators
	MGComponent_SYMBOL_ASSIGNTO_INT,						//var1 = 32, var2 = getnumberofmo, etc

	// MGPeriodicEvent forward flags
	MGComponent_PE_INT_X,
	MGComponent_PE_ALL_X,

	// MGPeriodicEvent commands
	MGComponent_PE_INT_HELP,
	MGComponent_PE_INT_ACTIVATE,
	MGComponent_PE_ALL_ACTIVATE_PARAMLIST,
	MGComponent_PE_INT_DEACTIVATE,
	MGComponent_PE_ALL_DEACTIVATE_PARAMLIST,
	MGComponent_PE_INT_SETUPTIMER_INT,
	MGComponent_PE_INT_LOGGING_ON,
	MGComponent_PE_INT_LOGGING_OFF,
	MGComponent_PE_INT_STOREFILENAME_FILENAME,				//pe <int> << | storefilename <file> (Stores the file name of the script file to be executed periodically)

	// MGMovingObject forward flags
	MGComponent_MO_INT_X,									//mo <int> ... (Forwards any MO command to an MO)
	MGComponent_MO_MARKED_X,								//mo marked ... (Forwards any MO command to all marked MOs)
	MGComponent_MO_ALL_X,									//mo all ... (Forwards any MO command to all MOs)

	// MGMovingObject commands
	MGComponent_MO_INT_MARK,
	MGComponent_MO_INT_UNMARK,
	MGComponent_MO_INT_GETLOCATION,
	MGComponent_MO_INT_GETSPEED,
	MGComponent_MO_INT_HELP,
	MGComponent_MO_INT_SETDESTINATION_INT_INT_PARAMLIST,
	MGComponent_MO_INT_LOGGING_OFF,
	MGComponent_MO_INT_LOGGING_ON,
	MGComponent_MO_INT_HISTORY_BOOL,
	MGComponent_MO_INT_EXPECT_GETLOCATION_INT_INT,
	// TODO: Implement these..
	// MGComponent_MO_INT_SETSPEED_INT,
	// MGComponent_MO_INT_EXPECT_GETSPEED_INT,

	// MGMap forward flags
	MGComponent_MAP_X,

	// MGMap commands
	MGComponent_MAP_HELP,
	MGComponent_MAP_PATH_INT_INT_INT_INT,
	MGComponent_MAP_SETSIZE_INT_INT_INT_INT,				//map setsize <x> <y> <tx> <ty> (Sets the size of the map to x*y tiles, each tx*ty pixels big)
	MGComponent_MAP_LOGGING_ON,
	MGComponent_MAP_LOGGING_OFF,

	// MGStationaryObject forward flags
	MGComponent_SO_INT_X,
	MGComponent_SO_ALL_X,

	// MGStationaryObject commands
	MGComponent_SO_INT_GETLOCATION,
	MGComponent_SO_INT_LOGGING_ON,
	MGComponent_SO_INT_LOGGING_OFF,
	MGComponent_SO_ALL_LOGGING_ON,
	MGComponent_SO_ALL_LOGGING_OFF,
};



int runMGFrameworkSocketTerminal(void *fm);


class MGFramework : public MGComponent
{
	private:
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
		unsigned int m_ActualFrameTime;	// Calculated for each frame
		unsigned int m_FPS;				// Holds desired FPS
		int m_DelayTime;			// Holds delay in ms for last frame

		// Mouse scrolling
		bool m_FeatureMouseScrollingEnabled;	// Feature activation

		// Countdown feature needs a flag and a counter.
		bool m_FrameCountdownEnabled;
		int m_FrameNumber;

		//MO related
		unsigned int m_MarkedMOs;

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
		void symbolAssignTo(std::string sym, std::string val, MGSymbolTable *s);
		void symbolAssignTo(const std::vector<std::string> &cmdvec, MGSymbolTable *s);

		// Selective Tile Rendering
		bool m_SelectiveTileRendering;
		bool m_RenderAll;

		// Center on MO
		int m_FeatureCenterOnMO;

		IMGWindow *m_Window;

	protected:

		std::list<MGMovingObject> m_MO;			// Moving Objects
		std::list<MGPeriodicEvent> m_PE;		// Periodic Events
		std::list<MGStationaryObject> m_SO;		// Stationary Objects
		MGSymbolTable *m_SymbolTable;			// Symbols
		MGSymbolTable *m_SymbolTableTransfer;	// Symbols transferred during a function call

		// Enable/disable input functionality
		void enableInput(){ m_InputEnabled = true;}
		void disableInput(){ m_InputEnabled = false;}
		bool isInputEnabled() const { return m_InputEnabled;}

// ***	// Feature Mouse scrolling
		bool featureMouseScrollingEnabled() const { return m_FeatureMouseScrollingEnabled; }
		void enableFeatureMouseScrolling(){ m_FeatureMouseScrollingEnabled = true; }
		void disableFeatureMouseScrolling(){ m_FeatureMouseScrollingEnabled = false; }

// ***	// Feature Center on MO
		bool featureCenterOnMO() const { return (m_FeatureCenterOnMO > -1); }
		unsigned int centerMOIndex() const { return m_FeatureCenterOnMO; }
		void enableFeatureCenterOnMO(int moindex = 0){ m_FeatureCenterOnMO = moindex; }
		void disableFeatureCenterOnMO(){ m_FeatureCenterOnMO = -1; }

		// Exit application functionality
		void quit();
		bool getQuitFlag() const { return m_Quit; }

		// Countdown feature needs a flag and a counter.
		inline bool frameCountdownEnabled() const {return m_FrameCountdownEnabled; }
		inline int getFrameNumber() const {return m_FrameNumber; }
		inline void setFrameNumber(int f){m_FrameNumber = f; }
		inline void enableFrameCountdown(){m_FrameCountdownEnabled = true; }
		inline void disableFrameCountdown(){m_FrameCountdownEnabled = false; }
		inline void countdownFrame(int howmany){m_FrameNumber -= howmany; }

		// Console activation related
		void activateConsole();

		//Socket terminal related.
		void openSocketTerminal(){ m_KeepSocketTerminalOpen = true; }
		void closeSocketTerminal(){ m_KeepSocketTerminalOpen = false; }

		// Frame selection related (click drag and release)
		inline bool isFramingOngoing() const { return m_FramingOngoing; }
		inline int getFrameStartX() const { return m_XFrameStart; }
		inline int getFrameStartY() const { return m_YFrameStart; }
		inline int getFrameEndX() const { return m_XFrameEnd; }
		inline int getFrameEndY() const { return m_YFrameEnd; }
		inline void setFrameStartX(int x){ m_XFrameStart = x; }
		inline void setFrameStartY(int y){ m_YFrameStart = y; }
		inline void setFrameEndX(int x){ m_XFrameEnd = x; }
		inline void setFrameEndY(int y){ m_YFrameEnd = y; }
		inline void activateFraming(int x, int y);
		inline void deactivateFraming();
		inline void updateFraming(int x, int y);

		// Execution, game logics..
		void handleMGFGameLogics();

		// MO related
		void deleteAllMO();
		void addMO(int n);
		unsigned int getNumberOfMO() const { return (unsigned int)m_MO.size(); }
		void deleteMO(std::list<MGMovingObject>::iterator it);
		bool setupMO(std::list<MGMovingObject>::iterator it, int x, int y, unsigned int owner, int speed, int x1, int y1, int x2, int y2);

		// SO related
		void deleteAllSO();
		void addSO(int n);
		unsigned int getNumberOfSO() const { return (unsigned int)m_SO.size(); }
		void deleteSO(std::list<MGStationaryObject>::iterator it);
		bool setupSO(std::list<MGStationaryObject>::iterator it, int x, int y, int t = 0, bool occupyMap = true);

		// PE related
		void deleteAllPE();
		void addPE(int n);
		unsigned int getNumberOfPE() const { return (unsigned int)m_PE.size(); }
		void deletePE(std::list<MGPeriodicEvent>::iterator it);

		// MO selection related
		int getNumberOfMarkedMO() const { return std::min(m_MarkedMOs, getNumberOfMO()); }

		// Event related
#ifndef UNITTEST_LINUX
		unsigned int m_Keys[SDL_NUM_SCANCODES];	// Stores keys that are pressed
#endif
		virtual bool processEvents();

		// Force a derived sub-class to implement this as it is not framework related.
		virtual void draw() = 0;
		virtual void handleGameLogics() = 0;

		// Drawing helper functions
		void drawBasicMiniMap(int distFromUpRightX, int distFromUpRightY);
		void drawAllSOWithTexHandles();

// ***	// Feature MO selection for own MO
		inline void enableFeatureOnlySelectOwnedMO(){ m_OnlySelectOwnedMO = true; }
		inline void disableFeatureOnlySelectOwnedMO(){ m_OnlySelectOwnedMO = false; }
		inline bool featureOnlySelectOwnedMOEnabled() const { return m_OnlySelectOwnedMO; }

		// Controlling frame rate
		inline unsigned int getFPS() const;
		inline void setDesiredFPS(unsigned int f);
		inline unsigned int getDesiredFPS() const;
		inline int getLastFrameDelayTime() const {return m_DelayTime;}	// How much time was left for additional calculations last frame.

		// Console activation related
		void enableTyping(){m_TypingEnabled = true;}
		void disableTyping(){m_TypingEnabled = false;}
		bool typingEnabled() const {return m_TypingEnabled;}

// ***	// Feature Selective Tile Rendering
		void enableFeatureSelectiveTileRendering(){ m_SelectiveTileRendering = true; }
		void disableFeatureSelectiveTileRendering(){ m_SelectiveTileRendering = false; }
		void setRenderAllTiles(){ m_RenderAll = true; }
		void unsetRenderAllTiles(){ m_RenderAll = false; }
		bool renderAllTiles() const { return m_RenderAll; }

		// Instance related
		void setClientPlayer(int id){ m_PlayerNumber = id; }
		int getClientPlayer() const { return m_PlayerNumber; }

// ***	// Feature Forest Generation Algorithms
		void increaseDensityOfStationaryObjects(int stationaryObjectType, int neighboursThreshold);
		void fillInStationaryObjectClusters(int stationaryObjectType);
		void setStationaryObjectTexHandles(int stationaryObjectType, std::vector<MGTexHandle*> tHVec);

	public:
		MGFramework();
		virtual ~MGFramework();

		// Map that holds game logics needs to be accessed when graphics are
		// drawn, and by moving objects.
		MGMap m_Map;

		IMGWindow *getWindow()
		{
			return m_Window;
		}

		// Initialization
		virtual bool init(int w, int h, int tw, int th) = 0;	// Force a derived sub-class to implement this as it is not framework related.
		bool setWindowProperties(int width, int height, int bpp, bool fullscreen, const std::string &title, IMGWindow *win);
		bool setWindowProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const std::string &title, IMGWindow *win);
		void unsetWindowProperties(){ m_WindowPropertiesSet = false; m_Window = NULL; }
		bool windowPropertiesSet() const { return m_WindowPropertiesSet; }

		// MO selection related (used by MO)
		void countMark(){ m_MarkedMOs++; }
		void countUnMark();

		// Client/Server
		void setInstanceType(eMGFInstanceType it){ m_MGFInstanceType = it; }
		eMGFInstanceType getInstanceType() const { return m_MGFInstanceType; }

		// Console command handling
		bool runConsoleCommand(const char *c, MGFramework *w, MGSymbolTable *s);
		eMGComponentConsoleCommand detectMGComponentConsoleCommand(const std::vector<std::string> &cmdvec) const;
		static bool isNumericalInt(const std::string &s); // returns true if the argument contains only numbers.
		static int staticToInt(const std::string &s); // returns an int converted from either a constant or a symbol.
		bool toBool(const std::string &s, MGSymbolTable *sym) const; // returns a bool converted from either a constant or a symbol.
		int toInt(const std::string &s, MGSymbolTable *sym) const; // returns an int converted from either a constant or a symbol.
		void setCommandReturnValue(int v){ m_CommandReturnVal = v; } // TODO: Make this private?
		bool okMGFrameworkSyntax(const std::vector<std::string> &v_s) const; // TODO: Make this private?

		//Socket terminal related
		bool socketTerminalOpen() const { return m_KeepSocketTerminalOpen; }
		void logIfEnabled(const char *log){}//{MGFLOG_INFO("" << log)} // TODO: Remove this?
		int getPort() const { return m_Port; }
		void setPort(int p){ m_Port = p; }

		// Program version
		std::string getMGFrameworkVersion() const { return std::string(MGFRAMEWORKVERSION); }

		// Execute the framework
		void run(const char *scriptFileName, bool runOneFrame = false);

		// Parsing script files containing mgf commands
		int parse(const char *sFileName);

// ***	// Feature Mini map
		void enableFeatureMiniMap(){ m_MiniMapEnabled = true; }
		void disableFeatureMiniMap(){ m_MiniMapEnabled = false; }
		bool featureMiniMapEnabled() const { return m_MiniMapEnabled; }

		// Rendering optimizations related
		bool isSelectiveTileRenderingActive() const { return m_SelectiveTileRendering; }

		// Dumping information to file for debugging purposes
		void dump(std::string addToName = "");

		static int initializeWinsock(WORD wVersionRequested);

};

#endif

