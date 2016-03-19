#include "oglRender.h"
#include <memory>
#include "windowInfo.h"

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
		glEnable(GL_DEPTH_TEST);
		OGLEulerCamera::getInstance()->setPos(glm::vec3(0.0f, 0.0f, 5.0f));
		m_bloom.init();
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
		m_bloom.render();

	}
	void OGLRender::v_shutdown()
	{
	}
}
