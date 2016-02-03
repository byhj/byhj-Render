
#ifndef root_H
#define root_H

#include "render.h"
#include "app.h"
#include <string>
#include <memory>

namespace byhj
{
	class Root
	{
	public:
		Root() ;
		~Root();

		static std::shared_ptr<Root> getInstance();

	
		void setApp(App *app);
		void setApp(std::shared_ptr<App> app);

		void beginScene();
		void endScene();

		void shutdown();

	private:
		std::shared_ptr<App> m_pApp;

		std::string m_configName;

	};
}
#endif