#include "MeshLoad.h"
#include "modelMgr.h"
#include "windowInfo.h"
#include <glfw/glfw3.h>

namespace byhj {

	void MeshLoad::init()
	{
		init_buffer();
		init_shader();
		init_fbo();
	}

	void MeshLoad::render()
	{

		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat gray[] ={ 0.1f, 0.1f, 0.1f, 1.0f };
		static const GLfloat one = 1.0f;
		float t = glfwGetTime() / 100.0f;

		glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f))
			* glm::rotate(glm::mat4(1.0f), (float)t * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::rotate(glm::mat4(1.0f), (float)t * 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 proj_matrix = glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f);
		glm::mat4 mvp = proj_matrix * mv_matrix;

		struct TRANSFORM_BUFFER
		{
			glm::mat4 proj_matrix;
			glm::mat4 mv_matrix[16];
		};

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);

		TRANSFORM_BUFFER * buffer = (TRANSFORM_BUFFER *)glMapBufferRange
			(
				GL_UNIFORM_BUFFER, 0, sizeof(TRANSFORM_BUFFER),
				GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
				);

		buffer->proj_matrix = glm::perspective(50.0f, 1.5f, 0.1f, 1000.0f); // proj_matrix;
		int i;

		for (i = 0; i < 16; i++)
		{
			float fi = (float)(i + 12) / 16.0f;
			buffer->mv_matrix[i] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f))
				* glm::rotate(glm::mat4(1.0f), (float)t * 25.0f * fi, glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::rotate(glm::mat4(1.0f), (float)t * 30.0f * fi, glm::vec3(1.0f, 0.0f, 0.0f));
		}

		glUnmapBuffer(GL_UNIFORM_BUFFER);



		glBindFramebuffer(GL_FRAMEBUFFER, layered_fbo);
		static const GLenum ca0 = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &ca0);
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glUseProgram(layer_prog);

		ModelMgr::getInstance()->render("teapot.obj", layer_prog);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glUseProgram(0); 


		glUseProgram(show_prog);
		glClearBufferfv(GL_COLOR, 0, gray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, array_texture);
		glDisable(GL_DEPTH_TEST);

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 16);

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}


	void MeshLoad::shutdown()
	{
	}

	void MeshLoad::init_buffer()
	{

		ModelMgr::getInstance()->loadOGLModel("teapot.obj");

		glGenBuffers(1, &m_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glBufferData(GL_UNIFORM_BUFFER, 17 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

		glBindVertexArray(0);

	}


	void  MeshLoad::init_shader()
	{
		m_showShader.init();
		m_showShader.attach(GL_VERTEX_SHADER, "show.vert");
		m_showShader.attach(GL_FRAGMENT_SHADER, "show.frag");
		m_showShader.link();
		show_prog = m_showShader.getProgram();

		m_layerShader.init();
		m_layerShader.attach(GL_VERTEX_SHADER, "layer.vert");
		m_layerShader.attach(GL_GEOMETRY_SHADER, "layer.geom");
		m_layerShader.attach(GL_FRAGMENT_SHADER, "layer.frag");
		m_layerShader.link();
		layer_prog = m_layerShader.getProgram();

	}

	void MeshLoad::init_fbo()
	{
		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();

		glGenTextures(1, &array_texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, array_texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, sw, sh, 16);

		glGenTextures(1, &array_depth);
		glBindTexture(GL_TEXTURE_2D_ARRAY, array_depth);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT32,  sw, sh, 16);

		//bind the fbo to color and depth buffer, notice the texture is array for layer render
		glGenFramebuffers(1, &layered_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, layered_fbo);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, array_texture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, array_depth, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

}