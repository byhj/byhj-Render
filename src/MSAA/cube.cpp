#include "cube.h"
#include "ogl/loadTexture.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace byhj
{
#pragma region VertexData

	const static GLfloat VertexData[] = {
		-0.5f, -0.5f, -0.5f,  
		0.5f , -0.5f, -0.5f,  
		0.5f , 0.5f, -0.5f,  
		0.5f , 0.5f, -0.5f,  
		-0.5f, 0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f, -0.5f, 0.5f,  
		0.5f , -0.5f, 0.5f,  
		0.5f , 0.5f, 0.5f,  
		0.5f , 0.5f, 0.5f,  
		-0.5f, 0.5f, 0.5f,  
		-0.5f, -0.5f, 0.5f,  

		-0.5f, 0.5f, 0.5f,  
		-0.5f, 0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f, 0.5f,  
		-0.5f, 0.5f, 0.5f,  

		0.5f , 0.5f, 0.5f,  
		0.5f , 0.5f, -0.5f,  
		0.5f , -0.5f, -0.5f,  
		0.5f , -0.5f, -0.5f,  
		0.5f , -0.5f, 0.5f,  
		0.5f , 0.5f, 0.5f,  

		-0.5f, -0.5f, -0.5f,  
		0.5f , -0.5f, -0.5f,  
		0.5f , -0.5f, 0.5f,  
		0.5f , -0.5f, 0.5f,  
		-0.5f, -0.5f, 0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f, 0.5f, -0.5f,  
		0.5f , 0.5f, -0.5f,  
		0.5f , 0.5f, 0.5f,  
		0.5f , 0.5f, 0.5f,  
		-0.5f, 0.5f, 0.5f,  
		-0.5f, 0.5f, -0.5f,  
	};

#pragma endregion

	void Cube::Init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();
	}

	void Cube::Render(const const ogl::MvpMatrix &matrix)
	{
		glUseProgram(program);
		glBindVertexArray(vao);

		glm::mat4 view  = matrix.view;
		glm::mat4 proj  = matrix.proj;
		glm::mat4 model = matrix.model;
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(view_loc, 1, GL_FALSE,  &view[0][0]);
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE,  &proj[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void Cube::Shutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	void Cube::init_shader()
	{
		CubeShader.init();
		CubeShader.attach(GL_VERTEX_SHADER,  "cube.vert");
		CubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
		CubeShader.link();
		CubeShader.info();
		program = CubeShader.GetProgram();
		model_loc = glGetUniformLocation(program, "model");
		view_loc  = glGetUniformLocation(program, "view");
		proj_loc  = glGetUniformLocation(program, "proj");
	}

	void Cube::init_buffer()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);    //load the vertex data
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Cube::init_vertexArray()
	{	
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glBindVertexArray(0);
	}

	void Cube::init_texture()
	{
	}


}//Namespace