#ifndef root_H
#define root_H

#include "render.h"
#include "app.h"
#include <string>

namespace byhj
{
	class Root
	{
	public:
		Root();
		virtual ~Root();

		static Root* getInstance();

		
		//void addRender(Render *render);
		//Render* getRender(std::string named);
		//void    setRender(Render *render);
		void setApp(App *pApp)
		{
			m_pApp = pApp;
		}
		

		void beginScene()
		{
			m_pApp->v_run();
		}
		void endScene()
		{
			m_pApp->v_end();
		}

	private:
		App    *m_pApp = nullptr;
		Render *m_pActiveRender = nullptr;

		std::string m_configName;

	};
}
#endif