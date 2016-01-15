#ifndef OGLRender_H
#define OGLRender_H

#include "render.h"

namespace byhj
{
	class OGLRender : public Render
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