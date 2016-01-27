#include "sample.h"


namespace byhj
{
	void Sample::init()
	{
		CubeGui     *m_pGui = new CubeGui;
		OGLApp      *m_pApp = new OGLApp;
		OGLRender   *m_pRender = new OGLRender;

		m_pApp->setRender(m_pRender);
		m_pApp->setGui(m_pGui);
		
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
		delete m_pGui;
		delete m_pRender;

	}

}