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
		

		glBindVertexArray(0);
	}

	void Cube::render()
	{
		glBindVertexArray(m_vao);
		glUseProgram(m_program);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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


	}
	void Cube::init_vertexArray()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


}