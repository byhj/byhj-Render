 #include "oglRender.h"
#include <memory>


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
       m_triangle.init();
	}

	void OGLRender::v_update()
	{
		m_triangle.update();
	}

    void OGLRender::v_render()
    {
		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, &black[0]);

		m_triangle.render();

    }
	void OGLRender::v_shutdown()
	{
		m_triangle.shutdown();
	}
}
