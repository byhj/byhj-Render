#include "Triangle.h"

void Triangle::init()
{
	VulkanBase::prepare();

	void initVertex();
	void initUbo();
	void initPipeline();
	void initDescriptorPool();
	void initDescriptorSet();
	void initDescriptorSetLayout();
	void buildCmdBuffers();

	prepared = true;
}

void Triangle::update()
{
	void updateUbo();
}

void Triangle::render()
{
	if (prepared) {
	   return;
	}

	vkDeviceWaitIdle(device);

	vkDeviceWaitIdle(device);
}

void Triangle::shutdown()
{
}

// Build separate command buffers for every framebuffer image
// Unlike in OpenGL all rendering commands are recorded once
// into command buffers that are then resubmitted to the queue
void Triangle::buildCmdBuffers()
{
	VkCommandBufferBeginInfo cmdBufInfo = {};
	cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufInfo.pNext = NULL;

	VkClearValue clearValues[2];
	clearValues[0].color = defaultClearColor;
	clearValues[1].depthStencil = {1.0f, 0.0f};

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = nullptr;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = width;
	renderPassBeginInfo.renderArea.extent.height = height;
	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValues;

	VkResult res = VK_SUCCESS;

	for (int32_t i = 0; i < drawCmdBuffers.size(); ++i) {
	  //for target frame buffer
		renderPassBeginInfo.framebuffer = frameBuffers[i];

		res = vkBeginCommandBuffer(drawCmdBuffers[i], &cmdBufInfo);
		assert(!res);

		vkCmdBeginRenderPass(drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
         
		//update dynamic viewport state
		VkViewport viewport = {};
		viewport.width  = static_cast<float>(width);
		viewport.height = static_cast<float>(height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(drawCmdBuffers[i], 0, 1, &viewport);

       //update dynamic scssor state
		VkRect2D scissor = {};
		scissor.extent.width = width;
		scissor.extent.height = height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(drawCmdBuffers[i], 0, 1, &scissor);

        //Bing desciptor sets describing shader binding points
		vkCmdBindDescriptorSets(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
			                    0, 1, &descriptorSet, 0, NULL);

		//Bind the rendering pipeline (including the shaders)
		vkCmdBindPipeline(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		//Bind Triangle vertices
		VkDeviceSize offsets[1] = {0};
		vkCmdBindVertexBuffers(drawCmdBuffers[i], VERTEX_BUFFER_BIND_ID, 1, &vertices.buffer, offsets);

		//Bind Triangle indices
		vkCmdBindIndexBuffer(drawCmdBuffers[i], indices.buffer, 0, VK_INDEX_TYPE_UINT32);

		//Draw indexed Triangle
		vkCmdDrawIndexed(drawCmdBuffers[i], indices.count, 1, 0, 0, 1);

		vkCmdEndRenderPass(drawCmdBuffers[i]);

		//Add a present memory barrier to the end of the command buffer
		// This will transform the frame buffer color attachment to a 
		// new layout for presenting it to the windowing system integration
		VkImageMemoryBarrier prePresentBarrier = {};
		prePresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		prePresentBarrier.pNext = NULL;
		prePresentBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		prePresentBarrier.dstAccessMask = 0;
		prePresentBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		prePresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		prePresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
		prePresentBarrier.image = swapChain.buffers[i].image;

		VkImageMemoryBarrier *pMemoryBarrier = &prePresentBarrier;
		vkCmdPipelineBarrier( 
			drawCmdBuffers[i],
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_FLAGS_NONE,
			0, nullptr,
			0, nullptr,
			1, &prePresentBarrier);
		res = vkEndCommandBuffer(drawCmdBuffers[i]);
		assert(!res);
	}


}

//Setups vertex and index buffers for an indexed triangle
//uploads them to the vram and sets binding points and attribute
//descriptions to match locations inside the shaders

void Triangle::initVertex()
{
	struct Vertex {
		float pos[3];
		float col[3];
	};

	// Setup vertices
	std::vector<Vertex> vertexBuffer ={
		{ { 1.0f,  1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
		{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } }
	};

	auto vertexBufferSize = vertexBuffer.size() * sizeof(Vertex);

	//Setup indices
	std::vector<uint32_t> indexBuffer = {0, 1, 2};
	int indexBufferSize = indexBuffer.size() * sizeof(uint32_t);

	VkMemoryAllocateInfo memoryAlloc = {};
	memoryAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAlloc.pNext = NULL;
	memoryAlloc.allocationSize = 0;
	memoryAlloc.memoryTypeIndex = 0;

	VkMemoryRequirements memoryReqs;
	VkResult res;
	void *data = nullptr;

	//Generate vertex buffer 
	//Setup
	VkBufferCreateInfo bufferIno = {};
	bufferIno.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferIno.pNext = nullptr;
	bufferIno.size  = vertexBufferSize;
	bufferIno.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferIno.flags = 0;

	//Copy vertex data to VRAM
     memset(&vertices, 0, sizeof(vertices));
	 res = vkCreateBuffer(device, &bufferIno, nullptr, &vertices.buffer);
	 assert(!res);

	 vkGetBufferMemoryRequirements(device, vertices.buffer, &memoryReqs);
	 memoryAlloc.allocationSize = memoryReqs.size;
	 getMemoryType(memoryReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,  &memoryAlloc.memoryTypeIndex);
	 vkAllocateMemory(device, &memoryAlloc, nullptr, &vertices.memory);
	 
	 res = vkMapMemory(device, vertices.memory, 0, memoryAlloc.allocationSize, 0, &data);
	 assert(!res);
	 memcpy(data, vertexBuffer.data(), vertexBufferSize);
	 vkUnmapMemory(device, vertices.memory);
	  
	 vkBindBufferMemory(device, vertices.buffer, vertices.memory, 0);

	 //Generate index buffer
	 VkBufferCreateInfo indexbufferInfo = {};
	 indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	 indexbufferInfo.pNext = nullptr;
	 indexbufferInfo.size  = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	 indexbufferInfo.flags = 0;

	 //Copy index data  to vram
	 memset(&indices, 0, sizeof(indices));
	 res = vkCreateBuffer(device, &indexbufferInfo, nullptr, &indices.buffer);
	 assert(!res);
	 vkGetBufferMemoryRequirements(device, indices.buffer, &memoryReqs);
	 memoryAlloc.allocationSize = memoryReqs.size;
	 getMemoryType(memoryReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memoryAlloc.memoryTypeIndex);
	 res = vkAllocateMemory(device, &memoryAlloc, nullptr, &indices.memory);
	 assert(!res);
	 
	 res = vkMapMemory(device, indices.memory, 0, indexBufferSize, 0, &data);
	 assert(!res);
	 memcpy(data, indexBuffer.data(), indexBufferSize);
	 vkUnmapMemory(device, indices.memory);
	 res = vkBindBufferMemory(device, indices.buffer, indices.memory, 0);
	 assert(!res);
     indices.count = indexBuffer.size();

     //Bind description
	 vertices.bindingDescs.resize(1);
	 vertices.bindingDescs[0].binding = VERTEX_BUFFER_BIND_ID;
	 vertices.bindingDescs[0].stride  = sizeof(Vertex);
	 vertices.bindingDescs[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	 //Attribute descriptions
	 //Describes memory layout and shader attribute locations
	 vertices.attributeDescs.resize(2);
	 //Location 0: Position
	 vertices.attributeDescs[0].binding = VERTEX_BUFFER_BIND_ID;
	 vertices.attributeDescs[0].location = 0;
	 vertices.attributeDescs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	 vertices.attributeDescs[0].binding = 0;

	 //Location 1: Color
	 vertices.attributeDescs[1].binding = VERTEX_BUFFER_BIND_ID;
	 vertices.attributeDescs[1].location = 1;
	 vertices.attributeDescs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	 vertices.attributeDescs[1].offset = sizeof(float) * 3;
	 vertices.attributeDescs[1].binding = 0;

	 //Assign to vertex buffer
	 vertices.vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	 vertices.vi.pNext = nullptr;
	 vertices.vi.vertexBindingDescriptionCount = vertices.bindingDescs.size();
	 vertices.vi.pVertexBindingDescriptions  = vertices.bindingDescs.data();
	 vertices.vi.vertexAttributeDescriptionCount = vertices.attributeDescs.size();
	 vertices.vi.pVertexAttributeDescriptions = vertices.attributeDescs.data();

}
void Triangle::initPipeline()
{
}
void Triangle::initUbo()
{
}

void Triangle::initDescriptorPool()
{
}
void Triangle::initDescriptorSet()
{
}
void Triangle::initDescriptorSetLayout()
{
}

void Triangle::updateUbo()
{
}