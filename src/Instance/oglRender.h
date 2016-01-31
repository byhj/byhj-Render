#ifndef OGLRender_H
#define OGLRender_H

#include "render.h"
#include "asteroid.h"
#include "Planet.h"

#include "windowInfo.h"

#include <memory>
#include <gl/glew.h>

namespace byhj
{
	class OGLRender : public Render
	{
	public:
		OGLRender();
		~OGLRender();

		void v_init();
		void v_update(const glm::mat4 &camMat);
		void v_render();
		void v_shutdown();

	private:
		Asteroid *m_Asteroid;
		Planet   *m_Planet;
	};
}
#endif