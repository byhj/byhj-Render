#include "vulkanRender.h"
#include "vk/vulkanDebug.h"

namespace byhj {

void VulkanRender::v_init()
{
	if (m_enableValidation) {
		Vulkan::Debug::setupDebugging(m_instance, VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT, NULL);
	}
	init_instance();
	init_device();
	init_commandPool();
	init_commandBuffers();
	init_swapchain();
	init_renderpass();
	init_depthStencil();
	init_framebuffer();
	init_pipelineCache();
}

void VulkanRender::v_update()
{
}

void VulkanRender::v_render()
{
}

void VulkanRender::v_shutdown()
{
}

//Create a vulkan instance
void VulkanRender::init_instance()
{
   VkApplicationInfo appInfo = {};
   appInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   appInfo.pApplicationName = "Test";
   appInfo.pEngineName      = "Vulkan";
   appInfo.apiVersion       = VK_MAKE_VERSION(1, 0, 8);

   std::vector<const char*> enabledExtensions = {VK_KHR_SURFACE_EXTENSION_NAME};
#ifdef _WIN32
   enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#else
   // todo : linux/android
   enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
   VkInstanceCreateInfo instanceCreateInfo ={};
   instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   instanceCreateInfo.pNext = NULL;
   instanceCreateInfo.pApplicationInfo = &appInfo;
   if (enabledExtensions.size() > 0) {
	   if (m_enableValidation) {
		   enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	   }
	   instanceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
	   instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
   }
   if (m_enableValidation) {
	   instanceCreateInfo.enabledLayerCount = Vulkan::Debug::validationLayerCount; // todo : change validation layer names!
	   instanceCreateInfo.ppEnabledLayerNames = Vulkan::Debug::validationLayerNames;
   }
   vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);
}


//Create a device
void VulkanRender::init_device() 
{
	VkDeviceQueueCreateInfo requestedQueues;
	std::vector<const char*> enabledExtensions ={ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkDeviceCreateInfo deviceCreateInfo ={};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = NULL;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &requestedQueues;
	deviceCreateInfo.pEnabledFeatures = NULL;

	if (enabledExtensions.size() > 0) {
		deviceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
	}
	if (m_enableValidation) {
		deviceCreateInfo.enabledLayerCount = Vulkan::Debug::validationLayerCount; // todo : validation layer names
		deviceCreateInfo.ppEnabledLayerNames = Vulkan::Debug::validationLayerNames;
	}

	vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);
}


//Create a commandBuffer 

void VulkanRender::init_commandPool()
{
	VkCommandPoolCreateInfo cmdPoolInfo ={};
	cmdPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = m_vulkanSwapChain.m_queueNodeIndex;
	cmdPoolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VkResult vkRes               = vkCreateCommandPool(m_device, &cmdPoolInfo, nullptr, &m_commandPool);
	assert(!vkRes);
}

void VulkanRender::init_swapchain()
{
	uint32_t sw = getClientWidth();
	uint32_t sh = getClientHeight();

	m_vulkanSwapChain.init(getHwnd(), getHinstance());
	m_vulkanSwapChain.setup(m_setupCmdBuffer, &sw, &sh);
}

void VulkanRender::init_commandBuffers()
{
// Create one command buffer per frame buffer in the swap chain
// Command buffers store a reference to the frame buffer inside their render pass info
// so for static usage withouth having to rebuild them each frame, we use one per frame buffer

}

void VulkanRender::init_pipelineCache()
{
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo ={};
	pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	VkResult err = vkCreatePipelineCache(m_device, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache);
	assert(!err);
}

