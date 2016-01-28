#ifndef d3dRender_H
#define d3dRender_H

#include "render.h"

namespace byhj
{
	class D3DRender : public Render
	{
	public:
		D3DRender();
		~D3DRender();

		virtual void v_init()     {}
		virtual void v_update()   {}
		virtual void v_render()   {}
		virtual void v_shutdown() {}
	private:


	};
}
#endif