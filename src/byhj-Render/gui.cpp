#include "Gui.h"

namespace byhj
{

	void Gui::v_init(int sw, int sh)
	{
		TwInit(TW_OPENGL_CORE, 0);
		TwWindowSize(sw, sh);
	}
	void Gui::v_update()
	{

	}

	void  Gui::v_render()
	{
		TwDraw();
	}

	void Gui::v_shutdown()
	{
		TwTerminate();
	}
}