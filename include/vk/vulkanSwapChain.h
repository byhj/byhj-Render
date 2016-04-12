#ifndef VulkanSwapChain_H
#define VulkanSwapChain_H


#include <stdlib.h>
#include <string>
#include <fstream>
#include <assert.h>
#include <stdio.h>

#include <windows.h>
#include <fcntl.h>
#include <io.h>

#include <vulkan/vulkan.h>
#include "vulkanUtility.h"


// Macro to get a procedure address based on a vulkan instance
#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                        \
{                                                                       \
    fp##entrypoint = (PFN_vk##entrypoint) vkGetInstanceProcAddr(inst, "vk"#entrypoint); \
    if (fp##entrypoint == NULL)                                         \
	{																    \
        exit(1);                                                        \
    }                                                                   \
}

// Macro to get a procedure address based on a vulkan device
#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                           \
{                                                                       \
    fp##entrypoint = (PFN_vk##entrypoint) vkGetDeviceProcAddr(dev, "vk"#entrypoint);   \
    if (fp##entrypoint == NULL)                                         \
	{																    \
        exit(1);                                                        \
    }                                                                   \
}

namespace byhj {

    class VulkanSwapChain
    {   
    public:
		struct SwapChainBuffer {
			VkImage image;
			VkImageView view;
		};

		VkFormat   m_colorFormat;
		VkColorSpaceKHR m_colorSpace;
		VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
		VkImage   *m_pSwapChainImages;
		SwapChainBuffer  * m_pBuffers;

		uint32_t m_imageCount;
		// Index of the deteced graphics and presenting device queue
		uint32_t m_queueNodeIndex = UINT32_MAX;

    	// wip naming
    	void init(void* platformWindow, void* platformHandle);
    	void init(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);
    	void setup(VkCommandBuffer cmdBuffer, uint32_t *width, uint32_t *height);
    	void shutdown();
    
    	// Acquires the next image in the swap chain
    	VkResult acquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t *currentBuffer);

    	// Present the current image to the queue
    	VkResult queuePresent(VkQueue queue, uint32_t currentBuffer);

	private:

		VkInstance        m_instance;
		VkDevice          m_device;
		VkPhysicalDevice  m_physicalDevice;
		VkSurfaceKHR      m_surface;




		// Function pointers
		PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
		PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
		PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
		PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
		PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
		PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
		PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
		PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
		PFN_vkQueuePresentKHR fpQueuePresentKHR;
    };
    
}
#endif