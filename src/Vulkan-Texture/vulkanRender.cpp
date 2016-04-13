#include "vulkanRender.h"
#include "vk/vulkanDebug.h"

namespace byhj {

void VulkanRender::v_init()
{
	if (m_enableValidation) {
		Vulkan::Debug::setupDebugging(m_instance, VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT, NULL);
	}
	init_vulkan();

	init_commandPool();
	create_setupCmdBuffer();
	init_swapchain();
	init_commandBuffers();
	init_depthStencil();
	init_renderpass();
	init_pipelineCache();
	init_framebuffer();

	flush_setupCmdBuffer();
	// Recreate setup command buffer for derived class
	create_setupCmdBuffer();
	m_cube.init(m_device);
	m_cube.init_pipeline(m_renderPass, m_pipelineCache);
	build_commandBuffers();
}

void VulkanRender::v_update()
{
	m_cube.update();
}

void VulkanRender::v_render()
{


	VkResult res = VK_SUCCESS;

	vkDeviceWaitIdle(m_device);
	VkSemaphore presentCompleteSemaphore;
	VkSemaphoreCreateInfo presentCompleteSemaphoreCreateInfo ={};
	presentCompleteSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	presentCompleteSemaphoreCreateInfo.pNext = nullptr;
	presentCompleteSemaphoreCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	res = vkCreateSemaphore(m_device, &presentCompleteSemaphoreCreateInfo, nullptr, &presentCompleteSemaphore);
	assert(!res);

	//Get next image in the swap chain (back/front buffer)
	res = m_vulkanSwapChain.acquireNextImage(presentCompleteSemaphore, &m_currentBuffer);

	assert(!res);

	//The submit infor structure contains a list of command buffers and semaphores to 
	// be submitted to a queue, if you want to submit multiple command buffer, pass a array
	VkSubmitInfo submitInfo ={};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount =1;
	submitInfo.pWaitSemaphores = &presentCompleteSemaphore;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_drawCmdBuffers[m_currentBuffer];
	//Submit to the graphics queue
	res = vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
	assert(!res);

	//Present the current buffer to the swap chain, this will display the image
	res =m_vulkanSwapChain.queuePresent(m_queue, m_currentBuffer);
	assert(!res);

	vkDestroySemaphore(m_device, presentCompleteSemaphore, nullptr);


	//Add as post present image memory barrier, this will transform the frame buffer color 
	//attachment back to it's initial layout after it has been presented to the windowing system
	//see buildcCommandBuffers for the pre present barrier that does the opposite transformation
	VkImageMemoryBarrier postPresentBarrier ={};
	postPresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	postPresentBarrier.pNext = nullptr;
	postPresentBarrier.srcAccessMask = 0;
	postPresentBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	postPresentBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	postPresentBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	postPresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	postPresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	postPresentBarrier.subresourceRange ={ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	postPresentBarrier.image = m_vulkanSwapChain.m_pBuffers[m_currentBuffer].image;

	//Use dedicated command buffer from example base class for submitting the post present barrier
	VkCommandBufferBeginInfo cmdBufferInfo ={};
	cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	res = vkBeginCommandBuffer(m_postPresentCmdBuffer, &cmdBufferInfo);
	assert(!res);

	//Put post present barrier into command buffer
	vkCmdPipelineBarrier(m_postPresentCmdBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		VK_FLAGS_NONE, 0, nullptr, 0, nullptr, 1, &postPresentBarrier);

	res = vkEndCommandBuffer(m_postPresentCmdBuffer);
	assert(!res);

	//Submit to the queue
	submitInfo ={};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_postPresentCmdBuffer;
	res = vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
	assert(!res);
	res = vkQueueWaitIdle(m_queue);
	assert(!res);

	vkDeviceWaitIdle(m_device);
}

void VulkanRender::v_shutdown()
{
    m_cube.shutdown();
	// Clean up Vulkan resources
	m_vulkanSwapChain.shutdown();

	// 	vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
	if (m_setupCmdBuffer != VK_NULL_HANDLE)
	{
		vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_setupCmdBuffer);

	}
	destroy_cmdBuffers();

	vkDestroyRenderPass(m_device, m_renderPass, nullptr);
	for (uint32_t i = 0; i < m_frameBuffers.size(); i++)
	{
		vkDestroyFramebuffer(m_device, m_frameBuffers[i], nullptr);
	}

	vkDestroyImageView(m_device, m_depthStencil.view, nullptr);
	vkDestroyImage(m_device, m_depthStencil.image, nullptr);
	vkFreeMemory(m_device, m_depthStencil.mem, nullptr);

	vkDestroyPipelineCache(m_device, m_pipelineCache, nullptr);


	vkDestroyCommandPool(m_device, m_commandPool, nullptr);

	vkDestroyDevice(m_device, nullptr);

	if (m_enableValidation) {
		Vulkan::Debug::freeDebugCallback(m_instance);
	}

	vkDestroyInstance(m_instance, nullptr);

#ifndef _WIN32
	xcb_destroy_window(connection, window);
	xcb_disconnect(connection);
#endif 
}
void VulkanRender::init_vulkan()
{
	VkResult err = VK_SUCCESS;

	init_instance();

	if (err) {
		Vulkan::exitFatal("Could not create Vulkan instance : \n" + Vulkan::errorString(err), "Fatal error");
	}

	// Physical m_device
	uint32_t gpuCount = 0;
	// Get number of available physical devices
	err = vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr);
	assert(!err);
	assert(gpuCount > 0);
	// Enumerate devices
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	err = vkEnumeratePhysicalDevices(m_instance, &gpuCount, physicalDevices.data());
	if (err)
	{
		Vulkan::exitFatal("Could not enumerate phyiscal devices : \n" + Vulkan::errorString(err), "Fatal error");
	}

