#ifndef Sample_H
#define Sample_H

#include "ogl/oglApp.h"
#include "oglRender.h"
#include "utility.h"
#include "root.h"

namespace byhj {
	class Sample {
	public:
		Sample();
		~Sample();

		void init();
		void update();
		void render();
		void shutdown();
	private:
		OGLApp *m_pApp = nullptr;
		OGLRender *m_pRender = nullptr;
	};
}
#endif