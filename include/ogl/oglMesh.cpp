#include "oglMesh.h"
#include <sstream>

namespace byhj
{

	void OGLMesh::draw(GLuint program)
	{
		// Bind appropriate textures
		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		GLuint normalNr = 1;

		for (GLuint i = 0; i < m_Textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			// Retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = m_Textures[i].type;

			if (name == "u_diffuseTex")
				ss << diffuseNr++;
			else if (name == "u_specularTex")
				ss << specularNr++;
			else if (name == "u_normalTex")
				ss << normalNr++;


			glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
		}

		// Also set each OGLMesh's shininess property to a default value (if you want you could extend this to another OGLMesh property and possibly change this value)
		glUniform1f(glGetUniformLocation(program, "material.shininess"), 32.0f);


		// Draw OGLMesh
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_IndexData.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Always good practice to set everything back to defaults once configured.
		for (GLuint i = 0; i < this->m_Textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	void OGLMesh::drawPatch(GLuint program)
	{
		// Bind appropriate textures
		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		GLuint normalNr = 1;

		for (GLuint i = 0; i < m_Textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			// Retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = m_Textures[i].type;

			if (name == "u_diffuseTex")
				ss << diffuseNr++;
			else if (name == "u_specularTex")
				ss << specularNr++;
			else if (name == "u_normalTex")
				ss << normalNr++;


			glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
		}

		// Also set each OGLMesh's shininess property to a default value (if you want you could extend this to another OGLMesh property and possibly change this value)
		glUniform1f(glGetUniformLocation(program, "material.shininess"), 32.0f);


		// Draw OGLMesh
		glBindVertexArray(m_vao);
		glDrawElements(GL_PATCHES, m_IndexData.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Always good practice to set everything back to defaults once configured.
		for (GLuint i = 0; i < this->m_Textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	void OGLMesh::drawInstance(GLuint program, GLuint amount)
	{
		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		GLuint normalNr = 1;

		for (GLuint i = 0; i < m_Textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			// Retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = m_Textures[i].type;

			if (name == "u_diffuseTex")
				ss << diffuseNr++;
			else if (name == "u_specularTex")
				ss << specularNr++;
			else if (name == "u_normalTex") 
				ss << normalNr++;

			number = ss.str();


			glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
		}

		// Also set each OGLMesh's shininess property to a default value (if you want you could extend this to another OGLMesh property and possibly change this value)
		glUniform1f(glGetUniformLocation(program, "material.shininess"), 32.0f);


		// Draw OGLMesh
		glBindVertexArray(m_vao);

		glDrawElementsInstanced(GL_TRIANGLES, m_IndexData.size(), GL_UNSIGNED_INT, 0, amount);

		glBindVertexArray(0);

		// Always good practice to set everything back to defaults once configured.
		for (GLuint i = 0; i < this->m_Textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void OGLMesh::calcTangent()
	{
		for (int i = 0; i != m_VertexData.size(); i += 3) {
		  
		  //Get a Trangle vertex
		  glm::vec3 v0 = m_VertexData[i+0].position;
		  glm::vec3 v1 = m_VertexData[i+1].position;
		  glm::vec3 v2 = m_VertexData[i+2].position;

		  glm::vec2 uv0 = m_VertexData[i+0].texcoord;
		  glm::vec2 uv1 = m_VertexData[i+1].texcoord;
		  glm::vec2 uv2 = m_VertexData[i+2].texcoord;

		  //Eeges of the triangle
		  glm::vec3 deltaPos1 = v1 - v0;
		  glm::vec3 deltaPos2 = v2 - v0;

		  glm::vec2 deltaUV1 = uv1 - uv0;
		  glm::vec2 deltaUV2 = uv2 - uv0;

		 // assert((deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x) != 0);

		  float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		  glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		  glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		  for (int j = 0; j < 3; ++j) {
		     m_VertexData[i + j].tangent = tangent;
			 m_VertexData[i + j].bitangent = bitangent;
		  }

		}

		/*
		for (int i = 0; i != m_VertexData.size(); ++i) {
			glm::vec3 & n = m_VertexData[i].normal;
			glm::vec3 & t = m_VertexData[i].tangent;
			glm::vec3 & b = m_VertexData[i].bitangent;

			// Gram-Schmidt orthogonalize
			t = glm::normalize(t - n * glm::dot(n, t));

			// Calculate handedness
			if (glm::dot(glm::cross(n, t), b) < 0.0f) {
				t = t * -1.0f;
			}
		}

		*/

	}
	void OGLMesh::setup()
	{
		calcTangent();

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_VertexData.size() * sizeof(Vertex), &m_VertexData[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexData.size() * sizeof(GLuint), &m_IndexData[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

		// Vertex TexCoords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));

		// Tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));

		//BiTangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bitangent));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	GLuint OGLMesh::getVBO() const
	{
		return m_vbo;
	}
	GLuint OGLMesh::getVAO() const
	{
		return m_vao;
	}
	GLuint OGLMesh::getIBO() const
	{
		return m_ibo;
	}




}
