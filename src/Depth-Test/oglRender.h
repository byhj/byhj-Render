#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "cube.h"
#include <memory>
#include <gl/glew.h>

namespace byhj
{
	class OGLRender : public byhj::OGLApp
	{
	public:
		OGLRender();
		~OGLRender();

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();

	private:
		Cube m_cube;
	};
}
#endif