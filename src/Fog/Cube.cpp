#include  "cube.h"
#include <glfw/glfw3.h>
#include "Utility.h"
#include "textureMgr.h"
#include "windowInfo.h"

namespace byhj
{
 ///////////////////Vertex Data////////////////////////////////
 
	const static GLfloat VertexData[] ={
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
 static const GLsizei VertexSize = sizeof(VertexData);
 
 static const GLuint IndexData[] =
 {
	 //Front
	 0, 1, 2,
	 2, 3, 0,

	 //Left
	 0, 3, 7,
	 7, 4, 0,

	 //back
	 4, 7, 6,
	 6, 5, 4,

	 //right
	 5, 6, 2,
	 2, 1, 5,

	 //top
	 3, 2, 6,
	 6, 7, 3,

	 //bottom
	 5, 1, 0,
	 0, 4, 5,
 };
 static const GLsizei IndexSize = sizeof(IndexData);
 static const GLuint IndexCount = IndexSize / sizeof(GLuint);

	void Cube::init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();

		m_fogGui.v_init();
	}

	void Cube::init_buffer()
	{
		//send the vertex data to opengl vertex buffer 
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Cube::init_vertexArray()
	{
	    glGenVertexArrays(1, &m_vao);
	    glBindVertexArray(m_vao);
	    
		//Shader interface 0:vertex, 1:normal, 2:texcoord
	    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	    glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, GL_BUFFER_OFFSET(sizeof(GLfloat) * 3) );
	    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, GL_BUFFER_OFFSET(sizeof(GLfloat) * 6) );
	    
	    glBindVertexArray(0);
	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Cube::init_texture()
	{
		//Load the cube texture
		TextureMgr::getInstance()->loadOGLTexture("crate.bmp");

	}
	void Cube::update()
	{
		m_fogGui.v_update();
	}

	void Cube::render()
	{
		glBindVertexArray(m_vao);
		glUseProgram(m_program);

		auto aspect =  WindowInfo::getInstance()->getAspect();
		//Update model view proj matrix
		auto time = glfwGetTime();
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)time, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
			                          glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj  = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniformLoc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniformLoc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniformLoc.proj, 1, GL_FALSE, &proj[0][0]);
		glUniform1i(uniformLoc.boxTex, 0);

		//////////////////////////////Set up the gui vars to uniform //////////////////////////////
		glUniform1f(uniformLoc.fogStart, m_fogGui.getFogStart());
		glUniform1f(uniformLoc.fogEnd, m_fogGui.getFogEnd());
		glUniform1f(uniformLoc.fogDensity, m_fogGui.getFogDensity());
		glUniform4fv(uniformLoc.fogColor, 1, m_fogGui.getFogColor());
		//std::cout << m_fogGui.getFogModel() << std::endl;
		glUniform1i(uniformLoc.fogModel, m_fogGui.getFogModel());
		glUniform1i(uniformLoc.baseModel, m_fogGui.getBaseModel());

		//Bind the texture for shader
		GLuint texId = TextureMgr::getInstance()->getOGLTextureByName("crate.bmp");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);

		//Draw the cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glUseProgram(0);

		//draw the gui
		m_fogGui.v_render();
	}

	void  Cube::shutdown()
	{
		//release the handle
		glDeleteProgram(m_program);
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);
	}

	void Cube::init_shader()
	{
	  m_CubeShader.init();
	  m_CubeShader.attach(GL_VERTEX_SHADER, "fog.vert");
	  m_CubeShader.attach(GL_FRAGMENT_SHADER, "fog.frag");
	  m_CubeShader.link();
	  m_CubeShader.info();
	  m_program = m_CubeShader.getProgram();

      uniformLoc.model = glGetUniformLocation(m_program, "u_model");
	  uniformLoc.view  = glGetUniformLocation(m_program, "u_view");
	  uniformLoc.proj  = glGetUniformLocation(m_program, "u_proj");
	  uniformLoc.fogStart = glGetUniformLocation(m_program, "u_fogStart");
	  uniformLoc.fogEnd = glGetUniformLocation(m_program, "u_fogEnd");
	  uniformLoc.fogDensity = glGetUniformLocation(m_program, "u_fogDensity");
	  uniformLoc.fogModel = glGetUniformLocation(m_program, "u_fogMode");
	  uniformLoc.baseModel = glGetUniformLocation(m_program, "u_baseMode");
	  uniformLoc.boxTex  = glGetUniformLocation(m_program, "u_boxTex");

	}

}