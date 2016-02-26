#pragma once
/*
 * Vulkan Samples
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 * Copyright (C) 2015-2016 Google, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "util.h"

///////////////////////////////////// Init ///////////////////////////////////
VkResult init_global_extension_properties(Vulkan::LayerProperties &layer_props);
VkResult init_global_layer_properties(Sample &info);
VkResult init_device_extension_properties(Sample &info,Vulkan::LayerProperties &layer_props);
VkResult init_device_layer_properties(Sample &info);

void init_instance_extension_names(Sample &info);
VkResult init_instance(Sample &info, char const *const app_short_name);
void init_device_extension_names(Sample &info);
VkResult init_device(Sample &info);
VkResult init_enumerate_device(Sample &info, uint32_t gpu_count = 1);
VkBool32 demo_check_layers(const std::vector<Vulkan::LayerProperties> &layer_props,
	                       const std::vector<const char *> &layer_names);

void init_connection(Sample &info);
void init_window(Sample &info);
void init_queue_family_index(Sample &info);
void init_presentable_image(Sample &info);

void init_swapchain_extension(Sample &info);
void init_command_pool(Sample &info);
void init_command_buffer(Sample &info);
void init_device_queue(Sample &info);
void init_swap_chain(Sample &info);
void init_depth_buffer(Sample &info);
void init_uniform_buffer(Sample &info);
void init_descriptor_and_pipeline_layouts(Sample &info, bool use_texture);
void init_renderpass(Sample &info, bool include_depth, bool clear = true);
void init_vertex_buffer(Sample &info, const void *vertexData, 
	                    uint32_t dataSize, uint32_t dataStride, bool use_texture);

void init_framebuffers(Sample &info, bool include_depth);
void init_descriptor_pool(Sample &info, bool use_texture);
void init_descriptor_set(Sample &info, bool use_texture);
void init_shaders(Sample &info, const char *vertShaderText, const char *fragShaderText);

void init_pipeline_cache(Sample &info);
void init_pipeline(Sample &info, VkBool32 include_depth, VkBool32 include_vi = true);
void init_sampler(Sample &info, VkSampler &sampler);
void init_image(Sample &info, Vulkan::Texture &texObj, const char *textureName);
void init_texture(Sample &info, const char *textureName = nullptr);
void init_viewports(Sample &info);
void init_scissors(Sample &info);
void init_fence(Sample &info, VkFence &fence);
void init_submit_info(Sample &info, VkSubmitInfo &submit_info, VkPipelineStageFlags &pipe_stage_flags);
void init_present_info(Sample &info, VkPresentInfoKHR &present);
void init_clear_color_and_depth(Sample &info, VkClearValue *clear_values);
void init_render_pass_begin_info(Sample &info, VkRenderPassBeginInfo &rp_begin);
void init_window_size(Sample &info, int32_t default_width, int32_t default_height);

VkResult init_debug_report_callback(Sample &info, PFN_vkDebugReportCallbackEXT dbgFunc);

/////////////////////////////////////  Execute ////////////////////////////////////


void execute_queue_cmdbuf(Sample &info, const VkCommandBuffer *cmd_bufs);
void execute_pre_present_barrier(Sample &info);
void execute_present_image(Sample &info);

void execute_begin_command_buffer(Sample &info);
void execute_end_command_buffer(Sample &info);
void execute_queue_command_buffer(Sample &info);



//////////////////////////////////////// Destroy //////////////////////////////////////

void destroy_debug_report_callback(Sample &info);
void destroy_pipeline(Sample &info);
void destroy_pipeline_cache(Sample &info);
void destroy_descriptor_pool(Sample &info);
void destroy_vertex_buffer(Sample &info);
void destroy_textures(Sample &info);
void destroy_framebuffers(Sample &info);
void destroy_shaders(Sample &info);
void destroy_renderpass(Sample &info);
void destroy_descriptor_and_pipeline_layouts(Sample &info);
void destroy_uniform_buffer(Sample &info);
void destroy_depth_buffer(Sample &info);
void destroy_swap_chain(Sample &info);
void destroy_command_buffer(Sample &info);
void destroy_command_pool(Sample &info);
void destroy_device(Sample &info);
void destroy_instance(Sample &info);
void destroy_window(Sample &info);