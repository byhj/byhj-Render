#include "RotationGui.h"
#include "windowInfo.h"

namespace byhj
{

	void RotationGui::v_init()
	{
		TwInit(TW_OPENGL_CORE, NULL);
		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();

		TwWindowSize(sw, sh);
		TwBar *pBar = TwNewBar("RotationBar");
		TwDefine(" RotationBar label='RotationBar' position='1050 16' alpha=0 help='Use this bar to edit the tess.' ");
		TwAddVarRW(pBar, "Quaternion", TW_TYPE_QUAT4F, &m_Orientation, "showval=true open=true ");

	}
	void RotationGui::v_update()
	{

	}
	void RotationGui::v_render()
	{
		TwDraw();
	}

	void RotationGui::v_shutdown()
	{
		TwTerminate();
	}
}