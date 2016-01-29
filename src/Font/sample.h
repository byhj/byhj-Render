#ifndef sample_H
#define sample_H

#include "root.h"
#include "d3d/d3dApp.h"
#include "d3d/d3dRender.h"

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

		D3DApp       *m_pApp 	= nullptr;
		D3DRender    *m_pRender = nullptr;
	};
}
#endif