#include "oglRTT.h"
#include "WindowInfo.h"

namespace byhj {

	void OGLRTT::init(glm::vec2 pos, glm::vec2 size)
	{
		init_buffer(pos, size);
		init_vertexArray();
		init_shader();
	}

	void OGLRTT::update()
	{
	}

	void OGLRTT::render(GLuint rttTex)
	{

		glUseProgram(m_program);
		glBindVertexArray(m_vao);

		glUniform1i(tex_loc, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rttTex);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void OGLRTT::shutdown()
	{

	}

	void OGLRTT::init_buffer(glm::vec2 pos, glm::vec2 size)
	{
		GLfloat w = size.x;
		GLfloat h = size.y;
		// Update VBO for each character
		GLfloat VertexData[6][5] ={
			{ pos.x,     pos.y + h,  0.0f,  0.0, 0.0 },
			{ pos.x,     pos.y,      0.0f,  0.0, 1.0 },
			{ pos.x + w, pos.y,      0.0f,  1.0, 1.0 },
			{ pos.x,     pos.y + h,  0.0f,  0.0, 0.0 },
			{ pos.x + w, pos.y,      0.0f,  1.0, 1.0 },
			{ pos.x + w, pos.y + h,  0.0f,  1.0, 0.0 }
		};
		for (int i = 0; i != m_IndexCount; ++i) {
		    m_IndexData.push_back(i);
		}

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, VertexData[0],  GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_IndexCount, &m_IndexData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OGLRTT::init_vertexArray()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		//Position : Index 1
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
		
		//Texture Coord :Index 2
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, GL_BUFFER_OFFSET( sizeof(GLfloat) * 3) );
		  
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glBindVertexArray(0);
  	}

	void OGLRTT::init_shader()
	{
		m_rttShader.init();
		m_rttShader.attach(GL_VERTEX_SHADER, "rtt.vert");
		m_rttShader.attach(GL_FRAGMENT_SHADER, "rtt.frag");
		m_rttShader.link();
		m_rttShader.info();
		m_program = m_rttShader.getProgram();
		tex_loc = glGetUniformLocation(m_program, "u_tex");
	}
}