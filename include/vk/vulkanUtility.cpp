#include "vulkanUtility.h"

/*
* Assorted commonly used Vulkan helper functions
*
* Copyright (C) 2015 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "vulkanUtility.h"

#ifdef __ANDROID__
#include "vulkanandroid.h"
#endif


namespace Vulkan
{

/*
// Compile a given string containing GLSL into SPV for use by VK
// Return value of false means an error was encountered.
EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type) {
	switch (shader_type) {
	case VK_SHADER_STAGE_VERTEX_BIT:
		return EShLangVertex;

	case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
		return EShLangTessControl;

	case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
		return EShLangTessEvaluation;

	case VK_SHADER_STAGE_GEOMETRY_BIT:
		return EShLangGeometry;

	case VK_SHADER_STAGE_FRAGMENT_BIT:
		return EShLangFragment;

	case VK_SHADER_STAGE_COMPUTE_BIT:
		return EShLangCompute;

	default:
		return EShLangVertex;
	}
}
void init_resources(TBuiltInResource &Resources) {
	Resources.maxLights = 32;
	Resources.maxClipPlanes = 6;
	Resources.maxTextureUnits = 32;
	Resources.maxTextureCoords = 32;
	Resources.maxVertexAttribs = 64;
	Resources.maxVertexUniformComponents = 4096;
	Resources.maxVaryingFloats = 64;
	Resources.maxVertexTextureImageUnits = 32;
	Resources.maxCombinedTextureImageUnits = 80;
	Resources.maxTextureImageUnits = 32;
	Resources.maxFragmentUniformComponents = 4096;
	Resources.maxDrawBuffers = 32;
	Resources.maxVertexUniformVectors = 128;
	Resources.maxVaryingVectors = 8;
	Resources.maxFragmentUniformVectors = 16;
	Resources.maxVertexOutputVectors = 16;
	Resources.maxFragmentInputVectors = 15;
	Resources.minProgramTexelOffset = -8;
	Resources.maxProgramTexelOffset = 7;
	Resources.maxClipDistances = 8;
	Resources.maxComputeWorkGroupCountX = 65535;
	Resources.maxComputeWorkGroupCountY = 65535;
	Resources.maxComputeWorkGroupCountZ = 65535;
	Resources.maxComputeWorkGroupSizeX = 1024;
	Resources.maxComputeWorkGroupSizeY = 1024;
	Resources.maxComputeWorkGroupSizeZ = 64;
	Resources.maxComputeUniformComponents = 1024;
	Resources.maxComputeTextureImageUnits = 16;
	Resources.maxComputeImageUniforms = 8;
	Resources.maxComputeAtomicCounters = 8;
	Resources.maxComputeAtomicCounterBuffers = 1;
	Resources.maxVaryingComponents = 60;
	Resources.maxVertexOutputComponents = 64;
	Resources.maxGeometryInputComponents = 64;
	Resources.maxGeometryOutputComponents = 128;
	Resources.maxFragmentInputComponents = 128;
	Resources.maxImageUnits = 8;
	Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
	Resources.maxCombinedShaderOutputResources = 8;
	Resources.maxImageSamples = 0;
	Resources.maxVertexImageUniforms = 0;
	Resources.maxTessControlImageUniforms = 0;
	Resources.maxTessEvaluationImageUniforms = 0;
	Resources.maxGeometryImageUniforms = 0;
	Resources.maxFragmentImageUniforms = 8;
	Resources.maxCombinedImageUniforms = 8;
	Resources.maxGeometryTextureImageUnits = 16;
	Resources.maxGeometryOutputVertices = 256;
	Resources.maxGeometryTotalOutputComponents = 1024;
	Resources.maxGeometryUniformComponents = 1024;
	Resources.maxGeometryVaryingComponents = 64;
	Resources.maxTessControlInputComponents = 128;
	Resources.maxTessControlOutputComponents = 128;
	Resources.maxTessControlTextureImageUnits = 16;
	Resources.maxTessControlUniformComponents = 1024;
	Resources.maxTessControlTotalOutputComponents = 4096;
	Resources.maxTessEvaluationInputComponents = 128;
	Resources.maxTessEvaluationOutputComponents = 128;
	Resources.maxTessEvaluationTextureImageUnits = 16;
	Resources.maxTessEvaluationUniformComponents = 1024;
	Resources.maxTessPatchComponents = 120;
	Resources.maxPatchVertices = 32;
	Resources.maxTessGenLevel = 64;
	Resources.maxViewports = 16;
	Resources.maxVertexAtomicCounters = 0;
	Resources.maxTessControlAtomicCounters = 0;
	Resources.maxTessEvaluationAtomicCounters = 0;
	Resources.maxGeometryAtomicCounters = 0;
	Resources.maxFragmentAtomicCounters = 8;
	Resources.maxCombinedAtomicCounters = 8;
	Resources.maxAtomicCounterBindings = 1;
	Resources.maxVertexAtomicCounterBuffers = 0;
	Resources.maxTessControlAtomicCounterBuffers = 0;
	Resources.maxTessEvaluationAtomicCounterBuffers = 0;
	Resources.maxGeometryAtomicCounterBuffers = 0;
	Resources.maxFragmentAtomicCounterBuffers = 1;
	Resources.maxCombinedAtomicCounterBuffers = 1;
	Resources.maxAtomicCounterBufferSize = 16384;
	Resources.maxTransformFeedbackBuffers = 4;
	Resources.maxTransformFeedbackInterleavedComponents = 64;
	Resources.maxCullDistances = 8;
	Resources.maxCombinedClipAndCullDistances = 8;
	Resources.maxSamples = 4;
	Resources.limits.nonInductiveForLoops = 1;
	Resources.limits.whileLoops = 1;
	Resources.limits.doWhileLoops = 1;
	Resources.limits.generalUniformIndexing = 1;
	Resources.limits.generalAttributeMatrixVectorIndexing = 1;
	Resources.limits.generalVaryingIndexing = 1;
	Resources.limits.generalSamplerIndexing = 1;
	Resources.limits.generalVariableIndexing = 1;
	Resources.limits.generalConstantMatrixVectorIndexing = 1;
}

	bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader,
		std::vector<unsigned int> &spirv) {
		EShLanguage stage = FindLanguage(shader_type);
		glslang::TShader shader(stage);
		glslang::TProgram program;
		const char *shaderStrings[1];
		TBuiltInResource Resources;
		init_resources(Resources);

		// Enable SPIR-V and Vulkan rules when parsing GLSL
		EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

		shaderStrings[0] = pshader;
		shader.setStrings(shaderStrings, 1);

		if (!shader.parse(&Resources, 100, false, messages)) {
			puts(shader.getInfoLog());
			puts(shader.getInfoDebugLog());
			return false; // something didn't work
		}

		program.addShader(&shader);

		//
		// Program-level processing...
		//

		if (!program.link(messages)) {
			puts(shader.getInfoLog());
			puts(shader.getInfoDebugLog());
			fflush(stdout);
			return false;
		}

		glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);

		return true;
	}
*/
	VkBool32 getMemoryType(VkPhysicalDeviceMemoryProperties  dmp,  uint32_t typeBits, VkFlags properties, uint32_t * typeIndex)
	{
		static 	VkPhysicalDeviceMemoryProperties deviceMemoryProperties = dmp;
		for (uint32_t i = 0; i < 32; i++) {
			if ((typeBits & 1) == 1) {
				if ((deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					*typeIndex = i;
					return true;
				}
			}
			typeBits >>= 1;
		}
		return false;
	}

	VkBool32 checkGlobalExtensionPresent(const char* extensionName)
	{
		uint32_t extensionCount = 0;
		std::vector<VkExtensionProperties> extensions;
		vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
		extensions.resize(extensionCount);
		vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions.data());
		for (auto& ext : extensions)
		{
			if (!strcmp(extensionName, ext.extensionName))
			{
				return true;
			}
		}
		return false;
	}

	VkBool32 checkDeviceExtensionPresent(VkPhysicalDevice physicalDevice, const char* extensionName)
	{
		uint32_t extensionCount = 0;
		std::vector<VkExtensionProperties> extensions;
		vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, NULL);
		extensions.resize(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &extensionCount, extensions.data());
		for (auto& ext : extensions)
		{
			if (!strcmp(extensionName, ext.extensionName))
			{
				return true;
			}
		}
		return false;
	}

	std::string errorString(VkResult errorCode)
	{
		switch (errorCode)
		{
			// todo : update to SDK 0.10.1
#define STR(r) case VK_ ##r: return #r
			STR(NOT_READY);
			STR(TIMEOUT);
			STR(EVENT_SET);
			STR(EVENT_RESET);
			STR(INCOMPLETE);
			STR(ERROR_OUT_OF_HOST_MEMORY);
			STR(ERROR_OUT_OF_DEVICE_MEMORY);
			STR(ERROR_INITIALIZATION_FAILED);
			STR(ERROR_DEVICE_LOST);
			STR(ERROR_MEMORY_MAP_FAILED);
			STR(ERROR_LAYER_NOT_PRESENT);
			STR(ERROR_EXTENSION_NOT_PRESENT);
			STR(ERROR_INCOMPATIBLE_DRIVER);
#undef STR
		default:
			return "UNKNOWN_ERROR";
		}
	}

	VkBool32 getSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat *depthFormat)
	{
		// Since all depth formats may be optional, we need to find a suitable depth format to use
		// Start with the highest precision packed format
		std::vector<VkFormat> depthFormats ={
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};

		for (auto& format : depthFormats)
		{
			VkFormatProperties formatProps;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
			// Format must support depth stencil attachment for optimal tiling
			if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				*depthFormat = format;
				return true;
			}
		}

		return false;
	}

	// Create an image memory barrier for changing the layout of
	// an image and put it into an active command buffer
	// See chapter 11.4 "Image Layout" for details
	//todo : rename
	void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout)
	{
		// Create an image barrier object
		VkImageMemoryBarrier imageMemoryBarrier = Init::imageMemoryBarrier();
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
		imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
		imageMemoryBarrier.subresourceRange.levelCount = 1;
		imageMemoryBarrier.subresourceRange.layerCount = 1;

		// Source layouts (old)

		// Undefined layout
		// Only allowed as initial layout!
		// Make sure any writes to the image have been finished
		if (oldImageLayout == VK_IMAGE_LAYOUT_UNDEFINED)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
		}

		// Old layout is color attachment
		// Make sure any writes to the color buffer have been finished
		if (oldImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}

		// Old layout is transfer source
		// Make sure any reads from the image have been finished
		if (oldImageLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		}

		// Old layout is shader read (sampler, input attachment)
		// Make sure any shader reads from the image have been finished
		if (oldImageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		}

		// Target layouts (new)

		// New layout is transfer destination (copy, blit)
		// Make sure any copyies to the image have been finished
		if (newImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		}

		// New layout is transfer source (copy, blit)
		// Make sure any reads from and writes to the image have been finished
		if (newImageLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = imageMemoryBarrier.srcAccessMask | VK_ACCESS_TRANSFER_READ_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		}

		// New layout is color attachment
		// Make sure any writes to the color buffer hav been finished
		if (newImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		}

		// New layout is depth attachment
		// Make sure any writes to depth/stencil buffer have been finished
		if (newImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		}

		// New layout is shader read (sampler, input attachment)
		// Make sure any writes to the image have been finished
		if (newImageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		}


		// Put barrier on top
		VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		VkPipelineStageFlags destStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

		// Put barrier inside setup command buffer
		vkCmdPipelineBarrier(
			cmdbuffer,
			srcStageFlags,
			destStageFlags,
			0,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier);
	}

	void exitFatal(std::string message, std::string caption)
	{
#ifdef _WIN32
		MessageBox(NULL, message.c_str(), caption.c_str(), MB_OK | MB_ICONERROR);
#else
		// TODO : Linux
#endif
		std::cerr << message << "\n";
		exit(1);
	}

	std::string readTextFile(const char *fileName)
	{
		std::string fileContent;
		std::ifstream fileStream(fileName, std::ios::in);
		if (!fileStream.is_open()) {
			printf("File %s not found\n", fileName);
			return "";
		}
		std::string line = "";
		while (!fileStream.eof()) {
			getline(fileStream, line);
			fileContent.append(line + "\n");
		}
		fileStream.close();
		return fileContent;
	}

	// Load a binary file into a buffer (e.g. SPIR-V)
	char *readBinaryFile(const char *filename, size_t *psize)
	{
		long int size;
		size_t retval;
		void *shader_code;

		FILE *fp = fopen(filename, "rb");
		if (!fp) return NULL;

		fseek(fp, 0L, SEEK_END);
		size = ftell(fp);

		fseek(fp, 0L, SEEK_SET);

		shader_code = malloc(size);
		retval = fread(shader_code, size, 1, fp);
		assert(retval == 1);

		*psize = size;

		return (char*)shader_code;
	}

	VkShaderModule loadShader(const char *fileName, VkDevice device, VkShaderStageFlagBits stage)
	{
		size_t size = 0;
		const char *shaderCode = readBinaryFile(fileName, &size);
		assert(size > 0);

		VkShaderModule shaderModule;
		VkShaderModuleCreateInfo moduleCreateInfo;
		VkResult err;

		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfo.pNext = NULL;

		moduleCreateInfo.codeSize = size;
		moduleCreateInfo.pCode = (uint32_t*)shaderCode;
		moduleCreateInfo.flags = 0;
		err = vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderModule);
		assert(!err);

		return shaderModule;
	}


	VkShaderModule loadShaderGLSL(const char *fileName, VkDevice device, VkShaderStageFlagBits stage)
	{
		std::string shaderSrc = readTextFile(fileName);
		const char *shaderCode = shaderSrc.c_str();
		size_t size = strlen(shaderCode);
		assert(size > 0);

		VkShaderModule shaderModule;
		VkShaderModuleCreateInfo moduleCreateInfo;
		VkResult err;

		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfo.pNext = NULL;

		moduleCreateInfo.codeSize = 3 * sizeof(uint32_t) + size + 1;
		moduleCreateInfo.pCode = (uint32_t*)malloc(moduleCreateInfo.codeSize);
		moduleCreateInfo.flags = 0;

		// Magic SPV number
		((uint32_t *)moduleCreateInfo.pCode)[0] = 0x07230203;
		((uint32_t *)moduleCreateInfo.pCode)[1] = 0;
		((uint32_t *)moduleCreateInfo.pCode)[2] = stage;
		memcpy(((uint32_t *)moduleCreateInfo.pCode + 3), shaderCode, size + 1);

		err = vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderModule);
		assert(!err);

		return shaderModule;
	}

	VkImageMemoryBarrier prePresentBarrier(VkImage presentImage)
	{
		VkImageMemoryBarrier imageMemoryBarrier = Init::imageMemoryBarrier();
		imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		imageMemoryBarrier.dstAccessMask = 0;
		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.subresourceRange ={ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		imageMemoryBarrier.image = presentImage;
		return imageMemoryBarrier;
	}

	VkImageMemoryBarrier postPresentBarrier(VkImage presentImage)
	{
		VkImageMemoryBarrier imageMemoryBarrier = Init::imageMemoryBarrier();
		imageMemoryBarrier.srcAccessMask = 0;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.subresourceRange ={ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		imageMemoryBarrier.image = presentImage;
		return imageMemoryBarrier;
	}

	void destroyUniformData(VkDevice device, UniformData *uniformData)
	{
		vkDestroyBuffer(device, uniformData->buffer, nullptr);
		vkFreeMemory(device, uniformData->memory, nullptr);
	}

VkMemoryAllocateInfo Init::memoryAllocateInfo()
{
	VkMemoryAllocateInfo memAllocInfo ={};
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.pNext = NULL;
	memAllocInfo.allocationSize = 0;
	memAllocInfo.memoryTypeIndex = 0;
	return memAllocInfo;
}

VkCommandBufferAllocateInfo Init::commandBufferAllocateInfo(VkCommandPool commandPool, VkCommandBufferLevel level, uint32_t bufferCount)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo ={};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = level;
	commandBufferAllocateInfo.commandBufferCount = bufferCount;
	return commandBufferAllocateInfo;
}

VkCommandBufferBeginInfo Init::commandBufferBeginInfo()
{
	VkCommandBufferBeginInfo cmdBufferBeginInfo ={};
	cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufferBeginInfo.pNext = NULL;
	return cmdBufferBeginInfo;
}

VkRenderPassBeginInfo Init::renderPassBeginInfo()
{
	VkRenderPassBeginInfo renderPassBeginInfo ={};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = NULL;
	return renderPassBeginInfo;
}

VkImageMemoryBarrier Init::imageMemoryBarrier()
{
	VkImageMemoryBarrier imageMemoryBarrier ={};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.pNext = NULL;
	// Some default values
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	return imageMemoryBarrier;
}

VkBufferMemoryBarrier Init::bufferMemoryBarrier()
{
	VkBufferMemoryBarrier bufferMemoryBarrier ={};
	bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	bufferMemoryBarrier.pNext = NULL;
	return bufferMemoryBarrier;
}

VkMemoryBarrier Init::memoryBarrier()
{
	VkMemoryBarrier memoryBarrier ={};
	memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
	memoryBarrier.pNext = NULL;
	return memoryBarrier;
}

VkImageCreateInfo Init::imageCreateInfo()
{
	VkImageCreateInfo imageCreateInfo ={};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = NULL;
	return imageCreateInfo;
}

VkSamplerCreateInfo Init::samplerCreateInfo()
{
	VkSamplerCreateInfo samplerCreateInfo ={};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.pNext = NULL;
	return samplerCreateInfo;
}

VkImageViewCreateInfo Init::imageViewCreateInfo()
{
	VkImageViewCreateInfo imageViewCreateInfo ={};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = NULL;
	return imageViewCreateInfo;
}

VkFramebufferCreateInfo Init::framebufferCreateInfo()
{
	VkFramebufferCreateInfo framebufferCreateInfo ={};
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.pNext = NULL;
	return framebufferCreateInfo;
}

VkSemaphoreCreateInfo Init::semaphoreCreateInfo(
	VkSemaphoreCreateFlags flags)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo ={};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = NULL;
	semaphoreCreateInfo.flags = flags;
	return semaphoreCreateInfo;
}

