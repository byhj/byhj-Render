#include "oglRender.h"

namespace byhj
{
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
		glBufferData(GL_ARRAY_BUFFER, , GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
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
