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
		OGLRender()  = default;
		~OGLRender() = default;

		void v_init()     override;
		void v_update()   override;
		void v_render()   override;
		void v_shutdown() override;

	private:
		Cube m_cube;
	};
}
#endif