VkSubmitInfo Init::submitInfo()
{
	VkSubmitInfo submitInfo ={};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = NULL;
	return submitInfo;
}

VkViewport Init::viewport(
	float width,
	float height,
	float minDepth,
	float maxDepth)
{
	VkViewport viewport ={};
	viewport.width = width;
	viewport.height = height;
	viewport.minDepth = minDepth;
	viewport.maxDepth = maxDepth;
	return viewport;
}

VkRect2D Init::rect2D(
	int32_t width,
	int32_t height,
	int32_t offsetX,
	int32_t offsetY)
{
	VkRect2D rect2D ={};
	rect2D.extent.width = width;
	rect2D.extent.height = height;
	rect2D.offset.x = offsetX;
	rect2D.offset.y = offsetY;
	return rect2D;
}

VkBufferCreateInfo Init::bufferCreateInfo(
	VkBufferUsageFlags usage,
	VkDeviceSize size)
{
	VkBufferCreateInfo bufCreateInfo ={};
	bufCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufCreateInfo.pNext = NULL;
	bufCreateInfo.usage = usage;
	bufCreateInfo.size = size;
	bufCreateInfo.flags = 0;
	return bufCreateInfo;
}

VkDescriptorPoolCreateInfo Init::descriptorPoolCreateInfo(
	uint32_t poolSizeCount,
	VkDescriptorPoolSize* pPoolSizes,
	uint32_t maxSets)
{
	VkDescriptorPoolCreateInfo descriptorPoolInfo ={};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.pNext = NULL;
	descriptorPoolInfo.poolSizeCount = poolSizeCount;
	descriptorPoolInfo.pPoolSizes = pPoolSizes;
	descriptorPoolInfo.maxSets = maxSets;
	return descriptorPoolInfo;
}

