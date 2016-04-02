#include "Planet.h"
#include "ogl/oglEulerCamera.h"
#include "modelMgr.h"
#include "windowInfo.h"

namespace byhj
{

	void Planet::init()
	{

		init_buffer();
		init_shader();
	}

	void Planet::update()
	{

		glUseProgram(m_Program);


		GLfloat time = glfwGetTime();
		m_Aspect = WindowInfo::getInstance()->getAspect();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -20.0f)) 
			            * glm::rotate(glm::mat4(1.0f), time / 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view  = OGLEulerCamera::getInstance()->getViewMat();
		glm::mat4 proj  = glm::perspective(45.0f, m_Aspect, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);

		glUseProgram(0);
	}

	void Planet::render()
	{
		glUseProgram(m_Program);

		ModelMgr::getInstance()->render("planet/planet.obj", m_Program);

		glUseProgram(0);

	}

	void Planet::shutdown()
	{
	}

	void Planet::init_buffer()
	{
		ModelMgr::getInstance()->loadOGLModel("planet/planet.obj");
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
