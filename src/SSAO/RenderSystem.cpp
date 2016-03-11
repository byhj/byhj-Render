#include "RenderSystem.h"

namespace byhj
{
	void RenderSystem::v_InitInfo()
	{
		windowInfo.title += ": Bloom";
	}

	void RenderSystem::v_Init()
	{
		glEnable(GL_DEPTH_TEST);

		m_Scene.init_fbo(GetScreenWidth(), GetScreenHeight());
		m_Scene.Init();
		m_Camera.SetPos( glm::vec3(0.0f, 0.0f, 5.0f) );
	}

	void RenderSystem::v_Render()
	{
		static const float bgColor[4] = {0.2f, 0.3f, 0.4f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, bgColor);
		static const float one = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &one);

		update();

		static ogl::MvpMatrix matrix;
		matrix.view  = m_Camera.GetViewMatrix();
		matrix.proj  = glm::perspective(glm::radians(m_Camera.GetZoom() ), GetAspect(), 0.1f, 1000.0f);
		matrix.model = glm::mat4(1.0f);

	    m_Scene.Render(matrix, m_Camera);
	}

	void RenderSystem::v_Shutdown()
	{
		m_Scene.Shutdown();
	}

	/////////////////////////////////Key and Mouse//////////////////////////////////
	void RenderSystem::v_Movement(GLFWwindow *window)
	{
		m_Camera.movement(window);
	}
	void RenderSystem::v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		m_Camera.key_callback(window, key, scancode, action, mode);
	}

	void RenderSystem::v_MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		m_Camera.mouse_callback(window, xpos, ypos);
	}

	void RenderSystem::v_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_Camera.scroll_callback(window, xoffset, yoffset);
	}

	void RenderSystem::update()
	{
		static GLfloat lastFrame = static_cast<float>( glfwGetTime() );
		GLfloat currentFrame = static_cast<float>( glfwGetTime() );
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		m_Camera.update(deltaTime);
	}

}