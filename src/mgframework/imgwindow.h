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
		virtual void* loadBMPImage(std::string filename) = 0;
		virtual void drawText(const char* string, int size, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB) = 0;
		virtual void putPixel32(int x, int y, unsigned int pixel) = 0;
		virtual unsigned int getPixel32(int x, int y) = 0;
		virtual void drawCircle32(int n_cx, int n_cy, int radius, unsigned int pixel) = 0;
		virtual void drawFillCircle32(int cx, int cy, int radius, unsigned int pixel) = 0;
		virtual void vLine32(int x, int y, int length, unsigned int pixel) = 0;
		virtual void hLine32(int x, int y, int length, unsigned int pixel) = 0;
};

#endif