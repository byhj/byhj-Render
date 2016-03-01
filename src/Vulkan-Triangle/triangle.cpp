#include "Triangle.h"

void Triangle::init()
{
	VulkanBase::prepare();

	initVertex();
	initUbo();
	initDescriptorSetLayout();
	initPipeline();
	initDescriptorPool();
	initDescriptorSet();
	buildCmdBuffers();

	prepared = true;
}

void Triangle::update()
{
	void updateUbo();
}

void Triangle::render()
{
	if (!prepared) {
	   return;
	}
	VkResult res = VK_SUCCESS;

	vkDeviceWaitIdle(device);
	VkSemaphore presentCompleteSemaphore;
	VkSemaphoreCreateInfo presentCompleteSemaphoreCreateInfo = {};
	presentCompleteSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	presentCompleteSemaphoreCreateInfo.pNext = nullptr;
	presentCompleteSemaphoreCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	res = vkCreateSemaphore(device, &presentCompleteSemaphoreCreateInfo, nullptr, &presentCompleteSemaphore);
	assert(!res);

	//Get next image in the swap chain (back/front buffer)
	res = swapChain.acquireNextImage(presentCompleteSemaphore, &currentBuffer);
	assert(!res);

	//The submit infor structure contains a list of command buffers and semaphores to 
	// be submitted to a queue, if you want to submit multiple command buffer, pass a array
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount =1;
	submitInfo.pWaitSemaphores = &presentCompleteSemaphore;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
	//Submit to the graphics queue
	res = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	assert(!res);

	//Present the current buffer to the swap chain, this will display the image
	res = swapChain.queuePresent(queue,  currentBuffer);
	assert(!res);

	vkDestroySemaphore(device, presentCompleteSemaphore, nullptr);


	//Add as post present image memory barrier, this will transform the frame buffer color 
	//attachment back to it's initial layout after it has been presented to the windowing system
	//see buildcCommandBuffers for the pre present barrier that does the opposite transformation
	VkImageMemoryBarrier postPresentBarrier = {};
	postPresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	postPresentBarrier.pNext = nullptr;
	postPresentBarrier.srcAccessMask = 0;
	postPresentBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	postPresentBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	postPresentBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	postPresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	postPresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	postPresentBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
	postPresentBarrier.image = swapChain.buffers[currentBuffer].image;

	//Use dedicated command buffer from example base class for submitting the post present barrier
	VkCommandBufferBeginInfo cmdBufferInfo = {};
	cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	res = vkBeginCommandBuffer(postPresentCmdBuffer, &cmdBufferInfo);
	assert(!res);

	//Put post present barrier into command buffer
	vkCmdPipelineBarrier(postPresentCmdBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		                 VK_FLAGS_NONE, 0, nullptr, 0, nullptr, 1, &postPresentBarrier);

	res = vkEndCommandBuffer(postPresentCmdBuffer);
	assert(!res);

	//Submit to the queue
	submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &postPresentCmdBuffer;
	res = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	assert(!res);
	res = vkQueueWaitIdle(queue);
	assert(!res);

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
	clearValues[1].depthStencil = {1.0f, 0};

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

void Triangle::initUbo()
{
	//Prepare and initialize uniform buffer containing shader uniforms

	//Vertex shader uniform buffer block
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.allocationSize = 0;
	allocInfo.memoryTypeIndex = 0;

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size  = sizeof(uniform);
	bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	//Create a new buffer
	VkResult res = VK_SUCCESS;
	res = vkCreateBuffer(device, &bufferInfo, nullptr, &uniform.buffer);
	assert(!res);


	//Get memory requirements including size, alignment and memory type
	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(device, uniform.buffer, &memReqs);
	allocInfo.allocationSize = memReqs.size;

	//Get the appropriate memory type for this type of buffer allocation
	//Only memory types that are visible to the host
	getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &allocInfo.memoryTypeIndex);
	//Allocate memory for the uniform buffer
	res = vkAllocateMemory(device, &allocInfo, nullptr, &uniform.memory);
	assert(!res);
    //Bind memory to buffer
	res = vkBindBufferMemory(device, uniform.buffer, uniform.memory, 0);
	assert(!res);

	//Store information in the uniform's descriptor
	uniform.desc.buffer = uniform.buffer;
	uniform.desc.offset = 0;
	uniform.desc.range  = sizeof(Uniform);

	updateUbo();

}
void Triangle::initDescriptorSetLayout()
{
	//Setup layout of descriptors used in this example
	//Basically connects the different shader stages to descriptors
	//for binding uniform buffers, image samplers, etc.
	//so every shader binding should map to one descriptor set layout binding
	
	//Binding 0: Uniform Buffer (Vertex Shader)
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	layoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo descSetLayoutInfo = {};
	descSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutInfo.pNext = nullptr;
	descSetLayoutInfo.bindingCount = 1;
	descSetLayoutInfo.pBindings = &layoutBinding;
    VkResult res = vkCreateDescriptorSetLayout(device, &descSetLayoutInfo, nullptr, &descriptorSetLayout);
	assert(!res);

	// Create the pipeline layout that is used to generate the rendering pipelines that are based on this
	// descriptor set layou 
	// In a more complex scenario you would have different pipeline layouts for different descriptor set 
	// Layouts that could be reused.
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = NULL;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	res = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
	assert(!res);
}

