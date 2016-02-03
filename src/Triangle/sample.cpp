#include "sample.h"


namespace byhj
{
	Sample::Sample()
	{
		m_pApp = std::make_shared<OGLApp>();
		m_pRender = std::make_shared<OGLRender>();
	}

	Sample::~Sample()
	{
		m_pApp = nullptr;
		m_pRender = nullptr;
		std::cout << "APP use count:" << m_pApp.use_count() << std::endl;
		std::cout << "Render use count:" << m_pRender.use_count() << std::endl;
	}

	void Sample::init()
	{
		m_pApp->v_init();

		std::cout << "APP use count:" << m_pApp.use_count() << std::endl;
		std::cout << "Render use count:" << m_pRender.use_count() << std::endl;
	}

	void Sample::run()
	{
		m_pApp->v_run();
	}

	void Sample::end()
	{
		m_pApp->v_shutdown();

		std::cout << "APP use count:" << m_pApp.use_count() << std::endl;
		std::cout << "Render use count:" << m_pRender.use_count() << std::endl;
	}

}