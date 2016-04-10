#include "MeshLoad.h"
#include "modelMgr.h"
#include "ogl/oglEulerCamera.h"

namespace byhj
{
	void MeshLoad::Init(int sw, int sh)
	{


		m_Aspect = static_cast<GLfloat>(sw) / sh;

		m_LightGui.v_init();

		init_shader();
		init_buffer();

	}

	void MeshLoad::Update()
	{

		glUseProgram(m_Program);

		m_LightGui.v_update();

	    GLfloat time = glfwGetTime();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view  = OGLEulerCamera::getInstance()->getViewMat();
		glm::mat4 proj  = glm::perspective(45.0f, m_Aspect, 0.1f, 1000.0f);
		glm::vec3 camPos = OGLEulerCamera::getInstance()->getPos();
		//std::cout << camPos.x << " " << camPos.y << " " << camPos.z << std::endl;

		glUniform3fv(uniform_loc.camPos, 1, &camPos[0]);
		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);

		glUseProgram(0);
	}

	void MeshLoad::Render()
	{
		glEnable(GL_DEPTH_TEST);

		glUseProgram(m_Program);

		ModelMgr::getInstance()->render(m_Program);

		m_LightGui.v_render();

		glUseProgram(0);
	}

	void MeshLoad::Shutdown()
	{
	}

	void MeshLoad::init_buffer()
	{
		ModelMgr::getInstance()->loadOGLModel("sponza/sponza.obj");
	}


	void MeshLoad::init_shader()
	{
		m_MeshLoadShader.init();
		m_MeshLoadShader.attach(GL_VERTEX_SHADER, "model.vert");
		m_MeshLoadShader.attach(GL_FRAGMENT_SHADER, "model.frag");
		m_MeshLoadShader.link();
		m_MeshLoadShader.info();

		m_Program = m_MeshLoadShader.getProgram();
		uniform_loc.model = glGetUniformLocation(m_Program, "u_model");
		uniform_loc.view  = glGetUniformLocation(m_Program, "u_view");
		uniform_loc.proj  = glGetUniformLocation(m_Program, "u_proj");
		uniform_loc.camPos = glGetUniformLocation(m_Program, "u_camPos");
		
	}
}
