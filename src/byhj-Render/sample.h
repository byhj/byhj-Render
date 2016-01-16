#ifndef sample_H
#define sample_H

#include "root.h"
#include "ogl/oglApp.h"
#include "triangleGui.h"
#include "ogl/oglRender.h"
#include "d3d/d3dRender.h"
#include "d3d/d3dApp.h"

namespace byhj
{
	class Sample
	{

	public:
		Sample(); 
		~Sample();
	
		void run();

	private:

		Root *m_pRoot = nullptr;

		OGLRender *m_pRender = nullptr;
		OGLApp  *m_pApp = nullptr;
		TriangleGui *m_pGui = nullptr;

	};
}
#endif