#include "torus.h"
#include "windowInfo.h"
#include <glfw/glfw3.h>

namespace byhj {

	void Torus::init()
	{
		ModelMgr::getInstance()->loadOGLModel("polygon/torus.obj");
		m_TorusShader.init();
		m_TorusShader.attach(GL_VERTEX_SHADER, "toon.vert");
		m_TorusShader.attach(GL_FRAGMENT_SHADER, "toon.frag");
		m_TorusShader.link();
		m_TorusShader.info();

		m_program = m_TorusShader.getProgram();
		uniform_loc.model = glGetUniformLocation(m_program, "u_Model");
		uniform_loc.view  = glGetUniformLocation(m_program, "u_View");
		uniform_loc.proj  = glGetUniformLocation(m_program, "u_Proj");
	}

	void Torus::update()
	{
		glUseProgram(m_program);

		GLfloat time = glfwGetTime();
		static auto aspect = WindowInfo::getInstance()->getAspect();

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), time, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view  = glm::lookAt(glm::vec3(0.0f, 0.2f, 3.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj  = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);

		glUseProgram(0);
	}

	void Torus::render()
	{
		glUseProgram(m_program);

		ModelMgr::getInstance()->render(m_program); 

		glUseProgram(0);
	}

	void Torus::shutdown()
	{

	}

}