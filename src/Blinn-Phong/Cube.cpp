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
		m_LightGui.v_init(WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());

		init_shader();
		init_buffer();
		init_vertexArray();
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

	void Cube::update()
	{
		m_LightGui.v_update();
	}

	void Cube::render()
	{

		m_LightGui.v_render();

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
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);

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
	  m_CubeShader.attach(GL_VERTEX_SHADER, "Cube.vert");
	  m_CubeShader.attach(GL_FRAGMENT_SHADER, "Cube.frag");
	  m_CubeShader.link();
	  m_CubeShader.info();
	  m_program = m_CubeShader.getProgram();

	  u_model = glGetUniformLocation(m_program, "g_model");
	  u_view  = glGetUniformLocation(m_program, "g_view");
	  u_proj  = glGetUniformLocation(m_program, "g_proj");
	}

}