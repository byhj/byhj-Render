#ifndef OGLRender_H
#define OGLRender_H

#include "render.h"
#include "Triangle.h"
#include "oglRender.h"


#include <gl/glew.h>

namespace byhj
{
	class OGLRender : public Render
	{
	public:
		OGLRender();
		virtual ~OGLRender() = default;

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();

	private:
	std::shared_ptr<Triangle> m_triangle;

	};
}
#endif