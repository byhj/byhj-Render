#ifndef app_H
#define app_H

#include "render.h"

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
	protected:
		Render *m_pRender = nullptr;
	};
}
#endif