VkDescriptorPoolSize Init::descriptorPoolSize(
	VkDescriptorType type,
	uint32_t descriptorCount)
{
	VkDescriptorPoolSize descriptorPoolSize ={};
	descriptorPoolSize.type = type;
	descriptorPoolSize.descriptorCount = descriptorCount;
	return descriptorPoolSize;
}

VkDescriptorSetLayoutBinding Init::descriptorSetLayoutBinding(
	VkDescriptorType type,
	VkShaderStageFlags stageFlags,
	uint32_t binding)
{
	VkDescriptorSetLayoutBinding setLayoutBinding ={};
	setLayoutBinding.descriptorType = type;
	setLayoutBinding.stageFlags = stageFlags;
	setLayoutBinding.binding = binding;
	// Default value in all examples
	setLayoutBinding.descriptorCount = 1;
	return setLayoutBinding;
}

VkDescriptorSetLayoutCreateInfo Init::descriptorSetLayoutCreateInfo(
	const VkDescriptorSetLayoutBinding* pBindings,
	uint32_t bindingCount)
{
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo ={};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = NULL;
	descriptorSetLayoutCreateInfo.pBindings = pBindings;
	descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
	return descriptorSetLayoutCreateInfo;
}

VkPipelineLayoutCreateInfo Init::pipelineLayoutCreateInfo(
	const VkDescriptorSetLayout* pSetLayouts,
	uint32_t setLayoutCount)
{
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo ={};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = NULL;
	pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
	pipelineLayoutCreateInfo.pSetLayouts = pSetLayouts;
	return pipelineLayoutCreateInfo;
}

