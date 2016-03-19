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
        m_plane.Init();
		m_grass.Init();
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
		glEnable(GL_BLEND);
		static const GLfloat black[] ={ 0.2f, 0.3f, 0.4f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);
		m_matrix.model = glm::mat4(1.0f);
		m_matrix.view = OGLEulerCamera::getInstance()->getViewMat();
		m_matrix.proj = glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f);
		m_plane.Render(m_matrix);
		m_grass.Render(OGLEulerCamera::getInstance()->getPos(), m_matrix);

    }
	void OGLRender::v_shutdown()
	{

	}

}
