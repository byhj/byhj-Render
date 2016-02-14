#include "MeshLoad.h"
#include "windowInfo.h"

namespace byhj
{
	GLfloat planeVertices[] ={
		// Positions        // Texture Coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	void MeshLoad::Init(int sw, int sh)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		m_LightGui.v_init(sw, sh);
		m_RotationGui.v_init(sw, sh);

		init_buffer();
		init_shader();
		init_fbo();
	}

	void MeshLoad::Update(const glm::mat4 &camMat)
	{

		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat one[] ={ 1.0f };
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fbo);
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_COLOR, 1, black);
		glClearBufferfv(GL_COLOR, 2, black);
		glClearBufferfv(GL_DEPTH, 0, one);

		glUseProgram(m_DeferredProgram);
	

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) * m_RotationGui.getRotationMat();
		glm::mat4 view  = camMat;
		glm::mat4 proj  = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);
		glUniform3fv(uniform_loc.ambient, 1, &m_LightGui.getLight(0)[0]);

		m_Model.draw(m_DeferredProgram);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, one);

		glUseProgram(m_LightProgram);
		glUniform1i(glGetUniformLocation(m_LightProgram, "posTex"), 0);
		glUniform1i(glGetUniformLocation(m_LightProgram, "normalTex"), 1);
		glUniform1i(glGetUniformLocation(m_LightProgram, "colorTex"), 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer.posTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gbuffer.normalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gbuffer.colorTex);

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		m_LightGui.v_update();
		m_RotationGui.v_update();

	    GLfloat time = glfwGetTime();



		glUseProgram(0);
	}

	void MeshLoad::Render()
	{


		glDisable(GL_DEPTH_TEST);

		m_RotationGui.v_render();
		m_LightGui.v_render();

		glEnable(GL_DEPTH_TEST);


		glUseProgram(0);
	}

	void MeshLoad::Shutdown()
	{
	}

	void MeshLoad::init_buffer()
	{
		m_Model.loadModel("armadillo.obj", LoadType::OGL);
		sw = WindowInfo::getInstance()->getWidth();
		sh = WindowInfo::getInstance()->getHeight();
		aspect = WindowInfo::getInstance()->getAspect();

		glGenBuffers(1, &planeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glGenVertexArrays(1, &planeVAO);

		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);
	}


	void MeshLoad::init_shader()
	{
		m_DeferredShader.init();
		m_DeferredShader.attach(GL_VERTEX_SHADER,   "deferred.vert");
		m_DeferredShader.attach(GL_FRAGMENT_SHADER, "deferred.frag");
		m_DeferredShader.link();
		m_DeferredShader.info();
		m_DeferredProgram= m_DeferredShader.getProgram();
		uniform_loc.model =  glGetUniformLocation(m_DeferredProgram, "u_Model");
		uniform_loc.view  =  glGetUniformLocation(m_DeferredProgram, "u_View");
		uniform_loc.proj  =  glGetUniformLocation(m_DeferredProgram, "u_Proj");

		m_LightShader.init();
		m_LightShader.attach(GL_VERTEX_SHADER,   "light.vert");
		m_LightShader.attach(GL_FRAGMENT_SHADER, "light.frag");
		m_LightShader.link();
		m_LightShader.info();
		m_LightProgram= m_LightShader.getProgram();
	}

	void MeshLoad::init_fbo()
	{
		// Set up G-Buffer
		// 3 textures:
		// 1. Positions (RGB)
		// 2. Color (RGB) + Specular (A)
		// 3. Normals (RGB) 

		glGenFramebuffers(1, &gbuffer.fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fbo);

		glGenTextures(1, &gbuffer.posTex);
		glBindTexture(GL_TEXTURE_2D, gbuffer.posTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sw, sh, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbuffer.posTex, 0);

		// - Normal color buffer
		glGenTextures(1, &gbuffer.normalTex);
		glBindTexture(GL_TEXTURE_2D, gbuffer.normalTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sw, sh, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbuffer.normalTex, 0);

		// - Color + Specular color buffer
		glGenTextures(1, &gbuffer.colorTex);
		glBindTexture(GL_TEXTURE_2D, gbuffer.colorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sw, sh, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gbuffer.colorTex, 0);

		// - Tell OpenGL which color attachments we'll use (of this frame buffer) for rendering 
		GLuint attachments[3] ={ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
		// - Create and attach depth buffer (render buffer)
		GLuint rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, sw, sh);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		// - Finally check if frame buffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Frame buffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
