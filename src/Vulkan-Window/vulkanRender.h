#ifndef VulkanRender_H
#define VulkanRender_H

#include "vk/vulkanApp.h"

namespace byhj {
  
	class VulkanRender : public VulkanApp {
	  public:
		  void v_init() override;
		  void v_update() override;
		  void v_render() override;
		  void v_shutdown() override;

   	 private:


  };
}
#endif