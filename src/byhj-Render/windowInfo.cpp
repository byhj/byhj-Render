#include "windowInfo.h"

namespace byhj
{
	std::shared_ptr<WindowInfo> WindowInfo::getInstance()
	{
		static auto pInstance = std::make_shared<WindowInfo>();

		return pInstance;
	}
	int WindowInfo::getWidth() const
	{
		return Width;
	}
	int WindowInfo::getHeight() const {
		return Height;
	}

	void WindowInfo::setWidth(int sw)
	{
	   Width = sw;
	}
	void WindowInfo::setHeight(int sh) 
	{
		Height = sh;
	}
	int WindowInfo::getPosX() const {
		return posX;
	}

	int WindowInfo::getPosY() const {
		return posY;
	}

	float WindowInfo::getAspect() const {
		return static_cast<float>(Width) / Height;
	}

	std::string WindowInfo::getTitle() const {
		return title;
	}
}