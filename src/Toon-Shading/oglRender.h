#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "torus.h"

namespace byhj {
	class OGLRender : public OGLApp
	{
	public:
		OGLRender();
		~OGLRender();

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();

	private:
		Torus m_torus;
	};
}
#endif
