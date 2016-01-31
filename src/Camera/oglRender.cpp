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
		m_MeshLoad = new MeshLoad;
		m_MeshLoad->Init(WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
	}

	void OGLRender::v_update(const glm::mat4 &camMat)
	{
		m_MeshLoad->Update(camMat);
	}

    void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		m_MeshLoad->Render();

    }
	void OGLRender::v_shutdown()
	{
	}

}
