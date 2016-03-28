#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "oglRTT.h"
#include "plane.h"

namespace byhj
{
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
		void init_fbo();
		GLuint fbo, colorBuffer;
		Plane m_plane;
		OGLRTT m_oglRTT;
	};
}
#endif