 #include "oglRender.h"

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

	OGLRender::OGLRender()
	{

	}

	OGLRender::~OGLRender()
	{
	}
    
	void OGLRender::v_init()
	{
	   init_buffer();
	   init_shader();
	}

	void OGLRender::v_update()
	{
	}

    void OGLRender::v_render()
    {

		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, &black[0]);

		glBindVertexArray(m_vao);
		glUseProgram(m_program);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
    }
	void OGLRender::v_shutdown()
	{
	}

	void OGLRender::init_buffer()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER,  VertexSize, VertexData, GL_STATIC_DRAW);


		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
	

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
    
	void OGLRender::init_shader()
	{
		m_TriangleShader.init();
		m_TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
		m_TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
		m_TriangleShader.link();
		m_program = m_TriangleShader.getProgram();
	}

	void OGLRender::init_texture()
	{

	}
}
