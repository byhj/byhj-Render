#ifndef ShaderMgr_H
#define ShaderMgr_H

#include "Mgr.h"
#include <memory>

namespace byhj
{
	class ShaderMgr : public Mgr
	{
	public:
	   ShaderMgr();
	   ~ShaderMgr();

	   static std::shared_ptr<ShaderMgr> getInstance();

	private:

	};
}
#endif
