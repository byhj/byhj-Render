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
		m_cube.init();
	}

	void OGLRender::v_update()
	{

	}
	void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.5f, 0.5f, 0.5f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, &black[0]);
		static const GLfloat one[] ={1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		glEnable(GL_DEPTH_TEST);

		m_cube.render();
	}

	void OGLRender::v_shutdown()
	{

	}

}