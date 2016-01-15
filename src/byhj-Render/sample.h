#ifndef sample_H
#define sample_H

#include "root.h"
#include "app.h"
#include "render.h"

namespace byhj
{
	class Sample
	{
	public:
	    Sample();
		~Sample();

		void run();

	private:
		void init();
		void update();
		void render();
		void shutdown();

		App *m_App;
		Render *m_Render;
	};
}
#endif