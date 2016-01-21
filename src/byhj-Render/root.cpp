#include "root.h"

namespace byhj
{
	Root::Root()
	{
	}

	Root::~Root()
	{
	}

	void Root::setApp(App *pApp)
	{
		m_pApp = pApp;
	}


	void Root::beginScene()
	{
		m_pApp->v_run();
	}
	void Root::endScene()
	{
		Root::m_pApp->v_end();
	}


	 Root* Root::getInstance()
	{
		static Root * pInstance = new Root();
		return pInstance;
	}
}