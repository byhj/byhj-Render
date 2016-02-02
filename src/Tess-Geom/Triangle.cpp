#include  "Triangle.h"
#include <GLFW/glfw3.h>

namespace byhj
{
 ///////////////////Vertex Data////////////////////////////////
 
 static const GLfloat VertexData[] =
 {
 	//Position              Color
 	-0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,
 	 0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,
 	 0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f
 };
 static const GLsizei VertexSize = sizeof(VertexData);
 
 static const GLuint IndexData[] =
 {
 	0, 1, 2,
 };
 static const GLsizei IndexSize = sizeof(IndexData);
 
	void Triangle::init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
	}

	void Triangle::init_buffer()
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
	void Triangle::init_vertexArray()
	{
	    glGenVertexArrays(1, &m_vao);
	    glBindVertexArray(m_vao);
	    
	    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	    glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3) );
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	    
	    glBindVertexArray(0);
	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Triangle::update()
	{
		glUseProgram(m_program);

		GLfloat time = static_cast<GLfloat>(glfwGetTime());
		GLfloat offset[] ={
			(float)sin(time) * 0.5f,
			(float)cos(time) * 0.5f,
			0.0f, 0.0f
		};
		glUniform4fv(m_offset_loc, 1, offset);

		glUseProgram(0);
	}

	void Triangle::render()
	{
		glBindVertexArray(m_vao);
		glUseProgram(m_program);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_PATCHES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void  Triangle::shutdown()
	{

	}
	void Triangle::init_shader()
	{
	  m_TriangleShader.init();
	  m_TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	  m_TriangleShader.attach(GL_TESS_CONTROL_SHADER, "triangle.tcs");
	  m_TriangleShader.attach(GL_TESS_EVALUATION_SHADER, "triangle.tes");
	  m_TriangleShader.attach(GL_GEOMETRY_SHADER, "triangle.geom");
	  m_TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	  m_TriangleShader.link();
	  m_TriangleShader.info();
	  m_program = m_TriangleShader.getProgram();
	  m_offset_loc = glGetUniformLocation(m_program, "offset");
	}

}