#include "OGLRender.h"
#include "windowInfo.h"

const static glm::vec3 g_LightPos(1.5f, 0.5f, 0.0f);

namespace byhj
{

	void OGLRender::v_init()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_Cube.init();
		m_Plane.init();
		m_Window.init();

		m_Camera.SetPos( glm::vec3(0.0f, 0.0f, 3.0f) );
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float>(glfwGetTime());
		GLfloat currentFrame = static_cast<float>(glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		m_Camera.update(deltaTime);
	}

	void OGLRender::v_render()
	{
		static const float bgColor[4] = {0.2f, 0.4f, 0.5f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, bgColor);
		static const float one = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &one);

		static ogl::Matrix matrix;
		matrix.view  = m_Camera.GetViewMatrix();
		matrix.proj  = glm::perspective(glm::radians(m_Camera.GetZoom() ),
			                            WindowInfo::getInstance()->getAspect(), 0.1f, 1000.0f);
		matrix.model = glm::mat4(1.0f);

		m_Cube.render(matrix);
		m_Plane.render(matrix);
		m_Window.render(matrix, m_Camera.GetPos());
	
	}

	void OGLRender::v_shutdown()
	{
		m_Cube.shutdown();
		m_Plane.shutdown();
		m_Window.shutdown();
	}

	/////////////////////////////////Key and Mouse//////////////////////////////////
	void OGLRender::v_movement(GLFWwindow *Triangle)
	{
		m_Camera.movement(Triangle);
	}
	void OGLRender::v_keyCallback(GLFWwindow* Triangle, int key, int scancode, int action, int mode)
	{
		m_Camera.key_callback(Triangle, key, scancode, action, mode);
	}

	void OGLRender::v_mouseCallback(GLFWwindow* Triangle, double xpos, double ypos)
	{
		m_Camera.mouse_callback(Triangle, xpos, ypos);
	}

	void OGLRender::v_scrollCallback(GLFWwindow* Triangle, double xoffset, double yoffset)
	{
		m_Camera.scroll_callback(Triangle, xoffset, yoffset);
	}



}