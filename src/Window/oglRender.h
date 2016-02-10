#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include <memory>
#include <gl/glew.h>

namespace byhj
{
	class OGLRender : OGLApp
	{
	public:
		OGLRender();
		~OGLRender();

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();

	private:
	};
}
#endif