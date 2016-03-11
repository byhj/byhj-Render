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
		m_MeshLoad.Init();
		m_camera.SetPos(glm::vec3(0.0f, 5.0f, 25.0f));
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float> (glfwGetTime());
		GLfloat currentFrame = static_cast<float> (glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		m_camera.update(deltaTime * 5.0f);

		m_MeshLoad.Update();
	}

    void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		glEnable(GL_DEPTH_TEST);

		m_MeshLoad.Render(m_camera);

    }
	void OGLRender::v_shutdown()
	{

	}

}