void Triangle::initPipeline()
{
	//Create out rendering pipeline used in this example
	//Vulkan uses the  concept of rendering pipelines to encapsulate fixed states
	//This replaces OpenGL's hube(and cumbersome) state machine
	//A pipeline is then storeed and hashed on the GPU making pipleine changes 
	//much faster than having to set dozens of states
	//In a real world application you'd have dozens of pipelines for every shader
	//set used in a scene
	//Note that there are a few states that are not stored with the pipeline
	//These are called dynamic  states and the pipeline only stores that they are
	//used with this pipeline, but not their states

	VkResult res = VK_SUCCESS;

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.layout = pipelineLayout;
	pipelineCreateInfo.renderPass = renderPass;

	//Vertex input state
	
	//Describes the topolo used with this pipeline
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
	inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	//Rasterization state
	VkPipelineRasterizationStateCreateInfo rasterizationState = {};
	rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
	//No culling
	rasterizationState.cullMode = VK_CULL_MODE_NONE;
	rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationState.depthClampEnable = VK_FALSE;
	rasterizationState.rasterizerDiscardEnable = VK_FALSE;
	rasterizationState.depthBiasEnable = VK_FALSE;

	//Color blend state
	//Describes blend modes and color masks
	VkPipelineColorBlendStateCreateInfo colorBlendState = {};
	colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	//One blend attachment state
	//Blending is not used in this example
	VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
	blendAttachmentState[0].colorWriteMask = 0xf;
	blendAttachmentState[0].blendEnable = VK_FALSE;
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = blendAttachmentState;

	//Viewport state
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	//Enable dynamic states
	//Descibes the dynamic states to be used with this pipeline
	//Dynamic states can be set even after pipeline has been created
	//So there is no need to create new pipelines just for changing
	//a viewport's demensions or a scissor box

	//The dynamic state properties themselves are stored in the command buffer
	std::vector<VkDynamicState> dynamicStatesEnables;
	dynamicStatesEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
	dynamicStatesEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);

	VkPipelineDynamicStateCreateInfo pipelineDynamicState = {};
	pipelineDynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipelineDynamicState.pDynamicStates = dynamicStatesEnables.data();
	pipelineDynamicState.dynamicStateCount = dynamicStatesEnables.size();

	//Depth and stencil state
	//Describes depth and stenctil test and compare ops
	// Basic depth compare setup with depth writes and depth test enabled,  No stencil used 
	VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilState.depthTestEnable = VK_TRUE;
	depthStencilState.depthWriteEnable = VK_TRUE;
	depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	depthStencilState.depthBoundsTestEnable = VK_FALSE;
	depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
	depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
	depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
	depthStencilState.stencilTestEnable = VK_FALSE;
	depthStencilState.front = depthStencilState.back;


   //Multi sampling state, No multi sampling used in this example
	VkPipelineMultisampleStateCreateInfo multiSampleState = {};
	multiSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multiSampleState.pSampleMask = nullptr;
	multiSampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	//Load Shaders
	VkPipelineShaderStageCreateInfo shaderStages[2] = { {}, {} };
