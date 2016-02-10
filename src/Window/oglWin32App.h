#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/win32App.h"
#include "ogl/oglContext.h"

#include <memory>
#include <gl/glew.h>

namespace byhj
{
	class OGLWIN32Render : public Win32App
	{
	public:
		OGLWIN32Render();
		~OGLWIN32Render();

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();
		void v_init_ogl();

	private:
		OGLContext m_context;
	};
}
#endif
