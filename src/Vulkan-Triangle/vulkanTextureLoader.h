
#pragma once

#include <vulkan/vulkan.h>
#include <gli/gli.hpp>

#include "vulkanUtility.h"

namespace Vulkan
{

	struct Texture
	{
		VkSampler sampler;
		VkImage image;
		VkImageLayout imageLayout;
		VkDeviceMemory deviceMemory;
		VkImageView view;
		uint32_t width, height;
		uint32_t mipLevels;
	};
}
	class VulkanTextureLoader
	{
	private:
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkQueue queue;
		VkCommandBuffer cmdBuffer;
		VkCommandPool cmdPool;
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

		// Try to find appropriate memory type for a memory allocation
		VkBool32 getMemoryType(uint32_t typeBits, VkFlags properties, uint32_t *typeIndex)
		{
			for (int i = 0; i < 32; i++) {
				if ((typeBits & 1) == 1) {
					if ((deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
					{
						*typeIndex = i;
						return true;
					}
				}
				typeBits >>= 1;
			}
			return false;
		}
	public:
		// Load a 2D texture
		void loadTexture(const char* filename, VkFormat format, Vulkan::Texture *texture);

		// Load a 2D texture
		void loadTexture(const char* filename, VkFormat format, Vulkan::Texture *texture, bool forceLinear);

		// Clean up vulkan resources used by a texture object
		void destroyTexture(Vulkan::Texture texture);

		// Load a cubemap texture (single file)
		void loadCubemap(const char* filename, VkFormat format, Vulkan::Texture *texture);

		VulkanTextureLoader(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool cmdPool)
		{
			this->physicalDevice = physicalDevice;
			this->device = device;
			this->queue = queue;
			this->cmdPool = cmdPool;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

			// Create command buffer for submitting image barriers
			// and converting tilings
			VkCommandBufferAllocateInfo cmdBufInfo ={};
			cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmdBufInfo.commandPool = cmdPool;
			cmdBufInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			cmdBufInfo.commandBufferCount = 1;

			VkResult vkRes = vkAllocateCommandBuffers(device, &cmdBufInfo, &cmdBuffer);
			assert(vkRes == VK_SUCCESS);
		}

		~VulkanTextureLoader()
		{
			vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);
		}




	};
