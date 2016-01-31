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
		m_Asteroid = new Asteroid;
		m_Planet  = new Planet;

		m_Asteroid->Init(WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
		m_Planet->Init(WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
	}

	void OGLRender::v_update(const glm::mat4 &camMat)
	{
		m_Asteroid->Update(camMat);
		m_Planet->Update(camMat);
	}

    void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		m_Asteroid->Render();
		m_Planet->Render();

    }
	void OGLRender::v_shutdown()
	{
	}

}
