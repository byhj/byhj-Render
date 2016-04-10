#ifndef OGLMesh_H
#define OGLMesh_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace byhj
{

	class OGLMesh {
	public:
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texcoord;
			glm::vec3 tangent;
			glm::vec3 bitangent;
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
		void drawPatch(GLuint program);
		void drawInstance(GLuint program, GLuint amount);
		void setup();

		GLuint getVBO() const;
		GLuint getVAO() const;
		GLuint getIBO() const;

	private:
		void calcTangent();

		GLuint m_vao, m_ibo, m_vbo;

		std::vector<Vertex> m_VertexData;
		std::vector<GLuint> m_IndexData;
		std::vector<Texture>m_Textures;
	};
}

#endif
