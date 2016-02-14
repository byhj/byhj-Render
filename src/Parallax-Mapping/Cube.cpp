#include  "cube.h"
#include <glfw/glfw3.h>
#include "Utility.h"
#include "textureMgr.h"
#include "windowInfo.h"

namespace byhj
{
 ///////////////////Vertex Data////////////////////////////////
 
	
	const static GLfloat VertexData[] ={
		// Positions          // Normals           // Texture Coord
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
 static const GLsizei VertexSize = sizeof(VertexData);
 

	void Cube::init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();

	}


	void Cube::update()
	{

	}

	void Cube::render()
	{
		glBindVertexArray(m_vao);
		glUseProgram(m_program);

		auto time = glfwGetTime();
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)time, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj  = glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f);

		glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(u_proj, 1, GL_FALSE, &proj[0][0]);

		colorTex = TextureMgr::getInstance()->getOGLTextureByName("toy_box_diffuse.png");
		normalTex = TextureMgr::getInstance()->getOGLTextureByName("toy_box_normal.png");
		disTex = TextureMgr::getInstance()->getOGLTextureByName("toy_box_disp.png");

		glUniform1i(glGetUniformLocation(m_program, "colorTex"), 0);
		glUniform1i(glGetUniformLocation(m_program, "normalTex"), 1);
		glUniform1i(glGetUniformLocation(m_program, "disTex"), 2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, disTex);

		glDrawArrays(GL_TRIANGLES, 0, 36);


		glBindVertexArray(0);
		glUseProgram(0);

	}

	void  Cube::shutdown()
	{
		glDeleteProgram(m_program);
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);
	}
	void Cube::init_shader()
	{
	  m_CubeShader.init();
	  m_CubeShader.attach(GL_VERTEX_SHADER,   "cube.vert");
	  m_CubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	  m_CubeShader.link();
	  m_CubeShader.info();
	  m_program = m_CubeShader.getProgram();

	  u_model = glGetUniformLocation(m_program, "g_model");
	  u_view  = glGetUniformLocation(m_program, "g_view");
	  u_proj  = glGetUniformLocation(m_program, "g_proj");
	}
	void Cube::init_buffer()
	{
		m_VertexData.resize(36);
		// Positions          // Normals           // Texture Coord
		    m_VertexData[0] = Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			m_VertexData[1] = Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			m_VertexData[2] = Vertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f );
			m_VertexData[3] = Vertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f );
			m_VertexData[4] = Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			m_VertexData[5] = Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);

			m_VertexData[6] = Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f );
			m_VertexData[7] = Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f );
			m_VertexData[8] = Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f  );
			m_VertexData[9] = Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f  );
			m_VertexData[10] = Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f );
			m_VertexData[11] = Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f );
		
			m_VertexData[12] = Vertex(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f );
			m_VertexData[13] = Vertex(-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			m_VertexData[14] = Vertex(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			m_VertexData[15] = Vertex(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			m_VertexData[16] = Vertex(-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			m_VertexData[17] = Vertex(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			
			m_VertexData[18] = Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f  );
			m_VertexData[19] = Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			m_VertexData[20] = Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f );
			m_VertexData[21] = Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f );
			m_VertexData[22] = Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f  );
			m_VertexData[23] = Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f  );

			m_VertexData[24] = Vertex(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
			m_VertexData[25] = Vertex(0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
			m_VertexData[26] = Vertex(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f );
			m_VertexData[27] = Vertex(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f );
			m_VertexData[28] = Vertex(-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
			m_VertexData[29] = Vertex(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

			m_VertexData[30] = Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f );
			m_VertexData[31] = Vertex(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f  );
			m_VertexData[32] = Vertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f   );
			m_VertexData[33] = Vertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f   );
			m_VertexData[34] = Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f );
			m_VertexData[35] = Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  );
			calcTangent();

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_VertexData.size(), &m_VertexData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Cube::init_vertexArray()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), GL_BUFFER_OFFSET(sizeof(GLfloat) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), GL_BUFFER_OFFSET(sizeof(GLfloat) * 6));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), GL_BUFFER_OFFSET(sizeof(GLfloat) * 8));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  GL_BUFFER_OFFSET(sizeof(GLfloat) * 11));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Cube::init_texture()
	{
		TextureMgr::getInstance()->loadOGLTexture("toy_box_diffuse.png");
		TextureMgr::getInstance()->loadOGLTexture("toy_box_normal.png");
		TextureMgr::getInstance()->loadOGLTexture("toy_box_disp.png");
	}
	void Cube::calcTangent()
	{
		for (int i=0; i < m_VertexData.size(); i+=3) {

			// Shortcuts for vertices
			glm::vec3 & v0 = m_VertexData[i + 0].pos;
			glm::vec3 & v1 = m_VertexData[i + 1].pos;
			glm::vec3 & v2 = m_VertexData[i + 2].pos;

			// Shortcuts for UVs
			glm::vec2 & uv0 = m_VertexData[i + 0].tc;
			glm::vec2 & uv1 = m_VertexData[i + 1].tc;
			glm::vec2 & uv2 = m_VertexData[i + 2].tc;

			// Edges of the triangle : postion delta
			glm::vec3 deltaPos1 = v1 - v0;
			glm::vec3 deltaPos2 = v2 - v0;

			// UV delta
			glm::vec2 deltaUV1 = uv1 - uv0;
			glm::vec2 deltaUV2 = uv2 - uv0;

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)* r;
			glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

			m_VertexData[i + 0].tangent = tangent;
			m_VertexData[i + 1].tangent = tangent;
			m_VertexData[i + 2].tangent = tangent;

			m_VertexData[i + 0].bitangent = bitangent;
			m_VertexData[i + 1].bitangent = bitangent;
			m_VertexData[i + 2].bitangent = bitangent;
		} 

	}
}