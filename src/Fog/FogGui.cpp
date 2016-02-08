#include "FogGui.h"

namespace byhj
{

	void FogGui::v_init(int sw, int sh)
	{
		TwInit(TW_OPENGL_CORE, 0);
		TwWindowSize(sw, sh);

		TwBar *pBar = TwNewBar("FogBar");
		TwDefine(" FogBar label='FogBar' position='1300 16' alpha=0 help='Use this bar to edit the fog model.' ");

		TwEnumVal FogType[3] ={ { FOG_LINEAR, "Linear" }, { FOG_EXP, "exp"}, { FOG_EXP2, "exp2" }, };
		TwType fogType = TwDefineEnum("FogType", FogType, 3);
		TwAddVarRW(pBar, "FogModel", fogType, &m_fogModel, " keyIncr='<' keyDecr='>' help='Change Light Model.' ");

	}

	void FogGui::v_update()
	{

	}
	void FogGui::v_render()
	{
		TwDraw();
	}

	void FogGui::v_shutdown()
	{
		TwTerminate();
	}

}