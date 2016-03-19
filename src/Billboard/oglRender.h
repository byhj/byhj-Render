#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "plane.h"
#include "grass.h"
#include "ogl/oglEulerCamera.h"

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
		Plane m_plane;
		Grass m_grass;
		OGLMVPMatrix m_matrix;
	};
}
#endif