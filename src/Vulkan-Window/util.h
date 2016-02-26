#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#ifdef _WIN32
#pragma comment(linker, "/subsystem:console")
#define WIN32_LEAN_AND_MEAN
#define VK_USE_PLATFORM_WIN32_KHR
#define NOMINMAX /* Don't let Windows define min() or max() */
#define APP_NAME_STR_LEN 80
#else // _WIN32
#define VK_USE_PLATFORM_XCB_KHR
#include <unistd.h>
#endif // _WIN32

#include <vulkan/vulkan.h>

#include "vulkan/vk_sdk_platform.h"

/* Number of descriptor sets needs to be the same at alloc,       */
/* pipeline layout creation, and descriptor set layout creation   */
#define NUM_DESCRIPTOR_SETS 1

/* Number of samples needs to be the same at image creation,      */
/* renderpass creation and pipeline creation.                     */
#define NUM_SAMPLES VK_SAMPLE_COUNT_1_BIT

/* Number of viewports and number of scissors have to be the same */
/* at pipeline creation and in any call to set them dynamically   */
/* They also have to be the same as each other                    */
#define NUM_VIEWPORTS 1
#define NUM_SCISSORS NUM_VIEWPORTS

/* Amount of time, in nanoseconds, to wait for a command buffer to complete */
#define FENCE_TIMEOUT 100000000

#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                               \
    {                                                                          \
        info.fp##entrypoint =                                                  \
            (PFN_vk##entrypoint)vkGetInstanceProcAddr(inst, "vk" #entrypoint); \
        if (info.fp##entrypoint == NULL) {                                     \
            std::cout << "vkGetDeviceProcAddr failed to find vk" #entrypoint;  \
            exit(-1);                                                          \
        }                                                                      \
    }

#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                                  \
    {                                                                          \
        info.fp##entrypoint =                                                  \
            (PFN_vk##entrypoint)vkGetDeviceProcAddr(dev, "vk" #entrypoint);    \
        if (info.fp##entrypoint == NULL) {                                     \
            std::cout << "vkGetDeviceProcAddr failed to find vk" #entrypoint;  \
            exit(-1);                                                          \
        }                                                                      \
    }

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

std::string get_base_data_dir();
std::string get_data_dir(std::string filename);



namespace Vulkan {
	/*
 * structure to track all objects related to a texture.
 */
struct Texture {

    VkSampler       sampler;

    VkImage         image;
    VkImageLayout   imageLayout;

    VkDeviceMemory memory;
    VkImageView    imageView;
    int32_t texWidth, texHeight;
};

/*
 * Keep each of our swap chain buffers' image, command buffer and view in one
 * spot
 */
struct SwapChainBuffer {
    VkImage image;
    VkImageView imageView;
};

/*
 * A layer can expose extensions, keep track of those
 * extensions here.
 */
struct LayerProperties {
    VkLayerProperties properties;
    std::vector<VkExtensionProperties> extensions;
};

/*
 * Structure for tracking information used / created / modified
 * by utility functions.
 */
	struct Depth {
		VkFormat format;
		VkImage  image;
		VkDeviceMemory memory;
		VkImageView imageView;
	};

	struct UniformData {
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDescriptorBufferInfo bufferInfo;
	};
	struct TextureData {
		VkDescriptorImageInfo imageInfo;
	};

	struct VertexBuffer {
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDescriptorBufferInfo bufferInfo;
	};



}

class Sample{

public:
#ifdef _WIN32
#define APP_NAME_STR_LEN 80
	HINSTANCE connection;        // hInstance - Windows Instance
	char name[APP_NAME_STR_LEN]; // Name to put on the window/icon
	HWND window;                 // hWnd - window handle
#else                            // _WIN32
	xcb_connection_t *connection;
	xcb_screen_t *screen;
	xcb_window_t window;
	xcb_intern_atom_reply_t *atom_wm_delete_window;
#endif  

	//////////////////////////////////////////////////////////////////
	VkSurfaceKHR surface;
	VkInstance   instance;
	VkDevice     device;
	VkQueue		 queue;
	VkFramebuffer *framebuffers;
	VkFormat format;
	uint32_t swapchainImageCount;
	VkSwapchainKHR swap_chain;

	VkSemaphore presentCompleteSemaphore;
	VkPhysicalDeviceProperties gpu_props;
	VkPhysicalDeviceMemoryProperties memory_properties;

	VkCommandPool cmd_pool;

	std::vector<Vulkan::SwapChainBuffer> buffers;
	std::vector<VkQueueFamilyProperties> queue_props;

	uint32_t graphics_queue_family_index;

	int width, height;
	bool prepared;
	bool use_staging_buffer;
	std::vector<const char *> instance_layer_names;
	std::vector<const char *> instance_extension_names;
	std::vector<Vulkan::LayerProperties> instance_layer_properties;
	std::vector<VkExtensionProperties> instance_extension_properties;

	std::vector<const char *> device_layer_names;
	std::vector<const char *> device_extension_names;
	std::vector<Vulkan::LayerProperties> device_layer_properties;
	std::vector<VkExtensionProperties> device_extension_properties;
	std::vector<VkPhysicalDevice> gpus;

	std::vector<Vulkan::Texture> textures;
	VkVertexInputBindingDescription vi_binding;
	VkVertexInputAttributeDescription vi_attribs[2];

	Vulkan::Depth depth;
	Vulkan::UniformData uniformData;
	Vulkan::TextureData textureData;
	Vulkan::VertexBuffer vertexBuffer;

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
	glm::mat4 MVP;

	VkCommandBuffer cmd;
	VkPipelineLayout pipelineLayout;
	VkPipelineCache  pipelineCache;
	VkRenderPass     renderPass;
	VkPipeline       pipeline;
	VkDeviceMemory stagingMemory;
	VkImage stagingImage;
	VkDescriptorPool desc_pool;
    VkPipelineShaderStageCreateInfo shaderStages[2];
    std::vector<VkDescriptorSet> desc_set;
	std::vector<VkDescriptorSetLayout> desc_layout;

	///////////////////////////// Debug CallBack  ////////////////////////////////////////////

	PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
	PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
	PFN_vkDebugReportMessageEXT dbgBreakCallback;
	std::vector<VkDebugReportCallbackEXT> debug_report_callbacks;

	///////////////////////////////////////////////////////////////////

	uint32_t current_buffer;
	uint32_t queue_count;

	VkViewport viewport;
	VkRect2D scissor;
};

bool memory_type_from_properties(Sample &info, uint32_t typeBits,
	VkFlags requirements_mask,
	uint32_t *typeIndex);

void set_image_layout(Sample &demo, VkImage image,
	VkImageAspectFlags aspectMask,
	VkImageLayout old_image_layout,
	VkImageLayout new_image_layout);

bool read_ppm(char const *const filename, int &width, int &height,
	uint64_t rowPitch, unsigned char *dataPtr);
void extract_version(uint32_t version, uint32_t &major, uint32_t &minor,
	uint32_t &patch);
bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader, std::vector<unsigned int> &spirv);
void init_glslang();
void finalize_glslang();
void wait_seconds(int seconds);
void print_UUID(uint8_t *pipelineCacheUUID);

typedef unsigned long long timestamp_t;
timestamp_t get_milliseconds();