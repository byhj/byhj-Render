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
		GLfloat sw = WindowInfo::getInstance()->getWidth();
		GLfloat sh = WindowInfo::getInstance()->getHeight();
		glm::mat4 ortho = glm::ortho(-sw/2, sw/2, -sh/2, sh/2, -1.0f, 1.0f);

		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &ortho[0][0]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void OGLRTT::shutdown()
	{

	}

	void OGLRTT::init_buffer(glm::vec2 pos, glm::vec2 size)
	{
		GLfloat width = size.x;
		GLfloat height = size.y;
		GLfloat posX = pos.x;
		GLfloat posY = pos.y;
		GLfloat sw = WindowInfo::getInstance()->getWidth();
		GLfloat sh = WindowInfo::getInstance()->getHeight();
		// Calculate the screen coordinates of the left side of the bitmap.
		float left = (float)((sw / 2) * -1) + (float)posX;

		// Calculate the screen coordinates of the right side of the bitmap.
		float  right = left + (float)width;

		// Calculate the screen coordinates of the top of the bitmap.
		float  top = (float)((sh / 2) * -1) + (float)posY;

		// Calculate the screen coordinates of the bottom of the bitmap.
		float  bottom = top - (float)height;

		// Update VBO for each character
		GLfloat VertexData[] = {
			// First triangle.
			left, top,      0.0f, 0.0f, 1.0f,
			left, bottom,   0.0f, 0.0f, 0.0f,
			right, bottom,  0.0f, 1.0f, 0.0f,

	        right, bottom,  0.0f, 1.0f, 0.0f,
		    right, top,     0.0f, 1.0f, 1.0f,
 		    left, top,      0.0f, 0.0f, 1.0f,
		};
		GLuint IndexData[] ={ 
            0, 1, 2,
			3, 4, 5
		};

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData,  GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexData), IndexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OGLRTT::init_vertexArray()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		  
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
		mvp_loc = glGetUniformLocation(m_program, "u_ortho");
	}
}