VkDescriptorSetAllocateInfo Init::descriptorSetAllocateInfo(
	VkDescriptorPool descriptorPool,
	const VkDescriptorSetLayout* pSetLayouts,
	uint32_t descriptorSetCount)
{
	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo ={};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = NULL;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;
	descriptorSetAllocateInfo.pSetLayouts = pSetLayouts;
	descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
	return descriptorSetAllocateInfo;
}

VkDescriptorImageInfo Init::descriptorImageInfo(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout)
{
	VkDescriptorImageInfo descriptorImageInfo ={};
	descriptorImageInfo.sampler = sampler;
	descriptorImageInfo.imageView = imageView;
	descriptorImageInfo.imageLayout = imageLayout;
	return descriptorImageInfo;
}

VkWriteDescriptorSet Init::writeDescriptorSet(
	VkDescriptorSet dstSet,
	VkDescriptorType type,
	uint32_t binding,
	VkDescriptorBufferInfo* bufferInfo)
{
	VkWriteDescriptorSet writeDescriptorSet ={};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = NULL;
	writeDescriptorSet.dstSet = dstSet;
	writeDescriptorSet.descriptorType = type;
	writeDescriptorSet.dstBinding = binding;
	writeDescriptorSet.pBufferInfo = bufferInfo;
	// Default value in all examples
	writeDescriptorSet.descriptorCount = 1;
	return writeDescriptorSet;
}

