#include "MeshLoad.h"

namespace byhj
{
	void MeshLoad::Init(int sw, int sh)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		m_Aspect = static_cast<GLfloat>(sw) / sh;

		m_RotationGui.v_init(sw, sh);

		init_buffer();
		init_shader();
	}

	void MeshLoad::Update(const glm::mat4 &camMat)
	{

		glUseProgram(m_program);

		m_RotationGui.v_update();

	    GLfloat time = glfwGetTime();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, -15.0f)) * m_RotationGui.getRotationMat();
		glm::mat4 view  = camMat;
		glm::mat4 proj  = glm::perspective(45.0f, m_Aspect, 0.1f, 1000.0f);
		glm::vec3 camPos;
		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);
		glUniform3fv(uniform_loc.camPos, 1, &camPos[0]);
		glUseProgram(0);
	}

	void MeshLoad::Render()
	{
		glUseProgram(m_program);

		glUniform1i(uniform_loc.tex, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureMgr::getInstance()->getOGLTextureByName("sea"));

		m_Model.draw(m_program);

		glDisable(GL_DEPTH_TEST);

		m_RotationGui.v_render();

		glEnable(GL_DEPTH_TEST);


		glUseProgram(0);
	}

	void MeshLoad::Shutdown()
	{
	}

	void MeshLoad::init_buffer()
	{
		m_Model.loadModel("nanosuit/nanosuit.obj", LoadType::OGL);
	}


	void MeshLoad::init_shader()
	{
		m_MeshLoadShader.init();
		m_MeshLoadShader.attach(GL_VERTEX_SHADER,   "refract.vert");
		m_MeshLoadShader.attach(GL_FRAGMENT_SHADER, "refract.frag");
		m_MeshLoadShader.link();
		m_MeshLoadShader.info();

		m_program = m_MeshLoadShader.getProgram();
		uniform_loc.model = glGetUniformLocation(m_program, "u_Model");
		uniform_loc.view  = glGetUniformLocation(m_program, "u_View");
		uniform_loc.proj  = glGetUniformLocation(m_program, "u_Proj");
		uniform_loc.camPos = glGetUniformLocation(m_program, "u_CamPos");
		uniform_loc.tex   = glGetUniformLocation(m_program, "skybox");
		
	}
}