void VulkanRender::init_depthStencil()
{
	VkImageCreateInfo image ={};
	image.sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image.pNext       = NULL;
	image.imageType   = VK_IMAGE_TYPE_2D;
	image.format      = m_depthFormat;
	image.extent      = { getClientWidth(), getClientHeight(), 1 };
	image.mipLevels   = 1;
	image.arrayLayers = 1;
	image.samples     = VK_SAMPLE_COUNT_1_BIT;
	image.tiling      = VK_IMAGE_TILING_OPTIMAL;
	image.usage       = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	image.flags       = 0;

	VkMemoryAllocateInfo mem_alloc ={};
	mem_alloc.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mem_alloc.pNext           = NULL;
	mem_alloc.allocationSize  = 0;
	mem_alloc.memoryTypeIndex = 0;

	VkImageViewCreateInfo depthStencilView ={};
	depthStencilView.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	depthStencilView.pNext                           = NULL;
	depthStencilView.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
	depthStencilView.format                          = m_depthFormat;
	depthStencilView.flags                           = 0;
	depthStencilView.subresourceRange                = {};
	depthStencilView.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	depthStencilView.subresourceRange.baseMipLevel   = 0;
	depthStencilView.subresourceRange.levelCount     = 1;
	depthStencilView.subresourceRange.baseArrayLayer = 0;
	depthStencilView.subresourceRange.layerCount     = 1;

	VkMemoryRequirements memReqs;
	VkResult err;

	err = vkCreateImage(m_device, &image, nullptr, &m_depthStencil.image);
	assert(!err);
	vkGetImageMemoryRequirements(m_device, m_depthStencil.image, &memReqs);
	mem_alloc.allocationSize = memReqs.size;
	getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);
	err = vkAllocateMemory(m_device, &mem_alloc, nullptr, &m_depthStencil.mem);
	assert(!err);

	err = vkBindImageMemory(m_device, m_depthStencil.image, m_depthStencil.mem, 0);
	assert(!err);
	Vulkan::setImageLayout(m_setupCmdBuffer, m_depthStencil.image, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

	depthStencilView.image = m_depthStencil.image;
	err = vkCreateImageView(m_device, &depthStencilView, nullptr, &m_depthStencil.view);
	assert(!err);
}

void VulkanRender::init_renderpass()
{
	VkAttachmentDescription attachments[2];

	attachments[0].format         = m_colorFormat;
	attachments[0].samples        = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments[0].finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	attachments[1].format         = m_depthFormat;
	attachments[1].samples        = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout  = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachments[1].finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference ={};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference ={};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass ={};
	subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.flags                   = 0;
	subpass.inputAttachmentCount    = 0;
	subpass.pInputAttachments       = NULL;
	subpass.colorAttachmentCount    = 1;
	subpass.pColorAttachments       = &colorReference;
	subpass.pResolveAttachments     = NULL;
	subpass.pDepthStencilAttachment = &depthReference;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments    = NULL;

	VkRenderPassCreateInfo renderPassInfo ={};
	renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.pNext           = NULL;
	renderPassInfo.attachmentCount = 2;
	renderPassInfo.pAttachments    = attachments;
	renderPassInfo.subpassCount    = 1;
	renderPassInfo.pSubpasses      = &subpass;
	renderPassInfo.dependencyCount = 0;
	renderPassInfo.pDependencies   = NULL;

	VkResult err;

	err = vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass);
	assert(!err);
}

void VulkanRender::init_framebuffer()
{
	VkImageView attachments[2];

	// Depth/Stencil attachment is the same for all frame buffers
	attachments[1] = m_depthStencil.view;

	VkFramebufferCreateInfo frameBufferCreateInfo ={};
	frameBufferCreateInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.pNext           = NULL;
	frameBufferCreateInfo.renderPass      = m_renderPass;
	frameBufferCreateInfo.attachmentCount = 2;
	frameBufferCreateInfo.pAttachments    = attachments;
	frameBufferCreateInfo.width           = getClientWidth();
	frameBufferCreateInfo.height          = getClientHeight();
	frameBufferCreateInfo.layers          = 1;

	// Create frame buffers for every swap chain image
	m_frameBuffers.resize(m_vulkanSwapChain.m_imageCount);
	for (uint32_t i = 0; i < m_frameBuffers.size(); i++)
	{
		attachments[0] = m_vulkanSwapChain.m_pBuffers[i].view;
		VkResult err = vkCreateFramebuffer(m_device, &frameBufferCreateInfo, nullptr, &m_frameBuffers[i]);
		assert(!err);
	}


}

VkBool32 VulkanRender::getMemoryType(uint32_t typeBits, VkFlags properties, uint32_t * typeIndex)
{
	for (uint32_t i = 0; i < 32; i++) {
		if ((typeBits & 1) == 1) {
			if ( (m_deivceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties ) {
				*typeIndex = i;
				return true;
			}
		}
		typeBits >>= 1;
	}
	return false;
}


}