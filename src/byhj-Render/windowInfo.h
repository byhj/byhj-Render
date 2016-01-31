#ifndef WindowInfo_H
#define WindowInfo_H

#include <string>
#include <memory>

namespace byhj
{

#ifdef _WIN32
#include <windows.h>
	const int ScreenWidth  = static_cast<int>(GetSystemMetrics(SM_CXSCREEN))  * 0.8;
	const int ScreenHeight = static_cast<int>(GetSystemMetrics(SM_CYSCREEN)) * 0.8;

	const int PosX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth);
	const int PosY = (GetSystemMetrics(SM_CYSCREEN) - ScreenHeight);

#else
	const int ScreenWidth = 1200;
	const int ScreenHeight = 1000;
	const int PosX = 300;
	const int PosY = 100;
#endif

class WindowInfo
{
public:
	WindowInfo() :title("OpenGL-Learning:"),
		Width(ScreenWidth),
		Height(ScreenHeight),
		posX(PosX),
		posY(PosY) {}

	static std::shared_ptr<WindowInfo> getInstance();
	
	int getWidth() const;
	int getHeight() const;
	int getPosX() const;
	int getPosY() const;
	float getAspect() const;
	std::string getTitle() const;

private:
    std::string title;
	int Width;
	int Height;
	int posX, posY;
};

}
#endif // ifndef WindowInfo_H
