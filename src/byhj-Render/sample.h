#ifndef sample_H
#define sample_H

#include "root.h"
#include "ogl/oglApp.h"

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
		App  *m_pApp = nullptr;
	};
}
#endif