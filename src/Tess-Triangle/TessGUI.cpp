#include "TessGui.h"

namespace byhj
{
	void TessGui::v_init(int sw, int sh)
	{
		TwInit(TW_OPENGL_CORE, 0);
		TwWindowSize(sw, sh);

		TwBar *pBar = TwNewBar("TessGui");
		TwDefine(" TessGui label='TessGui' position='1300 16' alpha=0 help='Use this bar to edit the tess.' ");
		
		TwAddVarRW(pBar, "wire", TW_TYPE_BOOL32, &m_WireFrame,
				" label='Wireframe mode' key=w help='Toggle wireframe display mode.' ");

		TwAddVarRW(pBar, "LightDir", TW_TYPE_DIR3F, &m_LightDir, 
			       " label='Light direction' opened=true help='Change the light direction.' ");
		TwAddVarRW(pBar, "AmbientMat", TW_TYPE_COLOR3F, &m_AmbientMat, " group = 'Material' ");
		TwAddVarRW(pBar, "DiffuseMat", TW_TYPE_COLOR3F, &m_DiffuseMat, " group = 'Material' ");
		TwAddVarRW(pBar, "TessInner",  TW_TYPE_UINT32, &m_TessInner, " group = 'Tess' ");
		TwAddVarRW(pBar, "TessOuter", TW_TYPE_UINT32, &m_TessOuter, " group = 'Tess' ");
	}

	void TessGui::v_update()
	{

	}

	void TessGui::v_render()
	{
		TwDraw();
	}

	void TessGui::v_shutdown()
	{
		TwTerminate();
	}

	bool TessGui::getWireFrame() const
	{
		return m_WireFrame;
	}

	GLuint TessGui::getTessInnder() const
	{
		return m_TessInner;
	}
	GLuint TessGui::getTessOuter() const
	{
		return m_TessOuter;
	}
	glm::vec3 TessGui::getLightDir() const
	{
		return m_LightDir;
	}
	glm::vec4 TessGui::getAmbientMat() const
	{
		return m_AmbientMat;
	}
	glm::vec4 TessGui::getDiffuseMat() const
	{
		return m_DiffuseMat;
	}
}