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
		m_triangle.init();
	}

	void OGLRender::v_update()
	{
	}

    void OGLRender::v_render()
    {
		m_triangle.render();
    }
	void OGLRender::v_shutdown()
	{
	}

}
