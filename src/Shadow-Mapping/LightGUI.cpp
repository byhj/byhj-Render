#include "LightGui.h"
#include "windowInfo.h"

namespace byhj
{


	void LightGui::v_init()
	{
		TwInit(TW_OPENGL_CORE, 0);
		float sw = WindowInfo::getInstance()->getWidth();
		float sh = WindowInfo::getInstance()->getHeight();
		TwWindowSize(sw, sh);

		TwBar *pBar = TwNewBar("LightBar");
		TwDefine(" LightBar label='LightBar' position='800 16' alpha=0 help='Use this bar to edit the tess.' ");

		TwAddVarRW(pBar, "LightDir", TW_TYPE_DIR3F, &m_lightDir,
			" label='Light direction' opened=true help='Change the light direction.' ");

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
	
	glm::vec3 LightGui::getLightDir()
	{
		return m_lightDir;
	}
}