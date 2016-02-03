#include "root.h"

namespace byhj
{
	Root::Root()
	{

	}
	Root::~Root()
	{

	}
	void Root::setApp(App *app)
	{
		//m_pApp = app;
	}
	void Root::setApp(std::shared_ptr<App> app)
	{
		m_pApp = app;
	}

	void Root::beginScene()
	{
		m_pApp->v_run();
	}

	void Root::endScene()
	{
		Root::m_pApp->v_shutdown();
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