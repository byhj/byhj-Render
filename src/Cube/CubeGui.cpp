#include "CubeGui.h"

namespace byhj
{
	void CubeGui::v_init(int sw, int sh)
	{
	    TwInit(TW_OPENGL_CORE, NULL);
		TwWindowSize(sw, sh);

		m_pBar = TwNewBar("CubeGui");
		TwDefine(" CubeGui label='CubeGui' position='10 10' alpha=30 help='Use this bar to edit the tess.' ");
		//TwAddVarRW(m_pBar, "speed",);
	
	}
}