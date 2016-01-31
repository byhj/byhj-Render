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