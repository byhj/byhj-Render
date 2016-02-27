#ifndef VulkanDebug_H
#define VulkanDebug_H

#include <vulkan/vulkan.h>

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <vector>
#include <string>
#include <cstring>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#else
#endif



namespace Vulkan {

namespace Debug {

	// Default validation layers
	extern int validationLayerCount;
	extern const char *validationLayerNames[];

	// Default debug callback
	VkBool32 messageCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t srcObject,
		size_t location,
		int32_t msgCode,
		const char* pLayerPrefix,
		const char* pMsg,
		void* pUserData);

	// Load debug function pointers and set debug callback
	// if callBack is NULL, default message callback will be used
	void setupDebugging(
		VkInstance instance,
		VkDebugReportFlagsEXT flags,
		VkDebugReportCallbackEXT callBack);
	// Clear debug callback
	void freeDebugCallback(VkInstance instance);
}
}

#endif