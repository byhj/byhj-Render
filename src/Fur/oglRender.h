#ifndef OGLRender_H
#define OGLRender_H

#include "render.h"
#include "utility.h"
#include "fur.h"

namespace byhj
{
	class OGLRender  : public Render {
	public:
		OGLRender();
		~OGLRender();

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();

	private:
		Fur *m_pFur = nullptr;
	};
}
#endif