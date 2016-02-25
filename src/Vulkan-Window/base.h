#pragma once

#ifdef _WIN32
#pragma comment (linker, "/subsystem:windows")
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#else
// todo : split linux xcb/x11 and android
#include <xcb/xcb.h>
#endif

#include <iostream>
#include <chrono>
#include <string>
#include <array>

#include <vulkan/vulkan.h>

class Base {

public:
   void init();
   void createWindow();
   void prepare();
   void render();
   void shutdown();
   
private: 
	void init_vk();

	VkSurfaceKHR m_surface;
	VkInstance m_instance;
	VkDevice    m_device;
	VkQueue   m_queue;
	VkPhysicalDeviceProperties m_gpuProps;
	VkQueueFamilyProperties    m_queueProps;
    VkFormat  m_format;
	VkColorSpaceKHR  m_colorSpace;
	VkSwapchainKHR  m_swapchain;
	VkCommandPool   m_cmdPool;
	struct  DepthBuffer
	{
		VkFormat format;
		VkImage  image;
		VkDeviceMemory memory;
		VkImageView view;
	};
};