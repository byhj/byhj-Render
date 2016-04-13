 #include "Cube.h"

namespace byhj {

void Cube::init(VkDevice device)
{
	m_device = device;
	m_triangleShader.init(device);
	init_vertex();
	init_ubo();
	init_texture();

	init_descriptorSetLayout();

	init_descriptorPool();
	init_descriptorSet();
}

void Cube::update()
{
	void update_ubo();
}

void Cube::render()
{

}

void Cube::shutdown()
{
	// Clean up used Vulkan resources 
// Note : Inherited destructor cleans up resources stored in base class
	vkDestroyPipeline(m_device, m_pipeline, nullptr);

	vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
	vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);

	vkDestroyBuffer(m_device, m_vertices.buffer, nullptr);
	vkFreeMemory(m_device, m_vertices.memory, nullptr);

	vkDestroyBuffer(m_device, m_indices.buffer, nullptr);
	vkFreeMemory(m_device, m_indices.memory, nullptr);

	vkDestroyBuffer(m_device, m_uniform.buffer, nullptr);
	vkFreeMemory(m_device, m_uniform.memory, nullptr);
}


void Cube::setupCmd(const VkCommandBuffer drawCmdBuffer)
{
	//Bing desciptor sets describing shader binding points
	vkCmdBindDescriptorSets(drawCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout,
		0, 1, &m_descriptorSet, 0, NULL);

	//Bind the rendering pipeline (including the shaders)
	vkCmdBindPipeline(drawCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

	//Bind Cube vertices
	VkDeviceSize offsets[1] ={ 0 };
	vkCmdBindVertexBuffers(drawCmdBuffer, VERTEX_BUFFER_BIND_ID, 1, &m_vertices.buffer, offsets);

	//Bind Cube indices
	vkCmdBindIndexBuffer(drawCmdBuffer, m_indices.buffer, 0, VK_INDEX_TYPE_UINT32);

	//Draw indexed Cube
	vkCmdDrawIndexed(drawCmdBuffer, m_indices.count, 1, 0, 0, 0);
}


//Setups vertex and index buffers for an indexed Cube
//uploads them to the vram and sets binding points and attribute
//descriptions to match locations inside the shaders

void Cube::init_vertex()
{
	struct Vertex {
		float position[3];
		float texcoord[2];
	};

#define dim 1.0f
	// Setup m_vertices
	std::vector<Vertex> vertexBuffer ={
		{ { dim,  dim, 0.0f },{ 1.0f, 1.0f } },
		{ { -dim,  dim, 0.0f },{ 0.0f, 1.0f } },
		{ { -dim, -dim, 0.0f },{ 0.0f, 0.0f } },
		{ { dim, -dim, 0.0f },{ 1.0f, 0.0f } }
	};

	auto vertexBufferSize = vertexBuffer.size() * sizeof(Vertex);

	//Setup m_indices
	std::vector<uint32_t> indexBuffer = { 0,1,2, 2,3,0 };
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
     memset(&m_vertices, 0, sizeof(m_vertices));
	 res = vkCreateBuffer(m_device, &bufferIno, nullptr, &m_vertices.buffer);
	 assert(!res);

	 vkGetBufferMemoryRequirements(m_device, m_vertices.buffer, &memoryReqs);
	 memoryAlloc.allocationSize = memoryReqs.size;
	 Vulkan::getMemoryType({}, memoryReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,  &memoryAlloc.memoryTypeIndex);
	 vkAllocateMemory(m_device, &memoryAlloc, nullptr, &m_vertices.memory);
	 
	 res = vkMapMemory(m_device, m_vertices.memory, 0, memoryAlloc.allocationSize, 0, &data);
	 assert(!res);
	 memcpy(data, vertexBuffer.data(), vertexBufferSize);
	 vkUnmapMemory(m_device, m_vertices.memory);
	  
	 vkBindBufferMemory(m_device, m_vertices.buffer, m_vertices.memory, 0);

	 //Generate index buffer
	 VkBufferCreateInfo indexbufferInfo = {};
	 indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	 indexbufferInfo.pNext = nullptr;
	 indexbufferInfo.size  = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	 indexbufferInfo.flags = 0;

	 //Copy index data  to vram
	 memset(&m_indices, 0, sizeof(m_indices));
	 res = vkCreateBuffer(m_device, &indexbufferInfo, nullptr, &m_indices.buffer);
	 assert(!res);
	 vkGetBufferMemoryRequirements(m_device, m_indices.buffer, &memoryReqs);
	 memoryAlloc.allocationSize = memoryReqs.size;
	 Vulkan::getMemoryType({}, memoryReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &memoryAlloc.memoryTypeIndex);
	 res = vkAllocateMemory(m_device, &memoryAlloc, nullptr, &m_indices.memory);
	 assert(!res);
	 
	 res = vkMapMemory(m_device, m_indices.memory, 0, indexBufferSize, 0, &data);
	 assert(!res);
	 memcpy(data, indexBuffer.data(), indexBufferSize);
	 vkUnmapMemory(m_device, m_indices.memory);
	 res = vkBindBufferMemory(m_device, m_indices.buffer, m_indices.memory, 0);
	 assert(!res);
     m_indices.count = indexBuffer.size();

     //Bind description
	 m_vertices.bindingDescs.resize(1);
	 m_vertices.bindingDescs[0].binding   = VERTEX_BUFFER_BIND_ID;
	 m_vertices.bindingDescs[0].stride    = sizeof(Vertex);
	 m_vertices.bindingDescs[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	 //Attribute descriptions
	 //Describes memory layout and shader attribute locations
	 m_vertices.attributeDescs.resize(2);
	 //Location 0: Position
	 m_vertices.attributeDescs[0].binding  = VERTEX_BUFFER_BIND_ID;
	 m_vertices.attributeDescs[0].location = 0;
	 m_vertices.attributeDescs[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
	 m_vertices.attributeDescs[0].offset   = 0;

	 //Location 1: Color
	 m_vertices.attributeDescs[1].binding  = VERTEX_BUFFER_BIND_ID;
	 m_vertices.attributeDescs[1].location = 1;
	 m_vertices.attributeDescs[1].format   = VK_FORMAT_R32G32_SFLOAT;
	 m_vertices.attributeDescs[1].offset   = sizeof(float) * 3;


	 //Assign to vertex buffer
	 m_vertices.inputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	 m_vertices.inputStateInfo.pNext = nullptr;
	 m_vertices.inputStateInfo.vertexBindingDescriptionCount   = m_vertices.bindingDescs.size();
	 m_vertices.inputStateInfo.pVertexBindingDescriptions      = m_vertices.bindingDescs.data();
	 m_vertices.inputStateInfo.vertexAttributeDescriptionCount = m_vertices.attributeDescs.size();
	 m_vertices.inputStateInfo.pVertexAttributeDescriptions    = m_vertices.attributeDescs.data();

}

void Cube::init_ubo()
{
	//Prepare and initialize uniform buffer containing shader uniforms

	//Vertex shader uniform buffer block
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext           = nullptr;
	allocInfo.allocationSize  = 0;
	allocInfo.memoryTypeIndex = 0;

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size  = sizeof(m_uniform);
	bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	//Create a new buffer
	VkResult res = VK_SUCCESS;
	res = vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_uniform.buffer);
	assert(!res);


	//Get memory requirements including size, alignment and memory type
	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(m_device, m_uniform.buffer, &memReqs);
	allocInfo.allocationSize = memReqs.size;
	 
	//Get the appropriate memory type for this type of buffer allocation
	//Only memory types that are visible to the host
	Vulkan::getMemoryType({}, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &allocInfo.memoryTypeIndex);
	//Allocate memory for the uniform buffer
	res = vkAllocateMemory(m_device, &allocInfo, nullptr, &m_uniform.memory);
	assert(!res);
    //Bind memory to buffer
	res = vkBindBufferMemory(m_device, m_uniform.buffer, m_uniform.memory, 0);
	assert(!res);

	//Store information in the uniform's descriptor
	m_uniform.desc.buffer = m_uniform.buffer;
	m_uniform.desc.offset = 0;
	m_uniform.desc.range  = sizeof(Uniform);

	update_ubo();

}
void Cube::init_texture()
{
	VulkanTextureLoader::getInstance()->loadTexture(
		"igor_and_pal_bc3.ktx",
		VK_FORMAT_BC3_UNORM_BLOCK,
		&m_texture);
}

void Cube::init_descriptorSetLayout()
{
	//Setup layout of descriptors used in this example
	//Basically connects the different shader stages to descriptors
	//for binding uniform buffers, image samplers, etc.
	//so every shader binding should map to one descriptor set layout binding
	

	std::vector<VkDescriptorSetLayoutBinding> layoutBindings(2);

	//Binding 0: Uniform Buffer (Vertex Shader)
	layoutBindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBindings[0].descriptorCount    = 1;
	layoutBindings[0].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
	layoutBindings[0].binding = 0;
	layoutBindings[0].pImmutableSamplers = nullptr;

	// Binding 1 : Fragment shader image sampler
	layoutBindings[1].descriptorType     = 	VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
	layoutBindings[1].descriptorCount    = 1;
	layoutBindings[1].stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
	layoutBindings[1].pImmutableSamplers = nullptr;
	layoutBindings[1].binding = 1;

	VkDescriptorSetLayoutCreateInfo descSetLayoutInfo = {};
	descSetLayoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutInfo.pNext        = nullptr;
	descSetLayoutInfo.bindingCount = layoutBindings.size();
	descSetLayoutInfo.pBindings    = &layoutBindings[0];
    VkResult res = vkCreateDescriptorSetLayout(m_device, &descSetLayoutInfo, nullptr, &m_descriptorSetLayout);
	assert(!res);

	// Create the pipeline layout that is used to generate the rendering pipelines that are based on this
	// descriptor set layout
	// In a more complex scenario you would have different pipeline layouts for different descriptor set 
	// Layouts that could be reused.
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext          = NULL;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts    = &m_descriptorSetLayout;

	res = vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
	assert(!res);
}

void Cube::init_pipeline(VkRenderPass &renderPass, VkPipelineCache &pipelineCache)
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
	blendAttachmentState[0].blendEnable    = VK_FALSE;
	colorBlendState.attachmentCount        = 1;
	colorBlendState.pAttachments           = blendAttachmentState;

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


#ifdef USE_GLSL
	m_triangleShader.loadGLSL(VK_SHADER_STAGE_VERTEX_BIT, "texture.vert");
	m_triangleShader.loadGLSL(VK_SHADER_STAGE_FRAGMENT_BIT, "texture.frag");
#else
	m_triangleShader.loadSPIR( VK_SHADER_STAGE_VERTEX_BIT, "Cube.vert.spv");
	m_triangleShader.loadSPIR( VK_SHADER_STAGE_FRAGMENT_BIT, "Cube.frag.spv");
#endif
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = m_triangleShader.getStages();

	VkGraphicsPipelineCreateInfo pipelineCreateInfo ={};
	pipelineCreateInfo.sType      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.layout     = m_pipelineLayout;
	pipelineCreateInfo.renderPass = renderPass;
    //Assign states, two shader stages
	pipelineCreateInfo.stageCount          = 2;
	pipelineCreateInfo.pVertexInputState   = &m_vertices.inputStateInfo;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	pipelineCreateInfo.pRasterizationState = &rasterizationState;
	pipelineCreateInfo.pColorBlendState    = &colorBlendState;
	pipelineCreateInfo.pMultisampleState   = &multiSampleState;
	pipelineCreateInfo.pViewportState      = &viewportState;
	pipelineCreateInfo.pDepthStencilState  = &depthStencilState;
	pipelineCreateInfo.stageCount          = shaderStages.size();
	pipelineCreateInfo.pStages             = &shaderStages[0];
	pipelineCreateInfo.renderPass          = renderPass;
	pipelineCreateInfo.pDynamicState       = &pipelineDynamicState;


	res = vkCreateGraphicsPipelines(m_device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline);
	assert(!res);
}

void Cube::init_descriptorPool()
{
	//We need to tell the api the number of max requessted descriptors per type
	VkDescriptorPoolSize typeCounts[2] = {};

	//This example only uses one descriptor type (uniform buffer) and only requests one descriptor of this type
	typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	typeCounts[0].descriptorCount = 1;

	typeCounts[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	typeCounts[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo descPoolInfo = {};
	descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descPoolInfo.pNext = nullptr;
	descPoolInfo.poolSizeCount = 2;
	descPoolInfo.pPoolSizes = typeCounts;
	// Set the max. number of sets that can be requested
    // Requesting descriptors beyond maxSets will result in an error
	descPoolInfo.maxSets = 2;

	VkResult res = vkCreateDescriptorPool(m_device, &descPoolInfo, nullptr, &m_descriptorPool);
	assert(!res);

}

void Cube::init_descriptorSet()
{
	//Update descirptor sets determing the shader binding points 
	//For every binding point used in a shader there needs to be one
	//Descriptor set matching that binding point
	VkResult res = VK_SUCCESS;

	VkDescriptorSetAllocateInfo descAllocInfo = {};
	descAllocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descAllocInfo.descriptorPool     = m_descriptorPool;
	descAllocInfo.descriptorSetCount = 1;
	descAllocInfo.pSetLayouts        = &m_descriptorSetLayout;
	res                              = vkAllocateDescriptorSets(m_device, &descAllocInfo, &m_descriptorSet);
	assert(!res);

	// Image descriptor for the color map texture
	VkDescriptorImageInfo descriptorImageInfo ={};
	descriptorImageInfo.sampler = m_texture.sampler;
	descriptorImageInfo.imageView = m_texture.view;
	descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;


   //Binding 0: Uniform buffer
	std::vector<VkWriteDescriptorSet> writeDescSets(2);
	writeDescSets[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSets[0].dstSet          = m_descriptorSet;
	writeDescSets[0].descriptorCount = 1;
	writeDescSets[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescSets[0].pBufferInfo     = &m_uniform.desc;
	writeDescSets[0].dstBinding      = 0;
	writeDescSets[0].descriptorCount = 1;

	// Binding 1 : Fragment shader texture samplers
	writeDescSets[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSets[1].dstSet          = m_descriptorSet;
	writeDescSets[1].descriptorCount = 1;
	writeDescSets[1].descriptorType  = 	VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
	writeDescSets[1].pImageInfo      = &descriptorImageInfo;
	writeDescSets[1].dstBinding      = 1;
	writeDescSets[1].descriptorCount = 1;

	vkUpdateDescriptorSets(m_device, writeDescSets.size(), &writeDescSets[0], 0, nullptr);
}


void Cube::update_ubo()
{

	m_matrix.model = glm::mat4(1.0f);
	m_matrix.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_matrix.proj  = glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f);

	//Map uniform buffer and update it
	uint8_t *pData;
	VkResult res = vkMapMemory(m_device, m_uniform.memory, 0, sizeof(m_matrix), 0, (void**)&pData);
	assert(!res);
	memcpy(pData, &m_matrix, sizeof(MVPMatrix));
	vkUnmapMemory(m_device, m_uniform.memory);

}


}