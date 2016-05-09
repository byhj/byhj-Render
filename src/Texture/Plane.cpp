#include "Plane.h"
#include "textureMgr.h"
#include "ogl/oglUtility.h"

namespace byhj {

	static const GLfloat VertexData[]  ={
		-0.5f,  0.5f, 0.0f,   0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,   1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f
	};
	static const GLsizei VertexSize = sizeof(VertexData);

	static  const GLuint IndexData[] ={
		0, 1, 2, 2, 3, 0
	};
	static const GLsizei IndexSize = sizeof(IndexData);

	Plane::Plane()
	{

	}

	Plane::~Plane()
	{

	}

	void Plane::init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();
	}

	void Plane::update()
	{

	}

	void Plane::render()
	{
		glUseProgram(m_program);
		glBindVertexArray(m_vao);

		GLuint texId = TextureMgr::getInstance()->getOGLTextureByName("byhj.jpg");
		glBindTexture(GL_TEXTURE_2D, texId);
		glActiveTexture(GL_TEXTURE0);

		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
		glBindVertexArray(0);
	}

	void Plane::shutdown()
	{
		glDeleteProgram(m_program);
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);
	}

	void Plane::init_buffer()
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

	void Plane::init_vertexArray()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, GL_BUFFER_OFFSET(0) );
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, GL_BUFFER_OFFSET(sizeof(GLfloat) * 3) );

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glBindVertexArray(m_vao);
	}

	void Plane::init_shader()
	{
		PlaneShader.init();
		PlaneShader.attach(GL_VERTEX_SHADER, "plane.vert");
		PlaneShader.attach(GL_FRAGMENT_SHADER, "plane.frag");
		PlaneShader.link();
		PlaneShader.info();
		m_program = PlaneShader.getProgram();
		m_tex = glGetUniformLocation(m_program, "tex");
		//CHECK_OGL_UNIFORM( m_tex, "Uniform can not gen" );
	}

	void Plane::init_texture()
	{
		TextureMgr::getInstance()->loadOGLTexture("byhj.jpg");
	}
}
