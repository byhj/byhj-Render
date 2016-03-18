#include "root.h"

namespace byhj
{
	Root::Root()
	{

	}

	Root::~Root()
	{

	}

	void Root::setRender(std::shared_ptr<App> app)
	{
		m_pApp = app;
	}

	void Root::BeginScene()
	{
		Root::m_pApp->v_run();
	}

	void Root::EndScene()
	{
		Root::m_pApp->v_shutdown();
	}


	std::shared_ptr<Root> Root::getInstance()
	{
		static std::shared_ptr<Root> pInstance = std::make_shared<Root>();
		return pInstance;
	}
}