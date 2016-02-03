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
		Root() = default;
		virtual ~Root() = default;

		static std::shared_ptr<Root> getInstance();

	
		void setApp(App *app);

		void beginScene();
		void endScene();

		void shutdown();

	private:
		App *m_pApp = nullptr;

		std::string m_configName;

	};
}
#endif