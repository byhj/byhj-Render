#ifndef sample_H
#define sample_H

#include "root.h"
#include "ogl/oglApp.h"
#include "ogl/oglRender.h"
#include "triangleGui.h"

#include <memory>

namespace byhj
{
	class Sample
	{

	public:
		Sample() = default;
		~Sample() = default;
	
		void init();
		void run();
		void end();

	private:					
		TriangleGui *m_pGui 	= nullptr;
		OGLApp      *m_pApp 	= nullptr;
		OGLRender    *m_pRender = nullptr;
	};
}
#endif