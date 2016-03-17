

#include "Grass.h"
#include "ogl/loadTexture.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "textureMgr.h"
#include <ctime>

namespace byhj
{
#pragma region VertexData

	void Grass::Init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();
	}

	void Grass::Render(glm::vec3 camPos, const OGLMVPMatrix &matrix)
	{

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		glDrawArrays(GL_POINTS, 0, 1000);
		

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
		// Seed the random generator.
		srand((int)std::time(NULL));

		// Set random positions and random colors for each piece of foliage.
		for (int i=0; i < 1000; i++)
		{
			Vertex vertex;
			vertex.position.x = ((float)rand() / (float)(RAND_MAX)) *9.0f - 4.5f;
			vertex.position.y = -0.5f;								 
			vertex.position.z = ((float)rand() / (float)(RAND_MAX)) *9.0f - 4.5f;

			float red = ((float)rand() / (float)(RAND_MAX)) * 1.0f;
			float green = ((float)rand() / (float)(RAND_MAX)) * 1.0f;
			vertex.color.r = red + 1.0f;
			vertex.color.g = green + 0.5f;
			vertex.color.b = 0.0f;

			m_VertexData.push_back(vertex);

		}

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);    //load the vertex data
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) *m_VertexData.size(), &m_VertexData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void Grass::init_vertexArray()
	{	
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3)));
		glBindVertexArray(0);
	}

	void Grass::init_texture()
	{
		texture = TextureMgr::getInstance()->loadOGLDDS("grass01.dds");
	}



}//Namespace