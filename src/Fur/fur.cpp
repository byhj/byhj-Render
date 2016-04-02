#include "fur.h"
#include "windowInfo.h"

namespace byhj
{
	void Fur::init()
	{
		init_buffer();
		init_shader();

	}

	void Fur::update()
	{
		
		static GLfloat lastFrame = static_cast<float> (glfwGetTime());
		GLfloat currentFrame = static_cast<float> (glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		OGLEulerCamera::getInstance()->update(deltaTime);
	}

	void Fur::render()
	{
		glEnable(GL_DEPTH_TEST);

		auto model = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
		auto view  = OGLEulerCamera::getInstance()->getViewMat();
		auto proj  = glm::perspective(45.0f, WindowInfo::getInstance()->getAspect(), 0.1f, 1000.0f);

		glUseProgram(base_prog);

		glUniformMatrix4fv(baseLoc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(baseLoc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(baseLoc.proj, 1, GL_FALSE, &proj[0][0]);

		ModelMgr::getInstance()->render(base_prog);

		//////////////////////////////////////////////////////////////////////////////////////////////////

		glUseProgram(0);

		glUseProgram(fur_prog);
		glUniformMatrix4fv(furLoc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(furLoc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(furLoc.proj, 1, GL_FALSE, &proj[0][0]);

		ModelMgr::getInstance()->render(fur_prog);

		glUseProgram(0);

	}

	void Fur::shutdown()
	{

	}

	void Fur::init_buffer()
	{
		ModelMgr::getInstance()->loadOGLModel("bunny.obj");
	}

	void Fur::init_shader()
	{
		m_FurShader.init();
		m_FurShader.attach(GL_VERTEX_SHADER, "fur.vert");
		m_FurShader.attach(GL_GEOMETRY_SHADER, "fur.geom");
		m_FurShader.attach(GL_FRAGMENT_SHADER, "fur.frag");
		m_FurShader.link();
		m_FurShader.info();

		fur_prog = m_FurShader.getProgram();
		furLoc.model = glGetUniformLocation(fur_prog, "u_model");
		furLoc.view  = glGetUniformLocation(fur_prog, "u_view");
		furLoc.proj  = glGetUniformLocation(fur_prog, "u_proj");

		m_BaseShader.init();
		m_BaseShader.attach(GL_VERTEX_SHADER,   "base.vert");
		m_BaseShader.attach(GL_FRAGMENT_SHADER, "base.frag");
		m_BaseShader.link();
		m_BaseShader.info();

		base_prog = m_BaseShader.getProgram();
		baseLoc.model = glGetUniformLocation(base_prog, "u_model");
		baseLoc.view  = glGetUniformLocation(base_prog, "u_view");
		baseLoc.proj  = glGetUniformLocation(base_prog, "u_proj");
	}
}