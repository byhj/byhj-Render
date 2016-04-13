#include "vulkanShader.h"

namespace byhj {

    void VulkanShader::init(VkDevice device)
	{
		m_device = device;
	}

	void VulkanShader::end()
	{

		for (auto& shaderModule : m_shaderModules) {
			vkDestroyShaderModule(m_device, shaderModule, nullptr);
		}
	}
	void VulkanShader::loadSPIR(VkShaderStageFlagBits stage, const char* fileName)
	{
		VkPipelineShaderStageCreateInfo shaderStage ={};
		shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStage.stage = stage;
		shaderStage.module = Vulkan::loadShader(fileName, m_device, stage);
		shaderStage.pName = "main"; // todo : make param
		assert(shaderStage.module != NULL);
		m_shaderModules.push_back(shaderStage.module);
		m_shaderStages.push_back(shaderStage);
	}

	void VulkanShader::loadGLSL(VkShaderStageFlagBits stage, const char* fileName)
	{
		VkPipelineShaderStageCreateInfo shaderStage ={};
		shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStage.stage = stage;
		shaderStage.module = Vulkan::loadShaderGLSL(fileName, m_device, stage);
		shaderStage.pName = "main"; // todo : make param
		assert(shaderStage.module != NULL);
		m_shaderModules.push_back(shaderStage.module);
        m_shaderStages.push_back(shaderStage);
	}

	std::vector<VkPipelineShaderStageCreateInfo> VulkanShader::getStages()
	{
		return m_shaderStages;
	}
}