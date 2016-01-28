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
		m_TessTriangle = new TessTriangle;
		m_TessTriangle->init(WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
	}

	void OGLRender::v_update()
	{
		m_TessTriangle->update();
	}

    void OGLRender::v_render()
	{
		glEnable(GL_DEPTH_TEST);

		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		m_TessTriangle->render();

    }
	void OGLRender::v_shutdown()
	{
		m_TessTriangle->shutdown();

		delete 	m_TessTriangle;
	}

}
