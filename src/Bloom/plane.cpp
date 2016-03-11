#include "Plane.h"
#include "ogl/loadTexture.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace byhj
{
#pragma region VertexData

GLfloat VertexData[] = {
		// Positions        // Texture Coords
		-1.0f,  1.0f, 0.0f,    0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
		1.0f,  1.0f, 0.0f,    1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,    1.0f, 0.0f,
};

#pragma endregion

void Plane::Init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
}

void Plane::Render( const GLuint &colorBuffer)
{
	glUseProgram(program);
	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glUniform1i(glGetUniformLocation(program, "hdr"), 1);
	glUniform1f(glGetUniformLocation(program, "exposure"), exposure);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Plane::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Plane::init_shader()
{
	PlaneShader.init();
	PlaneShader.attach(GL_VERTEX_SHADER,   "hdr.vert");
	PlaneShader.attach(GL_FRAGMENT_SHADER, "hdr.frag");
	PlaneShader.link();
	PlaneShader.info();
	program = PlaneShader.getProgram();
}

void Plane::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);    //load the vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Plane::init_vertexArray()
{	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

void Plane::init_texture()
{

}


}//Namespace