#include "TriangleGui.h"

namespace byhj
{
	void TriangleGui::v_init(int sw, int sh)
	{
	    TwInit(TW_OPENGL_CORE, NULL);
		TwWindowSize(sw, sh);

		m_pBar = TwNewBar("TriangleGUI");
		TwDefine(" TriangleGUI label='TriangleGUI' position='10 10' alpha=30 help='Use this bar to edit the tess.' ");
		//TwAddVarRW(m_pBar, "speed",);
	
	}
}