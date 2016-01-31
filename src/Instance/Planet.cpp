#include "Planet.h"

namespace byhj
{

	void Planet::Init(int sw, int sh)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		m_Aspect = static_cast<GLfloat>(sw) / sh;

		init_buffer();
		init_shader();
	}

	void Planet::Update(const glm::mat4 &camMat)
	{

		glUseProgram(m_Program);


		GLfloat time = glfwGetTime();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -20.0f)) 
			            * glm::rotate(glm::mat4(1.0f), time / 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view  = camMat;
		glm::mat4 proj  = glm::perspective(45.0f, m_Aspect, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);

		glUseProgram(0);
	}

	void Planet::Render()
	{
		glUseProgram(m_Program);
		glBindVertexArray(m_vao);

		m_Model.draw(m_Program);

		glUseProgram(0);
	}

	void Planet::Shutdown()
	{
	}

	void Planet::init_buffer()
	{
		m_Model.loadModel("planet/planet.obj", LoadType::OGL);
	}


	void Planet::init_shader()
	{
		m_PlanetShader.init();
		m_PlanetShader.attach(GL_VERTEX_SHADER, "Planet.vert");
		m_PlanetShader.attach(GL_FRAGMENT_SHADER, "Planet.frag");
		m_PlanetShader.link();
		m_PlanetShader.info();

		m_Program = m_PlanetShader.getProgram();
		uniform_loc.model = glGetUniformLocation(m_Program, "u_Model");
		uniform_loc.view  = glGetUniformLocation(m_Program, "u_View");
		uniform_loc.proj  = glGetUniformLocation(m_Program, "u_Proj");

	}
}
