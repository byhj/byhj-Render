#include "MeshLoad.h"
#include "windowInfo.h"
#include "ogl/oglEulerCamera.h"
#include "textureMgr.h"

namespace byhj
{
	GLfloat planeVertices[] ={
		// Positions        // Texture Coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	void MeshLoad::init()
	{
		init_buffer();
		init_shader();
		init_fbo();
	}

	void MeshLoad::update()
	{


	}

	void MeshLoad::render()
	{
		glEnable(GL_DEPTH_TEST);

		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat one[] ={ 1.0f };
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fbo);
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_COLOR, 1, black);
		glClearBufferfv(GL_DEPTH, 0, one);

		glUseProgram(m_DeferredProgram);

		static glm::mat4 preHouseMVP = glm::mat4(1.0f);
		static glm::mat4 preFanMVP = glm::mat4(1.0f);

		glm::vec3 pos   = glm::vec3(0.00000000f, 8.74925041f, 2.67939997f);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), -pos);
		glm::mat4 view  = OGLEulerCamera::getInstance()->getViewMat();
		glm::mat4 proj  = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
		glm::mat4 houseMVP = proj * view * model;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_tex);
		glUniform1i(glGetUniformLocation(m_DeferredProgram, "texture_diffuse"), 0);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);
		glUniformMatrix4fv(uniform_loc.prevMVP, 1, GL_FALSE, &preHouseMVP[0][0]);

		preHouseMVP = houseMVP;

		ModelMgr::getInstance()->render("house.obj", m_DeferredProgram);

		model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f)) * model;
		glm::mat4 fanMVP = proj * view * model;

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.prevMVP, 1, GL_FALSE, &preFanMVP[0][0]);

		ModelMgr::getInstance()->render("fan.obj", m_DeferredProgram);
		//////////////////////////////////////////////////////////////////////////////
	    preFanMVP = fanMVP;

		glUseProgram(m_LightProgram);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, one);


		glUniform1i(glGetUniformLocation(m_LightProgram, "motionVec"), 0);
		glUniform1i(glGetUniformLocation(m_LightProgram, "colorTex"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer.posTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gbuffer.colorTex);

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		GLfloat time = glfwGetTime();
		glUseProgram(0);

	
	}

	void MeshLoad::shutdown()
	{
	}

	void MeshLoad::init_buffer()
	{
		ModelMgr::getInstance()->loadOGLModel("fan.obj");
		ModelMgr::getInstance()->loadOGLModel("house.obj");

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
		m_DeferredShader.attach(GL_VERTEX_SHADER,   "motion.vert");
		m_DeferredShader.attach(GL_FRAGMENT_SHADER, "motion.frag");
		m_DeferredShader.link();
		m_DeferredShader.info();
		m_DeferredProgram= m_DeferredShader.getProgram();
		uniform_loc.model =  glGetUniformLocation(m_DeferredProgram, "u_Model");
		uniform_loc.view  =  glGetUniformLocation(m_DeferredProgram, "u_View");
		uniform_loc.proj  =  glGetUniformLocation(m_DeferredProgram, "u_Proj");
		uniform_loc.prevMVP = glGetUniformLocation(m_DeferredProgram, "u_prevMVP");

		m_LightShader.init();
		m_LightShader.attach(GL_VERTEX_SHADER,   "light.vert");
		m_LightShader.attach(GL_FRAGMENT_SHADER, "light.frag");
		m_LightShader.link();
		m_LightShader.info();
		m_LightProgram= m_LightShader.getProgram();

		m_tex = TextureMgr::getInstance()->loadOGLDDS("windmill_diffuse.dds");
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, sw, sh, 0, GL_RG, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbuffer.posTex, 0);


		// - Color + Specular color buffer
		glGenTextures(1, &gbuffer.colorTex);
		glBindTexture(GL_TEXTURE_2D, gbuffer.colorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sw, sh, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbuffer.colorTex, 0);

		// - Tell OpenGL which color attachments we'll use (of this frame buffer) for rendering 
		GLuint attachments[2] ={ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);

		// - Create and attach depth buffer (render buffer)
		GLuint rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, sw, sh);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		// - Finally check if frame buffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Frame buffer not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
