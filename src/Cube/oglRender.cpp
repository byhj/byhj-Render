 #include "oglRender.h"
#include <memory>


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
		glEnable(GL_DEPTH_TEST);

		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		m_cube.render();

    }
	void OGLRender::v_shutdown()
	{
		m_cube.shutdown();
	}

}
