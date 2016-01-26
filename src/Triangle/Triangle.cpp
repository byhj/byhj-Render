#include  "Triangle.h"

namespace byhj
{
	///////////////////Vertex Data////////////////////////////////

	static const GLfloat VertexData[] =
	{
		//Front
		-0.5f, -0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		0.0f,  0.5f,  0.0f,
	};
	static const GLsizei VertexSize = sizeof(VertexData);

	static const GLuint IndexData[] =
	{
		0, 1, 2,
	};
	static const GLsizei IndexSize = sizeof(IndexData);

	GLfloat* Triangle::getVertexData() 
	{
		return &m_VertexData[0];
	}

	GLuint * Triangle::getIndexData()
	{

		return &m_IndexData[0];
	}

	GLsizei Triangle::getVertexSize()
	{
		auto var = m_VertexData.size() * sizeof(GLfloat);
		return var;
	}
	GLsizei Triangle::getIndexSize()
	{
		return m_IndexData.size() * sizeof(GLuint);
	}


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


		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
	}
	void Triangle::init_vertexArray()
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

	void Triangle::update()
	{

	}

	void Triangle::render()
	{


		glBindVertexArray(m_vao);
		glUseProgram(m_program);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void  Triangle::shutdown()
	{

	}
	void Triangle::init_shader()
	{
		m_TriangleShader.init();
		m_TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
		m_TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
		m_TriangleShader.link();
		m_program = m_TriangleShader.getProgram();
	}

}