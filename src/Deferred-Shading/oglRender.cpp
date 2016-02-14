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
		m_MeshLoad.Init(WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float> (glfwGetTime());
		GLfloat currentFrame = static_cast<float> (glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		m_camera.update(deltaTime);

		m_MeshLoad.Update(m_camera.GetViewMatrix());
	}

    void OGLRender::v_render()
	{


		m_MeshLoad.Render();

    }
	void OGLRender::v_shutdown()
	{
	}

}
