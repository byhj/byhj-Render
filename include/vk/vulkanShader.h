#ifndef VulkanShader_H
#define VulkanShader_H

#include <vulkan/vulkan.h>
#include "vk/vulkanUtility.h"

namespace byhj {

    class VulkanShader {
	public:

		void init(VkDevice device);
		void end();

		// Load a SPIR-V shader
		void loadSPIR(VkShaderStageFlagBits stage, const char* fileName);
		
		// Load a GLSL shader, NOTE : This may not work with any IHV and requires some magic
		void loadGLSL(VkShaderStageFlagBits stage, const char* fileName);
	
		std::vector<VkPipelineShaderStageCreateInfo> getStages();

	private:
		std::vector<VkShaderModule> m_shaderModules;
		std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
		VkDevice m_device;
	};
}

#endif