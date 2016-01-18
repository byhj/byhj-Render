#ifndef Mesh_H
#define Mesh_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <d3d11.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <windows.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace byhj
{
	enum LoadType{
		OGL,
	    D3D
	};
	namespace ogl {

		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texcoord;
		};

		struct Texture {
			GLuint id;
			std::string type;
			aiString path;
		};

		class Mesh {
		public:
			Mesh();
			Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures)
			{
				m_VertexData = vertices;
				m_IndexData  = indices;
				m_Textures   = textures;
			}
			Mesh(const Mesh &mesh);
			~Mesh();

		private:
			std::vector<Vertex> m_VertexData;
			std::vector<GLuint> m_IndexData;
			std::vector<Texture>m_Textures;
		};
    } //namespace ogl

	namespace d3d {
	
		struct Vertex {
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};

		struct Texture {
			ID3D11ShaderResourceView *id;
			std::string type;
			aiString path;
		};
		class Mesh {
		public:
			Mesh() = default;
			Mesh(const std::vector<Vertex> &vertices, const std::vector<UINT> &indices, const std::vector<Texture> textures)
			{
				m_VertexData = vertices;
				m_IndexData  = indices;
				m_Textures  = textures;
			}
			Mesh(const Mesh &mesh);
			~Mesh();

		private:
			std::vector<Vertex> m_VertexData;
			std::vector<UINT> m_IndexData;
			std::vector<Texture> m_Textures;
		};
	}
}

#endif