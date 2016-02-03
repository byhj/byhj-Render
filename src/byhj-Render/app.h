#ifndef app_H
#define app_H

#include "render.h"
#include "gui.h"
#include <memory>

namespace byhj
{
	class App
	{
	public:
		App() = default;
		virtual ~App() = default;
		virtual void v_end() = 0;
		virtual void v_run() = 0;
		virtual void v_shutdown() {};

	};
}
#endif