 #include "oglRender.h"
#include "ogl/oglEulerCamera.h"

#include <memory>


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
        m_skybox.init();
		m_meshload.init();
		OGLEulerCamera::getInstance()->setPos(glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float> (glfwGetTime());
		GLfloat currentFrame = static_cast<float> (glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		OGLEulerCamera::getInstance()->update(deltaTime);
		m_skybox.update();
	}

    void OGLRender::v_render()
	{
		glEnable(GL_DEPTH_TEST);


		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		m_skybox.render();
		m_meshload.render();

    }
	void OGLRender::v_shutdown()
	{
		m_skybox.shutdown();
	}

}
