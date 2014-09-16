#ifndef _I_MG_WINDOW_H
#define _I_MG_WINDOW_H

enum eMGWindowScreenResolution{
	// Wide screen resolutions (16:9)
	MGWindow_RES_1920_1080 = 0,
	MGWindow_RES_1600_900,
	// Normal screen resolutions (4:3)
	MGWindow_RES_1024_768,
	MGWindow_RES_800_600,
	MGWindow_RES_640_480
};

class IMGWindow
{
	public:
		virtual bool createWindow() = 0;
		virtual bool setProperties(int width, int height, int bpp, bool fullscreen, const std::string &title) = 0;
		virtual bool setProperties(eMGWindowScreenResolution screenResolution, int bpp, bool fullscreen, const std::string &title) = 0;
		virtual void setSize(int width, int height) = 0;
		virtual int getHeight() = 0;
		virtual int getWidth() = 0;
		virtual void flipSurface() = 0;
		virtual void activateFullscreen() = 0;
		virtual void deactivateFullscreen() = 0;
};

#endif