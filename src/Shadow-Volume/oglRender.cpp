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
		m_MeshLoad.Init();
		OGLEulerCamera::getInstance()->setPos(glm::vec3(0.0f, 5.0f, 25.0f));
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float> (glfwGetTime());
		GLfloat currentFrame = static_cast<float> (glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		OGLEulerCamera::getInstance()->update(deltaTime * 5.0f);

		m_MeshLoad.Update();
	}

    void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.2f, 0.3f, 0.5f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		glEnable(GL_DEPTH_TEST);

		m_MeshLoad.Render();

    }
	void OGLRender::v_shutdown()
	{

	}

}
