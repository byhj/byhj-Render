#include "base.h"
#include "cube_data.h"

static Sample m_info;

void Base::init()
{
	VkResult U_ASSERT_ONLY res;
	char sample_title[] = "Draw Cube";
	const bool depthPresent = true;
	init_global_layer_properties(m_info);
	init_instance_extension_names(m_info);
	init_device_extension_names(m_info);
	init_instance(m_info, sample_title);
	init_enumerate_device(m_info);
	init_window_size(m_info, 500, 500);
	init_connection(m_info);
	init_window(m_info);
	init_swapchain_extension(m_info);
	init_device(m_info);

	init_command_pool(m_info);
	init_command_buffer(m_info);
	execute_begin_command_buffer(m_info);
	init_device_queue(m_info);
	init_swap_chain(m_info);
	init_depth_buffer(m_info);
	init_uniform_buffer(m_info);
	init_descriptor_and_pipeline_layouts(m_info, false);
	init_renderpass(m_info, depthPresent);
	init_shaders(m_info, vertShaderText, fragShaderText);
	init_framebuffers(m_info, depthPresent);
	init_vertex_buffer(m_info, g_vb_solid_face_colors_Data,
		sizeof(g_vb_solid_face_colors_Data),
		sizeof(g_vb_solid_face_colors_Data[0]), false);
	init_descriptor_pool(m_info, false);
	init_descriptor_set(m_info, false);
	init_pipeline_cache(m_info);
	init_pipeline(m_info, depthPresent);
}


void Base::render()
{
	/* VULKAN_KEY_START */
	VkResult res;
	VkClearValue clear_values[2];
	clear_values[0].color.float32[0] = 0.2f;
	clear_values[0].color.float32[1] = 0.2f;
	clear_values[0].color.float32[2] = 0.2f;
	clear_values[0].color.float32[3] = 0.2f;
	clear_values[1].depthStencil.depth = 1.0f;
	clear_values[1].depthStencil.stencil = 0;

	VkSemaphore presentCompleteSemaphore;
	VkSemaphoreCreateInfo presentCompleteSemaphoreCreateInfo;
	presentCompleteSemaphoreCreateInfo.sType =
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	presentCompleteSemaphoreCreateInfo.pNext = NULL;
	presentCompleteSemaphoreCreateInfo.flags = 0;

	res = vkCreateSemaphore(m_info.device, &presentCompleteSemaphoreCreateInfo,
		NULL, &presentCompleteSemaphore);
	assert(res == VK_SUCCESS);

	// Get the index of the next available swapchain image:
	res = vkAcquireNextImageKHR(m_info.device, m_info.swap_chain, UINT64_MAX,
		presentCompleteSemaphore, NULL,
		&m_info.current_buffer);
	// TODO: Deal with the VK_SUBOPTIMAL_KHR and VK_ERROR_OUT_OF_DATE_KHR
	// return codes
	assert(res == VK_SUCCESS);

	VkRenderPassBeginInfo rp_begin;
	rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rp_begin.pNext = NULL;
	rp_begin.renderPass = m_info.renderPass;
	rp_begin.framebuffer = m_info.framebuffers[m_info.current_buffer];
	rp_begin.renderArea.offset.x = 0;
	rp_begin.renderArea.offset.y = 0;
	rp_begin.renderArea.extent.width = m_info.width;
	rp_begin.renderArea.extent.height = m_info.height;
	rp_begin.clearValueCount = 2;
	rp_begin.pClearValues = clear_values;

	vkCmdBeginRenderPass(m_info.cmd, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(m_info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_info.pipeline);
	vkCmdBindDescriptorSets(m_info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_info.pipelineLayout, 0, NUM_DESCRIPTOR_SETS,
		m_info.desc_set.data(), 0, NULL);

	const VkDeviceSize offsets[1] ={ 0 };
	vkCmdBindVertexBuffers(m_info.cmd, 0, 1, &m_info.vertexBuffer.buffer, offsets);

	init_viewports(m_info);
	init_scissors(m_info);

	vkCmdDraw(m_info.cmd, 12 * 3, 1, 0, 0);
	vkCmdEndRenderPass(m_info.cmd);

	VkImageMemoryBarrier prePresentBarrier ={};
	prePresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	prePresentBarrier.pNext = NULL;
	prePresentBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	prePresentBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	prePresentBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	prePresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	prePresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	prePresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	prePresentBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	prePresentBarrier.subresourceRange.baseMipLevel = 0;
	prePresentBarrier.subresourceRange.levelCount = 1;
	prePresentBarrier.subresourceRange.baseArrayLayer = 0;
	prePresentBarrier.subresourceRange.layerCount = 1;
	prePresentBarrier.image = m_info.buffers[m_info.current_buffer].image;
	vkCmdPipelineBarrier(m_info.cmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, NULL, 0,
		NULL, 1, &prePresentBarrier);

	res = vkEndCommandBuffer(m_info.cmd);
	const VkCommandBuffer cmd_bufs[] ={ m_info.cmd };
	VkFenceCreateInfo fenceInfo;
	VkFence drawFence;
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = NULL;
	fenceInfo.flags = 0;
	vkCreateFence(m_info.device, &fenceInfo, NULL, &drawFence);

	VkPipelineStageFlags pipe_stage_flags =
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	VkSubmitInfo submit_info[1] ={};
	submit_info[0].pNext = NULL;
	submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info[0].waitSemaphoreCount = 1;
	submit_info[0].pWaitSemaphores = &presentCompleteSemaphore;
	submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
	submit_info[0].commandBufferCount = 1;
	submit_info[0].pCommandBuffers = cmd_bufs;
	submit_info[0].signalSemaphoreCount = 0;
	submit_info[0].pSignalSemaphores = NULL;

	/* Queue the command buffer for execution */
	res = vkQueueSubmit(m_info.queue, 1, submit_info, drawFence);
	assert(res == VK_SUCCESS);

	/* Now present the image in the window */

	VkPresentInfoKHR present;
	present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present.pNext = NULL;
	present.swapchainCount = 1;
	present.pSwapchains = &m_info.swap_chain;
	present.pImageIndices = &m_info.current_buffer;
	present.pWaitSemaphores = NULL;
	present.waitSemaphoreCount = 0;
	present.pResults = NULL;

	/* Make sure command buffer is finished before presenting */
	do {
		res =
			vkWaitForFences(m_info.device, 1, &drawFence, VK_TRUE, FENCE_TIMEOUT);
	} while (res == VK_TIMEOUT);

	assert(res == VK_SUCCESS);
	res = vkQueuePresentKHR(m_info.queue, &present);
	assert(res == VK_SUCCESS);

	wait_seconds(1);
	/* VULKAN_KEY_END */
}

void Base::shutdown()
{
	destroy_pipeline(m_info);
	destroy_pipeline_cache(m_info);
	destroy_descriptor_pool(m_info);
	destroy_vertex_buffer(m_info);
	destroy_framebuffers(m_info);
	destroy_shaders(m_info);
	destroy_renderpass(m_info);
	destroy_descriptor_and_pipeline_layouts(m_info);
	destroy_uniform_buffer(m_info);
	destroy_depth_buffer(m_info);
	destroy_swap_chain(m_info);
	destroy_command_buffer(m_info);
	destroy_command_pool(m_info);
	destroy_window(m_info);
	destroy_device(m_info);
	destroy_instance(m_info);
}