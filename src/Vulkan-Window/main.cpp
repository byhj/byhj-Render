#include "base.h"

#pragma comment(lib, "glslang.lib")
#pragma comment(lib, "spirv-tools.lib")
#pragma comment(lib, "spirv.lib")
#pragma comment(lib, "oglcompiler.lib")
#pragma comment(lib, "osdependent.lib")
#pragma comment(lib, "vulkan-1.lib")

int main(int argc, char **argv)
{
	Base base;
	base.init();
	base.render();
	base.shutdown();

	return 0;
}