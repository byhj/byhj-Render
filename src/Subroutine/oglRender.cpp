#include "oglRender.h"

namespace byhj {
	void OGLRender::v_init()
	{
		m_plane.init();
	}
	void OGLRender::v_update()
	{

	}

	void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);

		m_plane.render();
	}

	void OGLRender::v_shutdown()
	{

	}
}