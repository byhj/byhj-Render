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
		App(const App &app) = default;
		virtual ~App() = default;

		virtual void v_run() {};
		virtual void v_end() {};

		void setRender(std::shared_ptr<Render> pRender);
		void setGui(std::shared_ptr<Gui> pGui);

	protected:
		std::shared_ptr<Render> m_pRender = nullptr;
		std::shared_ptr<Gui   > m_pGui = nullptr;
	};
}
#endif