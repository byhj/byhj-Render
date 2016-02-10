#include "oglRender.h"

namespace byhj {
	OGLRender::OGLRender()
	{

	}

	OGLRender::~OGLRender()
	{

	}

	void OGLRender::v_init()
	{
		m_scene.init();
	}

	void OGLRender::v_update()
	{

	}

	void OGLRender::v_render()
	{
		GLfloat bgColor[] ={ 0.0f, 0.0f, 0.0f, 0.0f };
		glClearBufferfv(GL_COLOR, 0, bgColor);

		m_scene.render();
	}

	void OGLRender::v_shutdown()
	{

	}
}