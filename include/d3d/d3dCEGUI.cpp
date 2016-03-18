#include "d3dCEGUI.h"
#include "windowInfo.h"

namespace byhj {

	std::shared_ptr<D3DCEGUI> D3DCEGUI::pInstance = std::make_shared<D3DCEGUI>();

	std::shared_ptr<D3DCEGUI> D3DCEGUI::getInstance()
	{
		return pInstance;

	}
	void D3DCEGUI::render()
	{

		// render gui
		renderer->beginRendering();
		System::getSingleton().renderAllGUIContexts();
		renderer->endRendering();
	}

	void D3DCEGUI::shutdown()
	{
		// destroy system and renderer
		System::destroy();
		Direct3D11Renderer::destroy(*renderer);
		renderer = 0;
	}


	void D3DCEGUI::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		// create renderer and enable extra states
		Direct3D11Renderer& cegui_renderer = Direct3D11Renderer::create(pD3D11Device, pD3D11DeviceContext);
		//	cegui_renderer.enableExtraStateSettings(true);

		// create CEGUI system object
		CEGUI::System::create(cegui_renderer);

		// setup resource directories
		DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());
		rp->setResourceGroupDirectory("schemes", "../../media/datafiles/schemes/");
		rp->setResourceGroupDirectory("imagesets", "../../media/datafiles/imagesets/");
		rp->setResourceGroupDirectory("fonts", "../../media/datafiles/fonts/");
		rp->setResourceGroupDirectory("layouts", "../../media/datafiles/layouts/");
		rp->setResourceGroupDirectory("looknfeels", "../../media/datafiles/looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", "../../media/datafiles/lua_scripts/");
		rp->setResourceGroupDirectory("schemas", "../../media/datafiles/xml_schemas/");

		// set default resource groups
		ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		Scheme::setDefaultResourceGroup("schemes");
		WidgetLookManager::setDefaultResourceGroup("looknfeels");
		WindowManager::setDefaultResourceGroup("layouts");
		ScriptModule::setDefaultResourceGroup("lua_scripts");

		XMLParser* parser = System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");

		// load TaharezLook scheme and DejaVuSans-10 font
		SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "schemes");
		FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

		// set default font and cursor image and tooltip type
		System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
		System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
		System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");

		/////////////////////////////////////////////////////////////
		// Add your gui initialisation code in here.
		// You should preferably use layout loading because you won't
		// have to recompile everytime you change the layout. But you
		// can also use static window creation code here, of course.
		/////////////////////////////////////////////////////////////

		//System::getSingleton().notifyDisplaySizeChanged(Sizef(800.f, 600.f));

		// load layout
		Window* root = WindowManager::getSingleton().loadLayoutFromFile("application_templates.layout");
		System::getSingleton().getDefaultGUIContext().setRootWindow(root);
		// notify system of the window size
		renderer= static_cast<Direct3D11Renderer*>(System::getSingleton().getRenderer());
	}

	void D3DCEGUI::setCallback(UINT uMsg, WPARAM wParam, LPARAM lParam) {

		switch (uMsg)
		{
		case WM_CHAR:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectChar((CEGUI::utf32)wParam);
			break;
		case WM_LBUTTONDOWN:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
			break;
		case WM_RBUTTONDOWN:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::RightButton);
			break;
		case WM_LBUTTONUP:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
			break;
		case WM_RBUTTONUP:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::RightButton);
			break;
		case WM_MOUSEMOVE:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition((float)(LOWORD(lParam)), (float)(HIWORD(lParam)));
			break;
		case WM_MOUSEWHEEL:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(static_cast<float>((short)HIWORD(wParam)) / static_cast<float>(120));
			break;
		}
	}
}