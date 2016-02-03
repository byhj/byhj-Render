#include "root.h"

namespace byhj
{

	void Root::setApp(App *app)
	{
		m_pApp = app;
	}


	void Root::beginScene()
	{
		m_pApp->v_run();
	}

	void Root::endScene()
	{
		Root::m_pApp->v_end();
	}


	std::shared_ptr<Root> Root::getInstance()
	{
		static std::shared_ptr<Root> pInstance = std::make_shared<Root>();
		return pInstance;
	}

	void Root::shutdown()
	{
		m_pApp->v_shutdown();
		m_pApp = nullptr;
	}
}