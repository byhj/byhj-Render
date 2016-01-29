#include "sample.h"


namespace byhj
{
	void Sample::init()
	{
		OGLApp      *m_pApp = new OGLApp;
		OGLRender   *m_pRender = new OGLRender;

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