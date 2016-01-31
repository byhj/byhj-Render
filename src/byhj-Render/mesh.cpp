#include "Mesh.h"
#include <sstream>

namespace byhj
{

		void OGLMesh::draw(GLuint program)
		{
			// Bind appropriate textures
			GLuint diffuseNr = 1;
			GLuint specularNr = 1;

			for (GLuint i = 0; i < m_Textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);

				// Retrieve texture number (the N in diffuse_textureN)
				std::stringstream ss;
				std::string number;
				std::string name = m_Textures[i].type;

				if (name == "texture_diffuse")
					ss << diffuseNr++;
				else if (name == "texture_specular")
					ss << specularNr++;
				number = ss.str();


				glUniform1f(glGetUniformLocation(program, (name + number).c_str()), i);
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

		void OGLMesh::drawInstance(GLuint program, GLuint amount)
		{
			// Bind appropriate textures
			GLuint diffuseNr = 1;
			GLuint specularNr = 1;

			for (GLuint i = 0; i < m_Textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);

				// Retrieve texture number (the N in diffuse_textureN)
				std::stringstream ss;
				std::string number;
				std::string name = m_Textures[i].type;

				if (name == "texture_diffuse")
					ss << diffuseNr++;
				else if (name == "texture_specular")
					ss << specularNr++;
				number = ss.str();


				glUniform1f(glGetUniformLocation(program, (name + number).c_str()), i);
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

		void OGLMesh::setup()
		{
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
