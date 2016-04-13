
#ifndef Triangle_H
#define Triangle_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.h>

#define VERTEX_BUFFER_BIND_ID 0
// Note : 
//	Enabling this define will feed GLSL directly to the driver
//	Unlike the SDK samples that convert it to SPIR-V
//	This may or may not be supported depending on your ISV
//#define USE_GLSL
// Set to "true" to enable Vulkan's validation layers
// See vulkandebug.cpp for details
#define ENABLE_VALIDATION false

#include "vk/vulkanShader.h"

namespace byhj {

    class Triangle 
    {
    public:
    	Triangle() = default;
    	~Triangle() = default;
    
    	struct  Vertex
    	{
    		VkBuffer buffer;
    		VkDeviceMemory memory;
    		VkPipelineVertexInputStateCreateInfo inputStateInfo;
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
    
    	struct MVPMatrix {
    	   glm::mat4 model;
    	   glm::mat4 view;
    	   glm::mat4 proj;
    	};
    

    
    
    	void init(VkDevice device);
    	void update();
    	void render();
    	void shutdown();
         
		void setupCmd(const VkCommandBuffer drawCmdBuffer);
		void init_pipeline(VkRenderPass renderPass,VkPipelineCache pipelineCache);

    private: 
    	void init_vertex();
		void init_ubo();

        void init_descriptorPool();
    	void init_descriptorSet();
    	void init_descriptorSetLayout();
    
    	void update_ubo();

		VkDevice  m_device;

		Vertex    m_vertices;
		Index     m_indices;
		Uniform   m_uniform;
		MVPMatrix m_matrix;

		VkPipeline            m_pipeline;
		VkPipelineLayout      m_pipelineLayout;
		VkDescriptorPool      m_descriptorPool;
		VkDescriptorSet       m_descriptorSet;
		VkDescriptorSetLayout m_descriptorSetLayout;
		VulkanShader m_triangleShader;

    };


}

#endif