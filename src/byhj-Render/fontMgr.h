#ifndef FontMgr_H
#define FontMgr_H

#include <iostream>
#include <memory>
#include "mgr.h"

namespace byhj
{
	class FontMgr : public Mgr {
	public:
		 FontMgr(); 
		~FontMgr();

		static std::shared_ptr<FontMgr> getInstance();

	};
}
#endif