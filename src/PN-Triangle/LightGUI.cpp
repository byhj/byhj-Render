#include "LightGui.h"
#include "windowInfo.h"

namespace byhj
{


	void LightGui::v_init()
	{
		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();

		TwInit(TW_OPENGL_CORE, 0);
		TwWindowSize(sw, sh);

		TwBar *pBar = TwNewBar("LightBar");
		TwDefine(" LightBar label='LightBar' position='1300 16' alpha=0 help='Use this bar to edit the tess.' ");

		TwAddVarRW(pBar, "Gamma", TW_TYPE_BOOL32, &m_Gamma, "");

		TwAddVarRW(pBar, "Light1", TW_TYPE_COLOR3F, &m_LightColors[0], " group = 'Light' ");
		TwAddVarRW(pBar, "Light2", TW_TYPE_COLOR3F, &m_LightColors[1], " group = 'Light' ");
		TwAddVarRW(pBar, "Light3", TW_TYPE_COLOR3F, &m_LightColors[2], " group = 'Light' ");
		TwAddVarRW(pBar, "Light4", TW_TYPE_COLOR3F, &m_LightColors[3], " group = 'Light' ");

		TwEnumVal LightType[2] ={ {LIGHT_PHONG, "Phong"}, {LIGHT_BLINNE, "Blinn"} };
		TwType lightType = TwDefineEnum("LightType", LightType, 2);
		TwAddVarRW(pBar, "LightModel", lightType, &m_LightModel, " keyIncr='<' keyDecr='>' help='Change Light Model.' ");

	}

	void LightGui::v_update()
	{

	}
	void LightGui::v_render()
	{
		TwDraw();
	}

	void LightGui::v_shutdown()
	{
		TwTerminate();
	}
	bool LightGui::getGamma()
	{
		return m_Gamma;
	}

	LightModel LightGui::getLightModel()
	{
		return m_LightModel;
	}
	glm::vec3 LightGui::getLight(int index)
	{
		return m_LightColors[index];
	}
}