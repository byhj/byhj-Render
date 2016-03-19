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
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float>(glfwGetTime());
		GLfloat currentFrame = static_cast<float>(glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

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
	
	}

	void OGLRender::v_shutdown()
	{
	
	}


}