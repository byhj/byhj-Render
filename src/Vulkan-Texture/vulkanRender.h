#ifndef VulkanRender_H
#define VulkanRender_H

#include "vk/vulkanApp.h"
#include "vk/vulkanSwapChain.h"
#include "cube.h"

#include <vector>

namespace byhj {
  
	class VulkanRender : public VulkanApp {
	  public:
		  VulkanRender() = default;
		  ~VulkanRender() = default;

		  void v_init() override;
		  void v_update() override;
		  void v_render() override;
		  void v_shutdown() override;

		  struct VulkanDepthStencil {
			  VkImage image;
			  VkDeviceMemory mem;
			  VkImageView view;
		  };

   	 private:
		 void init_vulkan();
		 void init_instance();
		 void init_device(VkDeviceQueueCreateInfo requestedQueues);
		 void init_commandPool();
		 void init_swapchain();
		 void init_commandBuffers();
		 void init_depthStencil();
		 void init_renderpass();
		 void init_pipelineCache();
		 void init_framebuffer();

		 void flush_setupCmdBuffer();
		 void create_setupCmdBuffer();
		 void destroy_cmdBuffers();
		 void build_commandBuffers();

		 bool m_enableValidation = false;
		 VkClearColorValue m_defaultClearColor ={ { 0.02f, 0.03f, 0.04f, 1.0f } };

		 VkDevice         m_device;
		 VkQueue          m_queue;
		 VkInstance       m_instance;
		 VkRenderPass     m_renderPass;

		 VkCommandPool    m_commandPool;
		 VkCommandBuffer  m_setupCmdBuffer ;
		 VkCommandBuffer  m_postPresentCmdBuffer;

		 VkDescriptorPool   m_descriptorPool;
		 VkPipelineCache    m_pipelineCache;
		 VkPipelineLayout   m_pipelineLayout;

		 VkPhysicalDevice m_physicalDevice;
		 VkPhysicalDeviceMemoryProperties m_deviceMemoryProperties;

		 VulkanSwapChain    m_vulkanSwapChain;
		 VulkanDepthStencil m_depthStencil;

		 std::vector<VkCommandBuffer> m_drawCmdBuffers;
		 std::vector<VkFramebuffer>  m_frameBuffers;

		 VkFormat m_colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
		 VkFormat m_depthFormat;
		 uint32_t m_currentBuffer = 0;

		 Cube m_cube;


  };
}
#endif