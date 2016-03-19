#include "fur.h"

namespace byhj
{
	void Fur::init()
	{
		init_buffer();
		init_shader();

	}

	void Fur::update()
	{
		glUseProgram(m_program);

		auto model = glm::mat4(1.0f);
		auto view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		auto proj  = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view,  1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj,  1, GL_FALSE, &proj[0][0]);

		glUseProgram(0);
	}

	void Fur::render()
	{
		glUseProgram(m_program);

		ModelMgr::getInstance()->render(m_program);

		glUseProgram(0);

	}

	void Fur::shutdown()
	{

	}

	void Fur::init_buffer()
	{
		ModelMgr::getInstance()->loadOGLModel("bunny2.obj");
	}

	void Fur::init_shader()
	{
		m_FurShader.init();
		m_FurShader.attach(GL_VERTEX_SHADER, "fur.vert");
		m_FurShader.attach(GL_GEOMETRY_SHADER, "fur.geom");
		m_FurShader.attach(GL_FRAGMENT_SHADER, "fur.frag");
		m_FurShader.link();
		m_FurShader.info();

		m_program = m_FurShader.getProgram();
		uniform_loc.model = glGetUniformLocation(m_program, "g_Model");
		uniform_loc.view  = glGetUniformLocation(m_program, "g_View");
		uniform_loc.proj  = glGetUniformLocation(m_program, "g_Proj");
	}
}