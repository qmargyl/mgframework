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


// Version format is <major release>.<minor release>.<features added>.<bug fixes>
#define MGFRAMEWORKVERSION "1.0.7.0"

int runMGFrameworkSocketTerminal(void *fm);


class MGFramework :public MGComponent
{
	private:

		bool m_LoggingEnabled;		// Setting allows extended logging to console.
		bool m_TypingEnabled;		// Setting allows typing commands to console.
		bool m_WindowPropertiesSet; // Used to determine if all Windows properties have been set.
		bool m_MiniMapEnabled;		// Enables the mini map implementation.

		// FPS related
		Uint32 m_FrameTime;			// Holds current frame time
		Uint32 m_FPS;				// Holds desired FPS
		Sint32 m_DelayTime;			// Holds delay in ms for last frame

		// Version related
		string m_ProgramVersion;	// Holds the application version, not the MG Framework version.

		// Countdown feature needs a flag and a counter.
		bool m_FrameCountdownEnabled;
		int m_FrameNumber;
		bool frameCountdownEnabled(){return m_FrameCountdownEnabled;}
		int getFrameNumber(){return m_FrameNumber;}
		void setFrameNumber(int f){m_FrameNumber = f;}
		void enableFrameCountdown(){m_FrameCountdownEnabled = true;}
		void disableFrameCountdown(){m_FrameCountdownEnabled = false;}
		void countdownFrame(int howmany){m_FrameNumber -= howmany;}

		//Console related
		void activateConsole();

		//MO related
		int m_NMO; // Number of Moving Objects

		//Socket terminal related.
		SDL_Thread *m_SocketTerminal;
		bool m_KeepSocketTerminalOpen;
		void openSocketTerminal(){m_KeepSocketTerminalOpen = true;}
		void closeSocketTerminal(){m_KeepSocketTerminalOpen = false;}
		

	protected:
		MGWindow m_Window;		// The framework window
		MGMap m_Map;			// Map that holds game logics needs to be accessed when graphics are drawn - protected.
		MGMovingObject *m_MO;	// Moving Objects

		// MO related
		void createMO(int n);
		void addMO(int n);
		int getNumberOfMO(){ return m_NMO;}

		// Event related
		unsigned int m_Keys[SDLK_LAST];	// Stores keys that are pressed
		virtual bool processEvents();

		// Force a derived sub-class to implement this as it is not framework related.
		virtual void draw() = 0;
		virtual void handleGameLogics() = 0;

		virtual void resize(int x, int y);	// Not implemented yet

		// Graphics related, depending on SDL
		SDL_Surface *getSurface(){return m_Window.m_Screen;}
		void drawSprite(SDL_Surface* imageSurface, SDL_Surface* screenSurface, int srcX, int srcY, int dstX, int dstY, int width, int height);
		SDL_Surface *loadBMPImage( std::string filename );
		void drawText(SDL_Surface* screen, const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB);
		void putPixel32(SDL_Surface *surface, int x, int y, Uint32 pixel);
		Uint32 getPixel32(SDL_Surface *surface, int x, int y);
		void drawCircle32(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel);
		void drawFillCircle32(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel);

		// Controlling game speed and execution
		Uint32 getFPS();
		void setDesiredFPS(Uint32 f);
		Uint32 getDesiredFPS();
		Sint32 getLastFrameDelayTime(){return m_DelayTime;}	// A kind of measurement for how much time is left for additional calculations between frames.

		// Configuration
		void enableTyping(){m_TypingEnabled = true;}
		void disableTyping(){m_TypingEnabled = false;}
		bool typingEnabled(){return m_TypingEnabled;}

	public:
		MGFramework();
		virtual ~MGFramework();

		// Initialization
		virtual bool init(int w, int h, int tw, int th) = 0;	// Force a derived sub-class to implement this as it is not framework related.
		bool setWindowProperties(int width, int height, int bpp, bool fullscreen, const string& title);
		void unsetWindowProperties(){m_WindowPropertiesSet = false;}
		bool windowPropertiesSet(){return m_WindowPropertiesSet;}

			
		// Configuration of Logging/Debugging
		void enableLogging(){m_LoggingEnabled = true;}
		void disableLogging(){m_LoggingEnabled = false;}
		bool loggingEnabled(){return m_LoggingEnabled;}


		// Console command handling
		bool runConsoleCommand(const char *c);

		//Socket terminal related
		bool socketTerminalOpen(){return m_KeepSocketTerminalOpen;}

		// Program version
		void setProgramVersion(const char *version){m_ProgramVersion = string(version);}
		const char *getProgramVersion(){return m_ProgramVersion.c_str();}

		virtual void run();

		// Mini map
		void enableMiniMap(){m_MiniMapEnabled = true;}
		void disableMiniMap(){m_MiniMapEnabled = false;}
		bool miniMapEnabled(){return m_MiniMapEnabled;}

		// Utility functions
		static string toString(int number);
		static double distance(int x1, int y1, int x2, int y2);
		static std::vector<std::string> split(std::string str, char c);
		static int toInt(const string &s);
		static int randomN(int upperLimit){return std::rand()%upperLimit;}
		static bool detectCollisionRectangle(int x1, int y1, int x2, int y2, int a1, int b1, int a2, int b2);
		static bool detectCollisionPointRectangle(int px, int py, int x1, int y1, int x2, int y2);
		static int initializeWinsock(WORD wVersionRequested);
};

#endif

