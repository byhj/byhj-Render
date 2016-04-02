 #include "oglRender.h"
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
		m_scene.init();
		OGLEulerCamera::getInstance()->setPos(glm::vec3(0.0f, 2.0f, 5.0f));
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
		glEnable(GL_DEPTH_TEST);
		m_scene.render();
    }
	void OGLRender::v_shutdown()
	{
	}

}
