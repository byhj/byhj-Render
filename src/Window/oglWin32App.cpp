#include "oglWin32App.h"
#include <memory>
#include <assert.h>
#include <gl/glew.h>
#include <GL/wglew.h>

namespace byhj
{

	OGLWIN32Render::OGLWIN32Render()
	{
	}

	OGLWIN32Render::~OGLWIN32Render()
	{

	}

	void OGLWIN32Render::v_init_ogl()
	{
		m_context.createContext(getHwnd());
	}
	void OGLWIN32Render::v_init()
	{

	}

	void OGLWIN32Render::v_update()
	{

	}

	void OGLWIN32Render::v_render()
	{
		static const GLfloat black[] ={ 1.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, &black[0]);

		m_context.render();


	}
	void OGLWIN32Render::v_shutdown()
	{

	}
}