	// Note : 
	// This example will always use the first physical m_device reported, 
	// change the vector index if you have multiple Vulkan devices installed 
	// and want to use another one
	m_physicalDevice = physicalDevices[0];

	// Find a queue that supports graphics operations
	uint32_t graphicsQueueIndex = 0;
	uint32_t queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, NULL);
	assert(queueCount >= 1);

	std::vector<VkQueueFamilyProperties> queueProps;
	queueProps.resize(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, queueProps.data());

	for (graphicsQueueIndex = 0; graphicsQueueIndex < queueCount; graphicsQueueIndex++)
	{
		if (queueProps[graphicsQueueIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			break;
	}
	assert(graphicsQueueIndex < queueCount);

	// Vulkan m_device
	std::array<float, 1> queuePriorities ={ 0.0f };
	VkDeviceQueueCreateInfo queueCreateInfo ={};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = queuePriorities.data();

	init_device(queueCreateInfo);

	// Gather physical m_device memory properties
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_deviceMemoryProperties);
	                                                       
	// Get the graphics queue
	vkGetDeviceQueue(m_device, graphicsQueueIndex, 0, &m_queue);

	// Find a suitable depth format
	VkBool32 validDepthFormat = Vulkan::getSupportedDepthFormat(m_physicalDevice, &m_depthFormat);
	assert(validDepthFormat);

	m_vulkanSwapChain.init(m_instance, m_physicalDevice, m_device);
	m_vulkanSwapChain.init(getHwnd(), getHinstance());
}



//Create a vulkan instance
void VulkanRender::init_instance()
{
   VkApplicationInfo appInfo = {};
   appInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   appInfo.pApplicationName = "Test";
   appInfo.pEngineName      = "Vulkan";
   appInfo.apiVersion       = VK_MAKE_VERSION(1, 0, 3);

   std::vector<const char*> enabledExtensions = {VK_KHR_SURFACE_EXTENSION_NAME};
#ifdef _WIN32
   enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#else
   // todo : linux/android
   enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
   VkInstanceCreateInfo instanceCreateInfo ={};
   instanceCreateInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   instanceCreateInfo.pNext            = NULL;
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


//Create a m_device
void VulkanRender::init_device(VkDeviceQueueCreateInfo requestedQueues)
{
	std::vector<const char*> enabledExtensions ={ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkDeviceCreateInfo deviceCreateInfo ={};
	deviceCreateInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext                = NULL;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos    = &requestedQueues;
	deviceCreateInfo.pEnabledFeatures     = NULL;

	if (enabledExtensions.size() > 0) {
		deviceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
	}
	if (m_enableValidation) {
		deviceCreateInfo.enabledLayerCount = Vulkan::Debug::validationLayerCount; // todo : validation layer names
		deviceCreateInfo.ppEnabledLayerNames = Vulkan::Debug::validationLayerNames;
	}

	auto res = vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);
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
	m_vulkanSwapChain.setup(m_setupCmdBuffer, &sw, &sh);
}

void VulkanRender::create_setupCmdBuffer()
{
	if (m_setupCmdBuffer != VK_NULL_HANDLE)
	{
		vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_setupCmdBuffer);
		m_setupCmdBuffer = VK_NULL_HANDLE; // todo : check if still necessary
	}

	VkCommandBufferAllocateInfo cmdBufAllocateInfo =
		Vulkan::Init::commandBufferAllocateInfo(
			m_commandPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			1);

	VkResult vkRes = vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, &m_setupCmdBuffer);
	assert(!vkRes);

	// todo : Command buffer is also started here, better put somewhere else
	// todo : Check if necessaray at all...
	VkCommandBufferBeginInfo cmdBufInfo ={};
	cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// todo : check null handles, flags?

	vkRes = vkBeginCommandBuffer(m_setupCmdBuffer, &cmdBufInfo);
	assert(!vkRes);
}

