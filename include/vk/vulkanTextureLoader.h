
#pragma once

#include <vulkan/vulkan.h>
#include <gli/gli.hpp>

#include "vulkanUtility.h"
#include <memory>

namespace Vulkan
{

	struct Texture
	{
		VkSampler sampler;
		VkImage   image;
		VkImageLayout imageLayout;
		VkDeviceMemory deviceMemory;
		VkImageView view;
		uint32_t width, height;
		uint32_t mipLevels;
	};
}
class VulkanTextureLoader
{
public:
	VulkanTextureLoader();
	~VulkanTextureLoader();

	void init(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool cmdPool);

	// Load a 2D texture
	void loadTexture(const char* filename, VkFormat format, Vulkan::Texture *texture);

	// Load a 2D texture
	void loadTexture(const char* filename, VkFormat format, Vulkan::Texture *texture, bool forceLinear);

	// Clean up vulkan resources used by a texture object
	void destroyTexture(Vulkan::Texture texture);

	// Load a cubemap texture (single file)
	void loadCubemap(const char* filename, VkFormat format, Vulkan::Texture *texture);
	
	static std::shared_ptr<VulkanTextureLoader> getInstance();

private:
	// Try to find appropriate memory type for a memory allocation
	VkBool32 getMemoryType(uint32_t typeBits, VkFlags properties, uint32_t *typeIndex);

	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue queue;
	VkCommandBuffer cmdBuffer;
	VkCommandPool cmdPool;
	VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
};
