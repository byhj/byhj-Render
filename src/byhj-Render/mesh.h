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

	class OGLMesh {
	public:
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

		public:
			OGLMesh() = default;
			OGLMesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures)
			{
				m_VertexData = vertices;
				m_IndexData  = indices;
				m_Textures   = textures;

				setup();
			}
			OGLMesh(const OGLMesh &mesh) = default;
			~OGLMesh() = default;
			void draw(GLuint program);
			void drawInstance(GLuint program, GLuint amount);

			void setup();

			GLuint getVBO() const;
			GLuint getVAO() const;
			GLuint getIBO() const;

		private:
			GLuint m_vao, m_ibo, m_vbo;

			std::vector<Vertex> m_VertexData;
			std::vector<GLuint> m_IndexData;
			std::vector<Texture>m_Textures;
	};
	/*
	class D3DMesh {
	public:
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

		public:
			D3DMesh() = default;
			D3DMesh(const std::vector<Vertex> &vertices, const std::vector<UINT> &indices, const std::vector<Texture> textures)
			{
				m_VertexData = vertices;
				m_IndexData  = indices;
				m_Textures  = textures;
			}
			D3DMesh(const D3DMesh &mesh);
			~D3DMesh();

		private:
			std::vector<Vertex> m_VertexData;
			std::vector<UINT> m_IndexData;
			std::vector<Texture> m_Textures;
		};
		*/
}

#endif