#include "sample.h"

namespace byhj
{
	Sample::Sample()
	{

	}

	Sample::~Sample()
	{

	}

	void Sample::init()
	{
		m_pApp = new OGLApp;
		m_pRender = new OGLRender;

		m_pApp->setRender(m_pRender);
		Root::getInstance()->setApp(m_pApp);
	}

	void Sample::update()
	{

	}
	void Sample::render()
	{
		Root::getInstance()->beginScene();

		Root::getInstance()->endScene();
	}

	void Sample::shutdown()
	{
		Root::getInstance()->shutdown();

		SAFE_DELETE(m_pRender);
		SAFE_DELETE(m_pApp);
	}
}