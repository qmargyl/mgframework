#ifndef _MG_FRAMEWORK_H
#define _MG_FRAMEWORK_H

#include "window.h"
#include <SDL/SDL.h>
#include "mgmap.h"
#include "SDL_ttf.h"

#include <iostream>
#include <sstream>
using namespace std;

// Version format is <major release>.<minor release>.<features added>.<bug fixes>
#define MGFRAMEWORKVERSION "1.0.0.1"


class MGFramework
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

		string m_ProgramVersion;	// Holds the application version, not the MG Framework version.

		char m_CommandLine[128];

	protected:
		MGWindow m_Window;				// The framework window
		unsigned int m_Keys[SDLK_LAST];	// Stores keys that are pressed

		// Force a derived sub-class to implement this as it is not framework related.
		virtual void draw() = 0;
		virtual void handleGameLogics() = 0;

		virtual void resize(int x, int y);	// Not implemented yet
		virtual bool processEvents();

		// Graphics related, depending on SDL
		SDL_Surface *getSurface();
		void drawSprite(SDL_Surface* imageSurface, SDL_Surface* screenSurface, int srcX, int srcY, int dstX, int dstY, int width, int height);
		SDL_Surface *loadBMPImage( std::string filename );
		void drawText(SDL_Surface* screen, const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB);
		void putPixel32(SDL_Surface *surface, int x, int y, Uint32 pixel);



	public:
		MGFramework();
		virtual ~MGFramework();

		// Initialization
		virtual bool init(int w, int h, int tw, int th) = 0;	// Force a derived sub-class to implement this as it is not framework related.
		bool setWindowProperties(int width, int height, int bpp, bool fullscreen, const string& title);
		void unsetWindowProperties();
		bool windowPropertiesSet();

			
		// Logging/Debugging
		void enableLogging();
		void disableLogging();
		bool loggingEnabled();
		void enableTyping();
		void disableTyping();
		bool typingEnabled();

		// Program version
		void setProgramName(const char *name);
		const char *getProgramName();
		void setProgramVersion(const char *version);
		const char *getProgramVersion();

		// Controlling game speed and execution
		Uint32 getFPS();
		void setDesiredFPS(Uint32 f);
		Uint32 getDesiredFPS();
		Sint32 getLastFrameDelayTime(); // A kind of measurement for how much time is left for additional calculations between frames.

		virtual void run();

		// Mini map
		void enableMiniMap();
		void disableMiniMap();
		bool miniMapEnabled();

		// Map that holds game logics needs to be accessed when graphics are drawn - public.
		MGMap map;

		// Utility functions
		static string toString(int number);
		static double distance(int x1, int y1, int x2, int y2);
};

#endif

