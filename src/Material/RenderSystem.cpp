#include "RenderSystem.h"

const static glm::vec3 g_LightPos(1.0f, 0.5f, 1.0f);

namespace byhj
{
	void RenderSystem::v_InitInfo()
	{
		windowInfo.title += ": Cube";
	}

	void RenderSystem::v_Init()
	{
		glEnable(GL_DEPTH_TEST);

		cube.Init();
		light.Init();

		camera.SetPos( glm::vec3(0.0f, 0.0f, 3.0f) );
	}

	void RenderSystem::v_Render()
	{
		static const float bgColor[4] = {0.2f, 0.4f, 0.5f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, bgColor);
		static const float one = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &one);

		update();

		static ogl::MvpMatrix matrix;
		matrix.view  = camera.GetViewMatrix();
		matrix.proj  = glm::perspective(glm::radians( camera.GetZoom() ), getAspect(), 0.1f, 1000.0f);
		matrix.model = glm::rotate(glm::mat4(1.0f), 60.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		cube.Render(matrix, camera);

		//You should update the model matrix and notice the order to operator
		matrix.model = glm::translate(glm::mat4(1.0f), g_LightPos);
		matrix.model = glm::scale(matrix.model, glm::vec3(0.2f) );
	
		light.Render(matrix);
	}

	void RenderSystem::v_Shutdown()
	{
		cube.Shutdown();
		light.Shutdown();
	}

	/////////////////////////////////Key and Mouse//////////////////////////////////
	void RenderSystem::v_Movement(GLFWwindow *Triangle)
	{
		camera.movement(Triangle);
	}
	void RenderSystem::v_KeyCallback(GLFWwindow* Triangle, int key, int scancode, int action, int mode)
	{
		camera.key_callback(Triangle, key, scancode, action, mode);
	}

	void RenderSystem::v_MouseCallback(GLFWwindow* Triangle, double xpos, double ypos)
	{
		camera.mouse_callback(Triangle, xpos, ypos);
	}

	void RenderSystem::v_ScrollCallback(GLFWwindow* Triangle, double xoffset, double yoffset)
	{
		camera.scroll_callback(Triangle, xoffset, yoffset);
	}

	void RenderSystem::update()
	{
		static GLfloat lastFrame = static_cast<float>( glfwGetTime() );
		GLfloat currentFrame = static_cast<float>( glfwGetTime() );
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.update(deltaTime);
	}

}