void VulkanRender::flush_setupCmdBuffer()
{
	VkResult err;

	if (m_setupCmdBuffer == VK_NULL_HANDLE)
		return;

	err = vkEndCommandBuffer(m_setupCmdBuffer);
	assert(!err);

	VkSubmitInfo submitInfo ={};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_setupCmdBuffer;

	err = vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
	assert(!err);

	err = vkQueueWaitIdle(m_queue);
	assert(!err);

	vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_setupCmdBuffer);
	m_setupCmdBuffer = VK_NULL_HANDLE; // todo : check if still necessary
}

void VulkanRender::destroy_cmdBuffers()
{
	vkFreeCommandBuffers(m_device, m_commandPool, (uint32_t)m_drawCmdBuffers.size(), m_drawCmdBuffers.data());
	vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_postPresentCmdBuffer);
}

void VulkanRender::init_commandBuffers()
{
// Create one command buffer per frame buffer in the swap chain
// Command buffers store a reference to the frame buffer inside their render pass info
// so for static usage withouth having to rebuild them each frame, we use one per frame buffer
	// Create one command buffer per frame buffer 
	// in the swap chain
	// Command buffers store a reference to the 
	// frame buffer inside their render pass info
	// so for static usage withouth having to rebuild 
	// them each frame, we use one per frame buffer

	m_drawCmdBuffers.resize(m_vulkanSwapChain.m_imageCount);

	VkCommandBufferAllocateInfo cmdBufAllocateInfo =
		Vulkan::Init::commandBufferAllocateInfo(
			m_commandPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			(uint32_t)m_drawCmdBuffers.size());

	VkResult vkRes = vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, m_drawCmdBuffers.data());
	assert(!vkRes);

	// Create one command buffer for submitting the
	// post present image memory barrier
	cmdBufAllocateInfo.commandBufferCount = 1;

	vkRes = vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, &m_postPresentCmdBuffer);
	assert(!vkRes);
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
	uint32_t sw = getClientWidth();
	uint32_t sh = getClientHeight();

	VkImageCreateInfo image ={};
	image.sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image.pNext       = NULL;
	image.imageType   = VK_IMAGE_TYPE_2D;
	image.format      = m_depthFormat;
	image.extent      = { sw, sh, 1 };
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
	Vulkan::getMemoryType(m_deviceMemoryProperties, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);
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

	for (uint32_t i = 0; i < m_frameBuffers.size(); i++) {
		attachments[0] = m_vulkanSwapChain.m_pBuffers[i].view;
		VkResult err = vkCreateFramebuffer(m_device, &frameBufferCreateInfo, nullptr, &m_frameBuffers[i]);
		assert(!err);
	}


}

void VulkanRender::build_commandBuffers()
{
	VkCommandBufferBeginInfo cmdBufInfo ={};
	cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufInfo.pNext = NULL;

	VkClearValue clearValues[2];
	clearValues[0].color = m_defaultClearColor;
	clearValues[1].depthStencil ={ 1.0f, 0 };

	VkRenderPassBeginInfo renderPassBeginInfo ={};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = nullptr;
	renderPassBeginInfo.renderPass = m_renderPass;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = getClientWidth();
	renderPassBeginInfo.renderArea.extent.height = getClientHeight();
	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValues;

	VkResult res = VK_SUCCESS;

	for (int32_t i = 0; i < m_drawCmdBuffers.size(); ++i) {
		//for target frame buffer
		renderPassBeginInfo.framebuffer = m_frameBuffers[i];

		res = vkBeginCommandBuffer(m_drawCmdBuffers[i], &cmdBufInfo);
		assert(!res);

		vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		//update dynamic viewport state
		VkViewport viewport ={};
		viewport.width  = static_cast<float>(getClientWidth());
		viewport.height = static_cast<float>(getClientHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_drawCmdBuffers[i], 0, 1, &viewport);

		//update dynamic scssor state
		VkRect2D scissor ={};
		scissor.extent.width = getClientWidth();
		scissor.extent.height = getClientWidth();
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(m_drawCmdBuffers[i], 0, 1, &scissor);

		m_cube.setupCmd(m_drawCmdBuffers[i]);

		vkCmdEndRenderPass(m_drawCmdBuffers[i]);

		//Add a present memory barrier to the end of the command buffer
		// This will transform the frame buffer color attachment to a 
		// new layout for presenting it to the windowing system integration
		VkImageMemoryBarrier prePresentBarrier ={};
		prePresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		prePresentBarrier.pNext = NULL;
		prePresentBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		prePresentBarrier.dstAccessMask = 0;
		prePresentBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		prePresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		prePresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.subresourceRange ={ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		prePresentBarrier.image = m_vulkanSwapChain.m_pBuffers[i].image;

		VkImageMemoryBarrier *pMemoryBarrier = &prePresentBarrier;
		vkCmdPipelineBarrier(
			m_drawCmdBuffers[i],
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_FLAGS_NONE,
			0, nullptr,
			0, nullptr,
			1, &prePresentBarrier);

		res = vkEndCommandBuffer(m_drawCmdBuffers[i]);
		assert(!res);
	}

}






}