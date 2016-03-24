#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "ogl/oglEulerCamera.h"
#include "asteroid.h"
#include "Planet.h"

#include "windowInfo.h"

#include <memory>
#include <gl/glew.h>

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
		Asteroid m_Asteroid;
		Planet   m_Planet;
	};
}
#endif