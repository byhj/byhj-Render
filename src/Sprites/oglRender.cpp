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
		m_Point.init();
	}

	void OGLRender::v_update()
	{
		m_Point.update();
	}

    void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		m_Point.render();
    }
	void OGLRender::v_shutdown()
	{
		m_Point.shutdown();
	}

}
