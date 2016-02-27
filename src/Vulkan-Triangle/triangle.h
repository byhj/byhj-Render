#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.h>
#include "vulkanBase.h"

#define VERTEX_BUFFER_BIND_ID 0
// Note : 
//	Enabling this define will feed GLSL directly to the driver
//	Unlike the SDK samples that convert it to SPIR-V
//	This may or may not be supported depending on your ISV
//#define USE_GLSL
// Set to "true" to enable Vulkan's validation layers
// See vulkandebug.cpp for details
#define ENABLE_VALIDATION false

class Triangle : public VulkanBase
{
public:
	Triangle() :VulkanBase(ENABLE_VALIDATION)
	{
		width = 1280;
		height = 720;
		zoom = -2.5f;
		title = "Vulkan Example - Basic indexed triangle";
		// Values not set here are initialized in the base class constructor
	}

	~Triangle() {
		// Clean up used Vulkan resources 
       // Note : Inherited destructor cleans up resources stored in base class
		vkDestroyPipeline(device, pipeline, nullptr);

		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

		vkDestroyBuffer(device, vertices.buffer, nullptr);
		vkFreeMemory(device, vertices.memory, nullptr);

		vkDestroyBuffer(device, indices.buffer, nullptr);
		vkFreeMemory(device, indices.memory, nullptr);

		vkDestroyBuffer(device, uniform.buffer, nullptr);
		vkFreeMemory(device, uniform.memory, nullptr);
	}

	struct  Vertex
	{
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkPipelineVertexInputStateCreateInfo vi;
		std::vector<VkVertexInputBindingDescription>   bindingDescs;
		std::vector<VkVertexInputAttributeDescription> attributeDescs;
	};
	
	struct Index {
		int count;
		VkBuffer buffer;
		VkDeviceMemory memory;
	};

	struct Uniform {
	   VkBuffer buffer;
	   VkDeviceMemory memory;
	   VkDescriptorBufferInfo desc;
	};

	struct Matrix {
	   glm::mat4 model;
	   glm::mat4 view;
	   glm::mat4 proj;
	};

	Vertex vertices;
	Index  indices;
	Uniform uniform;
	Matrix matrix;

	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
	VkDescriptorSet descriptorSet;
	VkDescriptorSetLayout descriptorSetLayout;


	void init();
	void update();
	void render() override;
	void shutdown();

private:

	void buildCmdBuffers();

	void initVertex();
	void initPipeline();
	void initUbo();

    void initDescriptorPool();
	void initDescriptorSet();
	void initDescriptorSetLayout();

	void updateUbo();

};
