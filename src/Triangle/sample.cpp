#include "sample.h"


namespace byhj
{
	Sample::Sample()
	{


		m_pRender = std::make_shared<OGLRender>();
		m_pGui = std::make_shared<TriangleGui>();
		m_pApp = new OGLApp();

	    m_pApp->setRender(m_pRender);
	    m_pApp->setGui(m_pGui);

		Root::getInstance()->setApp(m_pApp);
	}

	Sample::~Sample()
	{
		delete m_pApp;
	}
    
	void Sample::run()
	{
		Root::getInstance()->beginScene();

		Root::getInstance()->endScene();
	}

}