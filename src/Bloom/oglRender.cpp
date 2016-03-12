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

	void OGLRender::v_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		m_camera.key_callback(window, key, scancode, action, mode);
	}

	void OGLRender::v_movement(GLFWwindow *window)
	{
		m_camera.movement(window);
	}

	void OGLRender::v_mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		m_camera.mouse_callback(window, xpos, ypos);
	}

	void OGLRender::v_scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_camera.scroll_callback(window, xoffset, yoffset);
	}


	void OGLRender::v_init()
	{
		glEnable(GL_DEPTH_TEST);
		m_camera.SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
		m_bloom.init();
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float> (glfwGetTime());
		GLfloat currentFrame = static_cast<float> (glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		m_camera.update(deltaTime);

	}

	void OGLRender::v_render()
	{
		m_bloom.render(m_camera);

	}
	void OGLRender::v_shutdown()
	{
	}
}
