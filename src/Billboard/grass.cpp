

#include "Grass.h"
#include "ogl/loadTexture.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "textureMgr.h"
namespace byhj
{
#pragma region VertexData

	const static GLfloat VertexData[] = 
	{
	  -1.5f,  -0.5f, -0.48f,
	   1.5f,  -0.5f,  0.51f,
	   0.0f,  -0.5f,  0.7f ,
	  -0.3f,  -0.5f, -2.3f,
	   0.5f,  -0.5f, -0.6f
	};

	void Grass::Init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();
	}

	void Grass::Render(glm::vec3 camPos, const OGLMVPMatrix &matrix)
	{
		glUseProgram(program);
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

	    glUniform3f(glGetUniformLocation(program, "camPos" ), camPos.x, camPos.y, camPos.z);
		glm::mat4 model = matrix.model; 
		glm::mat4 view  = matrix.view;
		glm::mat4 proj  = matrix.proj;
		glUniformMatrix4fv(model_loc, 1, GL_FALSE,  &model[0][0]);
		glUniformMatrix4fv(view_loc, 1, GL_FALSE,  &view[0][0]);
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE,  &proj[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawArrays(GL_POINTS, 0, 4);
		

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void Grass::Shutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	void Grass::init_shader()
	{
		GrassShader.init();
		GrassShader.attach(GL_VERTEX_SHADER, "grass.vert");
		GrassShader.attach(GL_GEOMETRY_SHADER, "grass.geom");
		GrassShader.attach(GL_FRAGMENT_SHADER, "grass.frag");
		GrassShader.link();
		GrassShader.info();
		program = GrassShader.getProgram();

		model_loc = glGetUniformLocation(program, "model");
		view_loc  = glGetUniformLocation(program, "view");
		proj_loc  = glGetUniformLocation(program, "proj");
		tex_loc   = glGetUniformLocation(program, "tex");
	}

	void Grass::init_buffer()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);    //load the vertex data
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		vecPos.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
		vecPos.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
		vecPos.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
		vecPos.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
		vecPos.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));
	}

	void Grass::init_vertexArray()
	{	
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindVertexArray(0);
	}

	void Grass::init_texture()
	{
		texture = TextureMgr::getInstance()->loadOGLDDS("grass01.dds");
	}



}//Namespace