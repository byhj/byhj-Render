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
		m_plane.init();
	}

	void OGLRender::v_update()
	{
		m_plane.update();
	}

	void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, &black[0]);
		static const GLfloat one[] ={1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		//We enable the blend func, so the texture black area can blend with backgroud
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_plane.render();
	}

	void OGLRender::v_shutdown()
	{
		m_plane.shutdown();
	}

}