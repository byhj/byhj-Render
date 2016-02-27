#pragma once

#include <stdlib.h>
#include <string>
#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <map>
#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#else
#endif

#include "vulkanUtility.h"

#include <vulkan/vulkan.h>

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>     
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Vulkan
{
	namespace Mesh
	{

	    typedef enum VertexLayout {
	    	VERTEX_LAYOUT_POSITION = 0x0,
	    	VERTEX_LAYOUT_NORMAL = 0x1,
	    	VERTEX_LAYOUT_COLOR = 0x2,
	    	VERTEX_LAYOUT_UV = 0x3,
	    	VERTEX_LAYOUT_TANGENT = 0x4,
	    	VERTEX_LAYOUT_BITANGENT = 0x5
	    } VertexLayout;
	    
	    struct MeshBufferInfo
	    {
	    	VkBuffer buf = VK_NULL_HANDLE;
	    	VkDeviceMemory mem = VK_NULL_HANDLE;
	    };
	    
	    struct MeshBuffer
	    {
	    	MeshBufferInfo vertices;
	    	MeshBufferInfo indices;
	    	uint32_t indexCount;
	    };
	    
	    // Get vertex size from vertex layout
	    static uint32_t vertexSize(std::vector<Vulkan::Mesh::VertexLayout> layout)
	    {
	    	uint32_t vSize = 0;
	    	for (auto& layoutDetail : layout)
	    	{
	    		switch (layoutDetail)
	    		{
	    			// UV only has two components
	    		case VERTEX_LAYOUT_UV:
	    			vSize += 2 * sizeof(float);
	    			break;
	    		default:
	    			vSize += 3 * sizeof(float);
	    		}
	    	}
	    	return vSize;
	    }
	    
	    static void freeMeshBufferResources(VkDevice device, Vulkan::Mesh::MeshBuffer *meshBuffer)
	    {
	    	vkDestroyBuffer(device, meshBuffer->vertices.buf, nullptr);
	    	vkFreeMemory(device, meshBuffer->vertices.mem, nullptr);
	    	if (meshBuffer->indices.buf != VK_NULL_HANDLE)
	    	{
	    		vkDestroyBuffer(device, meshBuffer->indices.buf, nullptr);
	    		vkFreeMemory(device, meshBuffer->indices.mem, nullptr);
	    	}
	    }
   }
}//Namesoace


// Simple mesh class for getting all the necessary stuff from models loaded via ASSIMP
class VulkanMeshLoader {
private:

	struct Vertex
	{
		glm::vec3 m_pos;
		glm::vec2 m_tex;
		glm::vec3 m_normal;
		glm::vec3 m_color;
		glm::vec3 m_tangent;
		glm::vec3 m_binormal;

		Vertex() {}

		Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& bitangent, const glm::vec3& color)
		{
			m_pos = pos;
			m_tex = tex;
			m_normal = normal;
			m_color = color;
			m_tangent = tangent;
			m_binormal = bitangent;
		}
	};

	struct MeshEntry {
		uint32_t NumIndices;
		uint32_t MaterialIndex;
		uint32_t vertexBase;
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
	};


	VkBool32 getMemoryType(VkPhysicalDeviceMemoryProperties deviceMemoryProperties, uint32_t typeBits, VkFlags properties, uint32_t * typeIndex)
	{
		for (int i = 0; i < 32; i++)
		{
			if ((typeBits & 1) == 1)
			{
				if ((deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					*typeIndex = i;
					return true;
				}
			}
			typeBits >>= 1;
		}
		return false;
	}

public:
	std::vector<MeshEntry> m_Entries;

	struct Dimension
	{
		glm::vec3 min = glm::vec3(FLT_MAX);
		glm::vec3 max = glm::vec3(-FLT_MAX);
		glm::vec3 size;
	} dim;

	uint32_t numVertices = 0;

	// Optional
	struct
	{
		VkBuffer buf;
		VkDeviceMemory mem;
	} vertexBuffer;

	struct {
		VkBuffer buf;
		VkDeviceMemory mem;
		uint32_t count;
	} indexBuffer;

	VkPipelineVertexInputStateCreateInfo vi;
	std::vector<VkVertexInputBindingDescription> bindingDescriptions;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	VkPipeline pipeline;

	Assimp::Importer Importer;
	const aiScene* pScene;

	~VulkanMeshLoader()
	{
		m_Entries.clear();
	}

	// Loads the mesh with some default flags
	bool LoadMesh(const std::string& Filename);

	// Load the mesh with custom flags
	bool LoadMesh(const std::string& Filename, int flags);

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);

	void InitMesh(unsigned int index, const aiMesh* paiMesh, const aiScene* pScene);

	// Clean up vulkan resources used by a mesh
	static void freeVulkanResources(VkDevice device, VulkanMeshLoader *mesh);


	// Create vertex and index buffer with given layout
	void createVulkanBuffers(
		VkDevice device,
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties,
		Vulkan::Mesh::MeshBuffer *meshBuffer,
		std::vector<Vulkan::Mesh::VertexLayout> layout,
		float scale);
};