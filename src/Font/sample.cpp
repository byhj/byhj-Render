#include "sample.h"


namespace byhj
{
	void Sample::init()
	{
		D3DApp      *m_pApp = new D3DApp;
		D3DRender   *m_pRender = new D3DRender;

		m_pApp->setRender(m_pRender);
		Root::getInstance()->setApp(m_pApp);
	}

	void Sample::run()
	{
		Root::getInstance()->beginScene();

		Root::getInstance()->endScene();
	}

	void Sample::end()
	{
		delete m_pApp;
		delete m_pRender;

	}

}