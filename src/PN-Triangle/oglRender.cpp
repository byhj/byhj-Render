 #include "oglRender.h"
#include <memory>

#include "ogl/oglEulerCamera.h"

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
		OGLEulerCamera::getInstance()->setPos(glm::vec3(0.0f, 0.0f, 5.0f));
		m_MeshLoad.Init(WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float> (glfwGetTime());
		GLfloat currentFrame = static_cast<float> (glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		OGLEulerCamera::getInstance()->update(deltaTime * 10.0f);
		m_MeshLoad.Update();
	}

    void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		m_MeshLoad.Render();

    }
	void OGLRender::v_shutdown()
	{

	}

}
