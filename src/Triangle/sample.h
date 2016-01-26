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
		~Sample() = default;
	
		void run();

	private:
		std::shared_ptr<TriangleGui>m_pGui = nullptr;
		std::shared_ptr<OGLRender> m_pRender = nullptr;
		std::shared_ptr<OGLApp> m_pApp = nullptr;

	};
}
#endif