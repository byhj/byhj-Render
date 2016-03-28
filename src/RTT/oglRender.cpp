#include "oglRender.h"

namespace byhj
{
	OGLRender::OGLRender()
	{


	}

	OGLRender::~OGLRender()
	{

	}

	void OGLRender::v_init()
	{
		m_plane.init();
		m_oglRTT.init(glm::vec2(0, 0), glm::vec2(1.0, 1.0) );
		OGLEulerCamera::getInstance()->setPos( glm::vec3(0.0f, 3.0f, 10.0f) );
		init_fbo();
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float> (glfwGetTime());
		GLfloat currentFrame = static_cast<float> (glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		OGLEulerCamera::getInstance()->update(deltaTime);
	}

	void OGLRender::v_render()
	{
		GLfloat bgColor[] ={ 0.0f, 0.0f, 0.0f, 0.0f };
		glClearBufferfv(GL_COLOR, 0, bgColor);

		glm::mat4 model = glm::mat4();
		model = glm::scale(model, glm::vec3(5.0f, 0.0f, 5.0f));
		glm::mat4 view  = OGLEulerCamera::getInstance()->getViewMat();
		glm::mat4 proj  = glm::perspective(glm::radians(OGLEulerCamera::getInstance()->getZoom()), 1.5f, 0.1f, 1000.0f);
        glm::mat4 mvp = proj * view * model;

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_plane.render(mvp);

	    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    m_oglRTT.render(colorBuffer);
	}

	void OGLRender::v_shutdown()
	{

	}

	void OGLRender::init_fbo()
	{
		glGenFramebuffers(1, &fbo);

		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();

		//Create floating point color buffer
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sw, sh, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Create depth buffer(render buffer)
		GLuint rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, sw, sh);

		//Attach buffers
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Frame buffer not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}