#ifdef USE_GLSL
	shaderStages[0] = loadShaderGLSL("test.vert", VK_SHADER_STAGE_VERTEX_BIT);
	shaderStages[1] = loadShaderGLSL("test.frag", VK_SHADER_STAGE_FRAGMENT_BIT);
#else
	shaderStages[0] = loadShader("triangle.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	shaderStages[1] = loadShader("triangle.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
#endif

    //Assign states, two shader stages
	pipelineCreateInfo.stageCount = 2;
	pipelineCreateInfo.pVertexInputState = &vertices.vi;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	pipelineCreateInfo.pRasterizationState = &rasterizationState;
	pipelineCreateInfo.pColorBlendState = &colorBlendState;
	pipelineCreateInfo.pMultisampleState = &multiSampleState;
	pipelineCreateInfo.pViewportState = &viewportState;
	pipelineCreateInfo.pDepthStencilState = &depthStencilState;
	pipelineCreateInfo.pStages = shaderStages;
	pipelineCreateInfo.renderPass = renderPass;
	pipelineCreateInfo.pDynamicState = &pipelineDynamicState;

	res = vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &pipeline);
	assert(!res);
}

void Triangle::initDescriptorPool()
{
	//We need to tell the api the number of max requessted descriptors per type
	VkDescriptorPoolSize typeCounts[1];
	//This example only uses one descriptor type (uniform buffer) and only requests one descriptor of this type
	typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	typeCounts[0].descriptorCount = 1;

	//For additionnal types you need to add new entries in the type count list
	//E.g for two combined image samplers:
	// typeCounts[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// typeCounts[1].descriptorCount = 2;

	//Create the global descriptor pool
    //All descriptors used in this example are allocated form this pool
	VkDescriptorPoolCreateInfo descPoolInfo = {};
	descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descPoolInfo.pNext = nullptr;
	descPoolInfo.poolSizeCount = 1;
	descPoolInfo.pPoolSizes = typeCounts;
	// Set the max. number of sets that can be requested
    // Requesting descriptors beyond maxSets will result in an error
	descPoolInfo.maxSets = 1;

	VkResult res = vkCreateDescriptorPool(device, &descPoolInfo, nullptr, &descriptorPool);
	assert(!res);

}

void Triangle::initDescriptorSet()
{
	//Update descirptor sets determing the shader binding points 
	//For every binding point used in a shader there needs to be one
	//Descriptor set matching that binding point
	VkResult res = VK_SUCCESS;

	VkDescriptorSetAllocateInfo descAllocInfo = {};
	descAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descAllocInfo.descriptorPool = descriptorPool;
	descAllocInfo.descriptorSetCount = 1;
	descAllocInfo.pSetLayouts = &descriptorSetLayout;
	res = vkAllocateDescriptorSets(device, &descAllocInfo, &descriptorSet);
	assert(!res);

   //Binding 0: Uniform buffer
	VkWriteDescriptorSet writeDescSet  = {};
	writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSet.dstSet = descriptorSet;
	writeDescSet.descriptorCount = 1;
	writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescSet.pBufferInfo  = &uniform.desc;
	writeDescSet.dstBinding = 0;
	vkUpdateDescriptorSets(device, 1, &writeDescSet, 0, nullptr);
}


void Triangle::updateUbo()
{

	matrix.model = glm::mat4(1.0f);
	matrix.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix.proj  = glm::perspective(45.0f, static_cast<float>(width)/height, 0.1f, 1000.0f);

	//Map uniform buffer and update it
	uint8_t *pData;
	VkResult res = vkMapMemory(device, uniform.memory, 0, sizeof(matrix), 0, (void**)&pData);
	assert(!res);
	memcpy(pData, &matrix, sizeof(matrix));
	vkUnmapMemory(device, uniform.memory);

}