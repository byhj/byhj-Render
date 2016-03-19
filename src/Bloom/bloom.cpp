#include "bloom.h"
#include "textureMgr.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "windowInfo.h"

namespace byhj {

	GLfloat VertexData[] ={
		// Positions        // Texture Coords
		-1.0f,  1.0f, 0.0f,    0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
		1.0f,  1.0f, 0.0f,    1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,    1.0f, 0.0f,
	};


	void Bloom::init()
	{
		 init_buffer();
		 init_vertexArray();
		 init_shader();
		 init_texture();
		 init_fbo();
	}

	void Bloom::update()
	{

	}

	void Bloom::render()
	{
		static float t = 0.0f;
		t += glfwGetTime() / 1000.0f;
		float intensity = abs(sin(t)) * 10.0f;

		glBindFramebuffer(GL_FRAMEBUFFER, m_bloomFbo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_bloomProgram); 

		float aspect = WindowInfo::getInstance()->getAspect();
        float time = glfwGetTime();
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0f, 1.0f, 1.0f));
		glm::mat4 view  = OGLEulerCamera::getInstance()->getViewMat();
		glm::mat4 proj  = glm::perspective(45.0f,aspect, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);

		intensity_loc = glGetUniformLocation(m_bloomProgram, "u_intensity");
		glUniform1f(intensity_loc, intensity);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTex);
		glUniform1i(glGetUniformLocation(m_bloomProgram, "u_diffuseTex"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_glowTex);
		glUniform1i(glGetUniformLocation(m_bloomProgram, "u_glowTex"), 1);

	     ModelMgr::getInstance()->render(m_bloomProgram);

		glUseProgram(0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

		/////////////////////////////////////////////////////////////////////////////////////
	
		glUseProgram(m_blurProgram);
		GLboolean horizontal = true, first_iteration = true;
		GLuint amount = 10;
		for (GLuint i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_blurFbos[horizontal]);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUniform1i(glGetUniformLocation(m_bloomProgram, "u_horizontal"), horizontal);

			// bind texture of other framebuffer (or scene if first iteration)
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(m_blurProgram, "u_lightTex"), 0);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? m_bloomTexs[1] : m_blurTexs[!horizontal]);

			render_plane();

			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}

		glUseProgram(0);

		/////////////////////////////Final//////////////////////////////////
// 3. Now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_sceneProgram);
		
		glActiveTexture(GL_TEXTURE0);
		auto sceneTexLoc = glGetUniformLocation(m_sceneProgram, "u_sceneTex");
		glUniform1i(sceneTexLoc, 0);
		glBindTexture(GL_TEXTURE_2D, m_bloomTexs[0]);

		glActiveTexture(GL_TEXTURE1);
		auto bloomTexLoc = glGetUniformLocation(m_sceneProgram, "u_bloomTex");
		glUniform1i(bloomTexLoc, 1);
		glBindTexture(GL_TEXTURE_2D, m_blurTexs[!horizontal]);

		glUniform1i(glGetUniformLocation(m_sceneProgram, "enableBloom"), 1);
		glUniform1f(glGetUniformLocation(m_sceneProgram, "exposure"), 0.01f);

		render_plane();

		glUseProgram(0);
		
	}

	void Bloom::render_plane()
	{
		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
	}
	void Bloom::shutdown()
	{

	}


	void Bloom::init_buffer()
	{
	    ModelMgr::getInstance()->loadOGLModel("UFO/UFO.obj");

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);    //load the vertex data
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}
	void Bloom::init_vertexArray()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		glBindVertexArray(0);
	}


	void Bloom::init_shader()
	{
		m_bloomShader.init();
		m_bloomShader.attach(GL_VERTEX_SHADER, "bloom.vert");
		m_bloomShader.attach(GL_FRAGMENT_SHADER, "bloom.frag");
		m_bloomShader.link();
		m_bloomShader.info();
		m_bloomProgram = m_bloomShader.getProgram();
		uniform_loc.model = glGetUniformLocation(m_bloomProgram, "u_model");
		uniform_loc.view  = glGetUniformLocation(m_bloomProgram, "u_view");
		uniform_loc.proj  = glGetUniformLocation(m_bloomProgram, "u_proj");

		m_blurShader.init();
		m_blurShader.attach(GL_VERTEX_SHADER, "blur.vert");
		m_blurShader.attach(GL_FRAGMENT_SHADER, "blur.frag");
		m_blurShader.link();
		m_blurShader.info();
		m_blurProgram = m_blurShader.getProgram();

		m_sceneShader.init();
		m_sceneShader.attach(GL_VERTEX_SHADER, "scene.vert");
		m_sceneShader.attach(GL_FRAGMENT_SHADER, "scene.frag");
		m_sceneShader.link();
		m_sceneShader.info();
		m_sceneProgram = m_sceneShader.getProgram();
	}

	void Bloom::init_texture()
	{
		m_diffuseTex = TextureMgr::getInstance()->loadOGLDDS("gate_diffuse_rgb.dds");
		m_glowTex = TextureMgr::getInstance()->loadOGLDDS("gate_glow_rgb.dds");
	}

	void Bloom::init_fbo()
	{
		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();

		//Create a floating point framebuffer for render bloom
	    glGenFramebuffers(1, &m_bloomFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_bloomFbo);

		//Create two floating piont color buffers(1 for normal rendering, other for brightness)
		glGenTextures(2, m_bloomTexs);
		for (GLuint i = 0; i < 2; ++i )  {
			glBindTexture(GL_TEXTURE_2D, m_bloomTexs[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sw, sh, 0, GL_RGB, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
			//Attach the color texture to the framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_bloomTexs[i], 0);
	     }

			//Create and attach depth buffer (render buffer)
			glGenRenderbuffers(1, &m_depthRbo);
			glBindRenderbuffer(GL_RENDERBUFFER, m_depthRbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, sw, sh);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRbo);

			// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
			GLuint attachments[2] ={ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, attachments);

			// - Finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


			//Create two ping pong framebuffer for blurring
			glGenFramebuffers(2, m_blurFbos);
			glGenTextures(2, m_blurTexs);
			for (GLuint i = 0; i < 2; ++i) {
				glBindFramebuffer(GL_FRAMEBUFFER, m_blurFbos[i]);
				glBindTexture(GL_TEXTURE_2D, m_blurTexs[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sw, sh, 0, GL_RGB, GL_FLOAT, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				//Attach the color texture to the framebuffer
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurTexs[i], 0);
				// Also check if framebuffers are complete (no need for depth buffer)
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
					std::cout << "Framebuffer not complete!" << std::endl;
			}
		    glBindFramebuffer(GL_FRAMEBUFFER, 0);
		    		    
		}
}