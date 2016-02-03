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
		std::shared_ptr<OGLApp>    m_pApp 	;
		std::shared_ptr<OGLRender> m_pRender;
	};
}
#endif