VkWriteDescriptorSet Init::writeDescriptorSet(
	VkDescriptorSet dstSet,
	VkDescriptorType type,
	uint32_t binding,
	VkDescriptorImageInfo * imageInfo)
{
	VkWriteDescriptorSet writeDescriptorSet ={};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = NULL;
	writeDescriptorSet.dstSet = dstSet;
	writeDescriptorSet.descriptorType = type;
	writeDescriptorSet.dstBinding = binding;
	writeDescriptorSet.pImageInfo = imageInfo;
	// Default value in all examples
	writeDescriptorSet.descriptorCount = 1;
	return writeDescriptorSet;
}

VkVertexInputBindingDescription Init::vertexInputBindingDescription(
	uint32_t binding,
	uint32_t stride,
	VkVertexInputRate inputRate)
{
	VkVertexInputBindingDescription vInputBindDescription ={};
	vInputBindDescription.binding = binding;
	vInputBindDescription.stride = stride;
	vInputBindDescription.inputRate = inputRate;
	return vInputBindDescription;
}

VkVertexInputAttributeDescription Init::vertexInputAttributeDescription(
	uint32_t binding,
	uint32_t location,
	VkFormat format,
	uint32_t offset)
{
	VkVertexInputAttributeDescription vInputAttribDescription ={};
	vInputAttribDescription.location = location;
	vInputAttribDescription.binding = binding;
	vInputAttribDescription.format = format;
	vInputAttribDescription.offset = offset;
	return vInputAttribDescription;
}

