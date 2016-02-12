#include  "cube.h"
#include <glfw/glfw3.h>
#include "windowInfo.h"

namespace byhj
{
 ///////////////////Vertex Data////////////////////////////////
 
 static const GLfloat VertexData[] =
 {
	 //Front
	 -0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f, 
	 0.5f,  0.5f,  0.5f, 
	 -0.5f,  0.5f,  0.5f,

	 //Back		   
	 -0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f, 
	 0.5f,  0.5f, -0.5f, 
	 -0.5f,  0.5f, -0.5f,
 };
 static const GLsizei VertexSize = sizeof(VertexData);
 
 static const GLuint IndexData[] =
 {
	 //Front
	 0, 1, 2,
	 2, 3, 0,

	 //Left
	 0, 3, 7,
	 7, 4, 0,

	 //back
	 4, 7, 6,
	 6, 5, 4,

	 //right
	 5, 6, 2,
	 2, 1, 5,

	 //top
	 3, 2, 6,
	 6, 7, 3,

	 //bottom
	 5, 1, 0,
	 0, 4, 5,
 };
 static const GLsizei IndexSize = sizeof(IndexData);
 static const GLuint IndexCount = IndexSize / sizeof(GLuint);

	void Cube::init()
	{
		init_viewport();
		init_shader();
		init_buffer();
		init_vertexArray();
	
	}



	void Cube::update()
	{
		glBindVertexArray(m_vao);

		auto aspect = WindowInfo::getInstance()->getAspect();
		glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
		
		float f = (float)glfwGetTime() * 0.01f;
		
		//glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);

		glm::mat4 * mv_array = (glm::mat4 *)glMapBufferRange(GL_UNIFORM_BUFFER,
			0,
			4 * sizeof(glm::mat4),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int i = 0; i < 4; i++)
		{
			mv_array[i] = proj_matrix
				* glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.5f))
				* glm::rotate(glm::mat4(1.0f), (float)f * 45.0f * (float)(i + 1), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), (float)f * 81.0f * (float)(i + 1), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		glBindVertexArray(0);
	}

	void Cube::render()
	{
		// Notice the gl_scissor_test , scissor test is that when you clear the
		// framebuffer using glClear() or glClearBufferfv(), the first scissor rectangle is applied as well.
		glEnable(GL_SCISSOR_TEST);
		glBindVertexArray(m_vao);
		glUseProgram(m_program);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
		glDisable(GL_SCISSOR_TEST);
	}

	void  Cube::shutdown()
	{
		glDeleteProgram(m_program);
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);
		glDeleteBuffers(1, &m_ubo);
	}
	void Cube::init_shader()
	{
	  m_CubeShader.init();
	  m_CubeShader.attach(GL_VERTEX_SHADER, "view.vert");
	  m_CubeShader.attach(GL_GEOMETRY_SHADER, "view.geom");
	  m_CubeShader.attach(GL_FRAGMENT_SHADER, "view.frag");
	  m_CubeShader.link();
	  m_CubeShader.info();
	  m_program = m_CubeShader.getProgram();

	}
	void Cube::init_buffer()
	{
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}
	void Cube::init_vertexArray()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Cube::init_viewport()
	{
		glUseProgram(m_program);
		glBindVertexArray(m_vao);

		int i;
		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();



		static const int width = sw;
		static const int height = sh;
		int scissor_width = (7 * width) / 16; // 7/16window
		int scissor_height = (7 * height) / 16;
		// Lower left...
		glScissorIndexed(0,     //index
			0, 0, //left bottom
			scissor_width, scissor_height); // width height
											// Lower right...
		glScissorIndexed(1,
			width - scissor_width, 0,
			width - scissor_width, scissor_height);
		//Upper left...
		glScissorIndexed(2,
			0, height - scissor_height,
			scissor_width, scissor_height);
		// Upper right...
		glScissorIndexed(3,
			width - scissor_width, height - scissor_height,
			scissor_width, scissor_height);
	}
}