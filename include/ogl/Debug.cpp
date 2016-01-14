#include "Debug.h"

#include "glDebug.h"

namespace byhj
{

namespace ogl
{ 

void OGLDebug::Init()
{

}

void OGLDebug::Debug()
{
	static bool Check = true;

	if (Check)
	{
		CheckDebugLog();
		Check = false;
	}
}
}

}
