#ifndef sample_H
#define sample_H

#include "root.h"
#include "ogl/oglApp.h"
#include "ogl/oglRender.h"

#include <memory>

namespace byhj
{
	class Sample
	{

	public:
		Sample();
		~Sample();
	
		void init();
		void run();
		void end();

	private:					
		OGLRender *m_pRender;
	};
}
#endif