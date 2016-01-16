#ifndef app_H
#define app_H

#include "render.h"
#include "gui.h"

namespace byhj
{
	class App
	{
	public:
		App();
		App(const App &app);
		virtual ~App();

		virtual void v_run() {};
		virtual void v_end() {};

		void setRender(Render *pRender)
		{
			m_pRender = pRender;
		}
		void setGui(Gui *pGui)
		{
			m_pGui = pGui;
		}
	protected:
		Render *m_pRender = nullptr;
		Gui    *m_pGui = nullptr;
	};
}
#endif