VkPipelineVertexInputStateCreateInfo Init::pipelineVertexInputStateCreateInfo()
{
	VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo ={};
	pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	pipelineVertexInputStateCreateInfo.pNext = NULL;
	return pipelineVertexInputStateCreateInfo;
}

VkPipelineInputAssemblyStateCreateInfo Init::pipelineInputAssemblyStateCreateInfo(
	VkPrimitiveTopology topology,
	VkPipelineInputAssemblyStateCreateFlags flags,
	VkBool32 primitiveRestartEnable)
{
	VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo ={};
	pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	pipelineInputAssemblyStateCreateInfo.topology = topology;
	pipelineInputAssemblyStateCreateInfo.flags = flags;
	pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
	return pipelineInputAssemblyStateCreateInfo;
}

VkPipelineRasterizationStateCreateInfo Init::pipelineRasterizationStateCreateInfo(
	VkPolygonMode polygonMode,
	VkCullModeFlags cullMode,
	VkFrontFace frontFace,
	VkPipelineRasterizationStateCreateFlags flags)
{
	VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo ={};
	pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	pipelineRasterizationStateCreateInfo.polygonMode = polygonMode;
	pipelineRasterizationStateCreateInfo.cullMode = cullMode;
	pipelineRasterizationStateCreateInfo.frontFace = frontFace;
	pipelineRasterizationStateCreateInfo.flags = flags;
	pipelineRasterizationStateCreateInfo.depthClampEnable = VK_TRUE;
	return pipelineRasterizationStateCreateInfo;
}

