#include "base.h"

#pragma comment(lib, "glslang.lib")
#pragma comment(lib, "spirv-tools.lib")
#pragma comment(lib, "spirv.lib")
#pragma comment(lib, "oglcompiler.lib")
#pragma comment(lib, "osdependent.lib")
#pragma comment(lib, "vulkan-1.lib")

int main()
{
	Base base;
	base.init();

	bool done; // flag saying when app is complete
	done = false; // initialize loop condition variable
	while (!done) {
		vkDeviceWaitIdle(base.m_info.device);
			     base.render();
		vkDeviceWaitIdle(base.m_info.device);
	}

	base.shutdown();

	return 0;
}

