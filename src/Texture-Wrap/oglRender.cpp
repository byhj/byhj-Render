#include "oglRender.h"

namespace byhj
{
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

	}

	void OGLRender::v_render()
	{
		GLfloat bgColor[] ={ 0.0f, 0.0f, 0.0f, 0.0f };
		glClearBufferfv(GL_COLOR, 0, bgColor);
		
		m_plane.render();
	}

	void OGLRender::v_shutdown()
	{
		m_plane.shutdown();
	}
}