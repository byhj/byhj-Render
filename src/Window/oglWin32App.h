#ifndef OGLRender_H
#define OGLRender_H

#include "d3d/d3dApp.h"
#include <memory>
#include <gl/glew.h>

namespace byhj
{
	class OGLWIN32Render : public D3DApp
	{
	public:
		OGLWIN32Render();
		~OGLWIN32Render();

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();
		void v_setOGL();

	private:
		HGLRC hrc; // Rendering context
		HDC hdc; // Device context
		HWND hwnd; // Window identifier
	};
}
#endif
