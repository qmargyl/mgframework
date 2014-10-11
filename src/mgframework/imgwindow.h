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

		virtual void drawSprite(void* imageSurface, int srcX, int srcY, int dstX, int dstY, int width, int height) = 0;
		virtual void* loadBMPImage(std::string filename, bool transparent) = 0;
		virtual void drawText(const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB) = 0;
		virtual void putPixelRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b) = 0;
		virtual void vLineRGB(int x, int y, int length, unsigned char r, unsigned char g, unsigned char b) = 0;
		virtual void hLineRGB(int x, int y, int length, unsigned char r, unsigned char g, unsigned char b) = 0;
};

#endif