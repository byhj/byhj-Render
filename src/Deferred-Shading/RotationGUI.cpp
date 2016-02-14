#include "RotationGui.h"

namespace byhj
{

	void RotationGui::v_init(int sw, int sh)
	{
		TwInit(TW_OPENGL_CORE, NULL);
		TwWindowSize(sw, sh);
		TwBar *pBar = TwNewBar("RotationBar");
		TwDefine(" RotationBar label='RotationBar' position='1050 16' alpha=0 help='Use this bar to edit the tess.' ");
		TwAddVarRW(pBar, "Quaternion", TW_TYPE_QUAT4F, &m_Orientation, "showval=true open=true ");

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