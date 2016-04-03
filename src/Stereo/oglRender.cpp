#include "OGLRender.h"
#include "windowInfo.h"

const static glm::vec3 g_LightPos(1.5f, 0.5f, 0.0f);

namespace byhj
{

	void OGLRender::v_init()
	{
		m_cube.init();
	}

	void OGLRender::v_update()
	{
		m_cube.update();
	}

	void OGLRender::v_render()
	{
		static const float bgColor[4] = {0.2f, 0.4f, 0.5f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, bgColor);
		static const float one = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &one);

		m_cube.render();
	
	}

	void OGLRender::v_shutdown()
	{

	}


}