VkPipelineColorBlendAttachmentState Init::pipelineColorBlendAttachmentState(
	VkColorComponentFlags colorWriteMask,
	VkBool32 blendEnable)
{
	VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState ={};
	pipelineColorBlendAttachmentState.colorWriteMask = colorWriteMask;
	pipelineColorBlendAttachmentState.blendEnable = blendEnable;
	return pipelineColorBlendAttachmentState;
}

VkPipelineColorBlendStateCreateInfo Init::pipelineColorBlendStateCreateInfo(
	uint32_t attachmentCount,
	const VkPipelineColorBlendAttachmentState * pAttachments)
{
	VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo ={};
	pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	pipelineColorBlendStateCreateInfo.pNext = NULL;
	pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount;
	pipelineColorBlendStateCreateInfo.pAttachments = pAttachments;
	return pipelineColorBlendStateCreateInfo;
}

VkPipelineDepthStencilStateCreateInfo Init::pipelineDepthStencilStateCreateInfo(
	VkBool32 depthTestEnable,
	VkBool32 depthWriteEnable,
	VkCompareOp depthCompareOp)
{
	VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo ={};
	pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
	pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
	pipelineDepthStencilStateCreateInfo.depthCompareOp = depthCompareOp;
	pipelineDepthStencilStateCreateInfo.front = pipelineDepthStencilStateCreateInfo.back;
	pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
	return pipelineDepthStencilStateCreateInfo;
}

VkPipelineViewportStateCreateInfo Init::pipelineViewportStateCreateInfo(
	uint32_t viewportCount,
	uint32_t scissorCount,
	VkPipelineViewportStateCreateFlags flags)
{
	VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo ={};
	pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	pipelineViewportStateCreateInfo.viewportCount = viewportCount;
	pipelineViewportStateCreateInfo.scissorCount = scissorCount;
	pipelineViewportStateCreateInfo.flags = flags;
	return pipelineViewportStateCreateInfo;
}

VkPipelineMultisampleStateCreateInfo Init::pipelineMultisampleStateCreateInfo(
	VkSampleCountFlagBits rasterizationSamples,
	VkPipelineMultisampleStateCreateFlags flags)
{
	VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo ={};
	pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	pipelineMultisampleStateCreateInfo.rasterizationSamples = rasterizationSamples;
	return pipelineMultisampleStateCreateInfo;
}

VkPipelineDynamicStateCreateInfo Init::pipelineDynamicStateCreateInfo(
	const VkDynamicState * pDynamicStates,
	uint32_t dynamicStateCount,
	VkPipelineDynamicStateCreateFlags flags)
{
	VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo ={};
	pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	pipelineDynamicStateCreateInfo.pDynamicStates = pDynamicStates;
	pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
	return pipelineDynamicStateCreateInfo;
}

VkPipelineTessellationStateCreateInfo Init::pipelineTessellationStateCreateInfo(uint32_t patchControlPoints)
{
	VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo ={};
	pipelineTessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	pipelineTessellationStateCreateInfo.patchControlPoints = patchControlPoints;
	return pipelineTessellationStateCreateInfo;
}

VkGraphicsPipelineCreateInfo Init::pipelineCreateInfo(
	VkPipelineLayout layout,
	VkRenderPass renderPass,
	VkPipelineCreateFlags flags)
{
	VkGraphicsPipelineCreateInfo pipelineCreateInfo ={};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.pNext = NULL;
	pipelineCreateInfo.layout = layout;
	pipelineCreateInfo.renderPass = renderPass;
	pipelineCreateInfo.flags = flags;
	return pipelineCreateInfo;
}

VkComputePipelineCreateInfo Init::computePipelineCreateInfo(VkPipelineLayout layout, VkPipelineCreateFlags flags)
{
	VkComputePipelineCreateInfo computePipelineCreateInfo ={};
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.layout = layout;
	computePipelineCreateInfo.flags = flags;
	return computePipelineCreateInfo;
}

VkPushConstantRange Init::pushConstantRange(
	VkShaderStageFlags stageFlags,
	uint32_t size,
	uint32_t offset)
{
	VkPushConstantRange pushConstantRange ={};
	pushConstantRange.stageFlags = stageFlags;
	pushConstantRange.offset = offset;
	pushConstantRange.size = size;
	return pushConstantRange;
}

}