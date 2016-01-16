#ifndef __GUI_H_
#define __GUI_H_

#include "AntTweakBar.h"
#include <d3d11.h>

namespace byhj
{
	class Gui
	{
	public:
		Gui() = default;
		virtual ~Gui() = default;

		virtual void v_init(int sw, int sh);
		virtual void v_update();
		virtual void v_render();
		virtual void v_shutdown();

	private:

	};
}
#endif
