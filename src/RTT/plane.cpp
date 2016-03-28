#include "Plane.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "textureMgr.h"
#include "utility.h"

namespace byhj
{

void Plane::init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
}

void Plane::render(const glm::mat4 &mvp)
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);

	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(tex_loc, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Plane::shutdown()
{
	glDeleteProgram(m_program);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void Plane::init_shader()
{
	PlaneShader.init();
	PlaneShader.attach(GL_VERTEX_SHADER, "plane.vert");
	PlaneShader.attach(GL_FRAGMENT_SHADER, "plane.frag");
	PlaneShader.link();
	PlaneShader.info();
	m_program = PlaneShader.getProgram();

	//assert(tex_loc != 0xffffffff);
	//	CHECK_OGL_UNIFORM(tex_loc != 0xffffffff, "Texture Location" );
	mvp_loc = glGetUniformLocation(m_program, "u_mvp");
	tex_loc = glGetUniformLocation(m_program, "u_tex");
}


void Plane::init_buffer()
{
	GLfloat VertexData[] ={
		// Positions        // Texture Coords
		-1.0f, 0.0f, -1.0f,    0.0f, 0.0f,
		-1.0f, 0.0f,  1.0f,    0.0f, 1.0f,
		 1.0f, 0.0f,  1.0f,    1.0f, 1.0f,
		 1.0f, 0.0f, -1.0f,    1.0f, 0.0f,
	};


	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);    //load the vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Plane::init_vertexArray()
{	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

void Plane::init_texture()
{
	m_texture = TextureMgr::getInstance()->loadOGLTexture("grass.jpg");
}


}//Namespace