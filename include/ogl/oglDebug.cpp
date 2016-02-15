#include "oglDebug.h"

#include "glDebug.h"

namespace byhj
{
	std::shared_ptr<OGLDebug> OGLDebug::getInstance()
	{
		static auto pInstance = std::make_shared<OGLDebug>();
		return pInstance;
	}
		
void OGLDebug::debug()
{
	static bool Check = true;

	if (Check)
	{
		CheckDebugLog();
		Check = false;
	}
}
}
