#include "bloom.h"
#include "textureMgr.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "windowInfo.h"

namespace byhj {
   
	void Bloom::init()
	{
		init_buffer();
		init_shader();
		init_texture();
	}

	void Bloom::update()
	{

	}

	void Bloom::render(const OGLCamera &camera)
	{
		glUseProgram(m_program);
		
		float aspect = WindowInfo::getInstance()->getAspect();
        float time = glfwGetTime();
		glm::mat4 model =  glm::rotate(glm::mat4(1.0f), time, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view  = camera.GetViewMatrix();
		glm::mat4 proj  = glm::perspective(45.0f,aspect, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTex);
		glUniform1i(glGetUniformLocation(m_program, "u_diffuseTex"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_glowTex);
		glUniform1i(glGetUniformLocation(m_program, "u_glowTex"), 1);

		m_model.draw(m_program);

		glUseProgram(0);
	}

	void Bloom::shutdown()
	{

	}


	void Bloom::init_buffer()
	{
		m_model.loadModel("UFO/UFO.obj", LoadType::OGL);
	}

	void Bloom::init_shader()
	{
		m_bloomShader.init();
		m_bloomShader.attach(GL_VERTEX_SHADER, "bloom.vert");
		m_bloomShader.attach(GL_FRAGMENT_SHADER, "bloom.frag");
		m_bloomShader.link();
		m_bloomShader.info();
		m_program = m_bloomShader.getProgram();
		uniform_loc.model = glGetUniformLocation(m_program, "u_model");
		uniform_loc.view  = glGetUniformLocation(m_program, "u_view");
		uniform_loc.proj  = glGetUniformLocation(m_program, "u_proj");
		
	}

	void Bloom::init_texture()
	{
		m_diffuseTex = TextureMgr::getInstance()->loadOGLDDS("gate_diffuse_rgb.dds");
		m_glowTex = TextureMgr::getInstance()->loadOGLDDS("gate_glow_rgb.dds");


	}
}