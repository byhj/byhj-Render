#include  "Skybox.h"
#include <glfw/glfw3.h>
#include "textureMgr.h"
#include "windowInfo.h"
#include "ogl/oglEulerCamera.h"

namespace byhj
{
 ///////////////////Vertex Data////////////////////////////////
 
 static const GLfloat VertexData[] =
 {
	 //Front
	 -0.5f, -0.5f,  0.5f,
	  0.5f, -0.5f,  0.5f, 
	 0.5f,  0.5f,  0.5f, 
	 -0.5f,  0.5f,  0.5f,

	 //Back		   
	 -0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f, 
	 0.5f,  0.5f, -0.5f, 
	 -0.5f,  0.5f, -0.5f,
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

	void Skybox::init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();
	}

	void Skybox::init_buffer()
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
	void Skybox::init_vertexArray()
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

	void Skybox::update()
	{

	}

	void Skybox::render()
	{
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(m_vao);
		glUseProgram(m_program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTex);
		GLuint texLoc = glGetUniformLocation(m_program, "u_skybox");
		glUniform1i(texLoc, 0);

		auto time = glfwGetTime();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view  = OGLEulerCamera::getInstance()->getViewMat();
		view = glm::mat4(glm::mat3(view));
		glm::mat4 proj  = glm::perspective(45.0f, WindowInfo::getInstance()->getAspect(), 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);

		glEnable(GL_DEPTH_TEST);
	}

	void  Skybox::shutdown()
	{
		glDeleteProgram(m_program);
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);
	}
	void Skybox::init_shader()
	{
	  m_SkyboxShader.init();
	  m_SkyboxShader.attach(GL_VERTEX_SHADER, "Skybox.vert");
	  m_SkyboxShader.attach(GL_FRAGMENT_SHADER, "Skybox.frag");
	  m_SkyboxShader.link();
	  m_SkyboxShader.info();
	  m_program = m_SkyboxShader.getProgram();

	  uniform_loc.model = glGetUniformLocation(m_program, "u_model");
	  uniform_loc.view  = glGetUniformLocation(m_program, "u_view");
	  uniform_loc.proj  = glGetUniformLocation(m_program, "u_proj");
	  uniform_loc.skytex   = glGetUniformLocation(m_program, "u_skybox");
	}

	void Skybox::init_texture()
	{
		std::vector<std::string> faces;
		faces.push_back("/house/posx.jpg");
		faces.push_back("/house/negx.jpg");
		faces.push_back("/house/posy.jpg");
		faces.push_back("/house/negy.jpg");
		faces.push_back("/house/negz.jpg");
		faces.push_back("/house/posz.jpg");

		m_skyboxTex = TextureMgr::getInstance()->loadOGLTexture("house", faces);
	}
}