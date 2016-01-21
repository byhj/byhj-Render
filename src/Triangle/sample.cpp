#include "sample.h"


namespace byhj
{
	Sample::Sample()
	{
		m_pRoot = Root::getInstance();

		m_pRender = new OGLRender();
		m_pGui = new TriangleGui();
		m_pApp = new OGLApp();

	    m_pApp->setRender(m_pRender);
	    m_pApp->setGui(m_pGui);

		m_pRoot->setApp(m_pApp);
	}

	Sample::~Sample()
	{

	}
    
	void Sample::run()
	{
		m_pRoot->beginScene();

		m_pRoot->endScene();
	}

}