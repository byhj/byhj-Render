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

		App * init();

		void beginRender();
		void endRender();
		void shutdown();

	private:
		App    *m_pApp;
		Render *m_pActiveRender;

		std::string m_configName;

	};
}
#endif