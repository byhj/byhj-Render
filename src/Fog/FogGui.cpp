#include "FogGui.h"
#include "windowInfo.h"

namespace byhj
{

	void FogGui::v_init()
	{
		TwInit(TW_OPENGL_CORE, 0);
		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();

		TwWindowSize(sw, sh);
		std::string posStr = "position= '" + std::to_string(sw - 250) + " " + std::to_string(10) + "'";
		std::string defStr = " FogBar label='FogBar' " + posStr + " alpha=0 help='Use this bar to edit the fog model.' ";
		TwBar *pBar = TwNewBar("FogBar");
		TwDefine(defStr.c_str());

		TwEnumVal FogType[3] ={ { FOG_LINEAR, "Linear" }, { FOG_EXP, "exp"}, { FOG_EXP2, "exp2" }, };
		TwType fogType = TwDefineEnum("FogType", FogType, 3);
		TwAddVarRW(pBar, "FogModel", fogType, &m_fogModel, " keyIncr='<' keyDecr='>' help='Change Light Model.' ");

	}

	void FogGui::v_update()
	{
		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();
		TwWindowSize(sw, sh);
		std::string posStr = "position= '" + std::to_string(sw - 250) + " " + std::to_string(10) + "'";
		std::string defStr = " FogBar label='FogBar' " + posStr + " alpha=0 help='Use this bar to edit the fog model.' ";
		TwDefine(defStr.c_str());
	}

	void FogGui::v_render()
	{
		TwDraw();
	}

	void FogGui::v_shutdown()
	{
		TwTerminate();
	}


	float FogGui::getFogDensity()
	{
		return m_fogDensity;
	}

	float  FogGui::getFogStart()
	{
		return m_fogStart;
	}
	float FogGui::getFogEnd()
	{
		return m_fogEnd;
	}
	int FogGui::getBaseModel()
	{
		return m_baseModel;
	}
	int FogGui::getFogModel()
	{
		return m_fogModel;
	}

	float* FogGui::getFogColor()
	{
		return &m_fogColor[0];
	}
}