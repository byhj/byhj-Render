#include "fontMgr.h"

namespace byhj
{
	FontMgr::FontMgr()
	{
		m_dir += "fonts/";
	}

	FontMgr::~FontMgr()
	{
	}

	std::shared_ptr<FontMgr> FontMgr::getInstance()
	{
		static std::shared_ptr<FontMgr> pInstance = std::make_shared<FontMgr>();

		return pInstance;
	}
}