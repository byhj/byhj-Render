#include "App.h"

namespace byhj
{
	App::App()
	{

	}

	App::~App()
	{

	}
	void App::setRender(std::shared_ptr<Render> pRender)
	{
		m_pRender = pRender;
	}
	void App::setGui(std::shared_ptr<Gui> pGui)
	{
		m_pGui = pGui;
	}

}