#include "RenderSystem.h"

namespace byhj
{
	void RenderSystem::v_InitInfo()
	{
		windowInfo.title += ": Point";
	}

	void RenderSystem::v_Init()
	{
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_MULTISAMPLE);  

		m_Cube.Init();
		m_Framebuffer.init(GetScreenWidth(), GetScreenHeight());
		m_Camera.SetPos( glm::vec3(0.0f, 0.0f, 3.0f) );
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
		matrix.proj  = glm::perspective(glm::radians(m_Camera.GetZoom() ), getAspect(), 0.1f, 1000.0f);
		matrix.model = glm::mat4(1.0f);

		// 1. Draw scene as normal in multisampled buffers
         
		m_Framebuffer.Bind();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    m_Cube.Render(matrix);

		// 2. Now blit multisampled buffer(s) to default framebuffers
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffer.GetFbo());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);				
		glBlitFramebuffer(0, 0, GetScreenWidth(), GetScreenWidth(), 0, 0, GetScreenWidth(), GetScreenWidth(), GL_COLOR_BUFFER_BIT, GL_NEAREST);     
	}

	void RenderSystem::v_Shutdown()
	{
		m_Cube.Shutdown();
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