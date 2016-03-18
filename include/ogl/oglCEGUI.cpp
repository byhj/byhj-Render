#include "oglCEGUI.h"

namespace byhj {

	std::shared_ptr<OGLCEGUI> OGLCEGUI::pInstance = std::make_shared<OGLCEGUI>();
	
	std::shared_ptr<OGLCEGUI> OGLCEGUI::getInstance() 
	{
		return pInstance;

	}
	void OGLCEGUI::render()
	{
		renderer = static_cast<OpenGL3Renderer*>(System::getSingleton().getRenderer());
		static float time = glfwGetTime();

		// inject time pulses
		const float newtime = glfwGetTime();
		const float time_elapsed = newtime - time;
		System::getSingleton().injectTimePulse(time_elapsed);
		System::getSingleton().getDefaultGUIContext().injectTimePulse(time_elapsed);
		time = newtime;

		// render gui
		renderer->beginRendering();
		System::getSingleton().renderAllGUIContexts();
		renderer->endRendering();
	}

	void OGLCEGUI::shutdown()
	{
		// destroy system and renderer
		System::destroy();
		OpenGL3Renderer::destroy(*renderer);
		renderer = 0;
	}
    CEGUI::MouseButton OGLCEGUI::toCEGUIButton(int button)
    {
    	switch (button)
    	{
    	case GLFW_MOUSE_BUTTON_LEFT:
    		return CEGUI::LeftButton;
    
    	case GLFW_MOUSE_BUTTON_MIDDLE:
    		return CEGUI::MiddleButton;
    
    	case GLFW_MOUSE_BUTTON_RIGHT:
    		return CEGUI::RightButton;
    
    	default:
    		return CEGUI::MouseButtonCount;
    	}
    }
    
    CEGUI::Key::Scan OGLCEGUI::toCEGUIKey(int glfwKey)
    {
    	switch (glfwKey)
    	{
    	case GLFW_KEY_ESCAPE: return CEGUI::Key::Escape;
    	case GLFW_KEY_F1: return CEGUI::Key::F1;
    	case GLFW_KEY_F2: return CEGUI::Key::F2;
    	case GLFW_KEY_F3: return CEGUI::Key::F3;
    	case GLFW_KEY_F4: return CEGUI::Key::F4;
    	case GLFW_KEY_F5: return CEGUI::Key::F5;
    	case GLFW_KEY_F6: return CEGUI::Key::F6;
    	case GLFW_KEY_F7: return CEGUI::Key::F7;
    	case GLFW_KEY_F8: return CEGUI::Key::F8;
    	case GLFW_KEY_F9: return CEGUI::Key::F9;
    	case GLFW_KEY_F10: return CEGUI::Key::F10;
    	case GLFW_KEY_F11: return CEGUI::Key::F11;
    	case GLFW_KEY_F12: return CEGUI::Key::F12;
    	case GLFW_KEY_F13: return CEGUI::Key::F13;
    	case GLFW_KEY_F14: return CEGUI::Key::F14;
    	case GLFW_KEY_F15: return CEGUI::Key::F15;
    	case GLFW_KEY_UP: return CEGUI::Key::ArrowUp;
    	case GLFW_KEY_DOWN: return CEGUI::Key::ArrowDown;
    	case GLFW_KEY_LEFT: return CEGUI::Key::ArrowLeft;
    	case GLFW_KEY_RIGHT: return CEGUI::Key::ArrowRight;
    	case GLFW_KEY_LEFT_SHIFT: return CEGUI::Key::LeftShift;
    	case GLFW_KEY_RIGHT_SHIFT: return CEGUI::Key::RightShift;
    	case GLFW_KEY_LEFT_CONTROL: return CEGUI::Key::LeftControl;
    	case GLFW_KEY_RIGHT_CONTROL: return CEGUI::Key::RightControl;
    	case GLFW_KEY_LEFT_ALT: return CEGUI::Key::LeftAlt;
    	case GLFW_KEY_RIGHT_ALT: return CEGUI::Key::RightAlt;
    	case GLFW_KEY_TAB: return CEGUI::Key::Tab;
    	case GLFW_KEY_ENTER: return CEGUI::Key::Return;
    	case GLFW_KEY_BACKSPACE: return CEGUI::Key::Backspace;
    	case GLFW_KEY_INSERT: return CEGUI::Key::Insert;
    	case GLFW_KEY_DELETE: return CEGUI::Key::Delete;
    	case GLFW_KEY_PAGE_UP: return CEGUI::Key::PageUp;
    	case GLFW_KEY_PAGE_DOWN: return CEGUI::Key::PageDown;
    	case GLFW_KEY_HOME: return CEGUI::Key::Home;
    	case GLFW_KEY_END: return CEGUI::Key::End;
    	case GLFW_KEY_KP_ENTER: return CEGUI::Key::NumpadEnter;
    	case GLFW_KEY_SPACE: return CEGUI::Key::Space;
    	case 'A': return CEGUI::Key::A;
    	case 'B': return CEGUI::Key::B;
    	case 'C': return CEGUI::Key::C;
    	case 'D': return CEGUI::Key::D;
    	case 'E': return CEGUI::Key::E;
    	case 'F': return CEGUI::Key::F;
    	case 'G': return CEGUI::Key::G;
    	case 'H': return CEGUI::Key::H;
    	case 'I': return CEGUI::Key::I;
    	case 'J': return CEGUI::Key::J;
    	case 'K': return CEGUI::Key::K;
    	case 'L': return CEGUI::Key::L;
    	case 'M': return CEGUI::Key::M;
    	case 'N': return CEGUI::Key::N;
    	case 'O': return CEGUI::Key::O;
    	case 'P': return CEGUI::Key::P;
    	case 'Q': return CEGUI::Key::Q;
    	case 'R': return CEGUI::Key::R;
    	case 'S': return CEGUI::Key::S;
    	case 'T': return CEGUI::Key::T;
    	case 'U': return CEGUI::Key::U;
    	case 'V': return CEGUI::Key::V;
    	case 'W': return CEGUI::Key::W;
    	case 'X': return CEGUI::Key::X;
    	case 'Y': return CEGUI::Key::Y;
    	case 'Z': return CEGUI::Key::Z;
    	default: return CEGUI::Key::Unknown;
    	}
    }
    
    void OGLCEGUI::charCallback(GLFWwindow* window, unsigned int char_pressed)
    {
    	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(char_pressed);
    }
    
    void OGLCEGUI::cursorPosCallback(GLFWwindow* window, double x, double y)
    {
    	CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(x, y);
    }
    
    void OGLCEGUI::keyCallback(GLFWwindow* window, int key, int scan, int action, int mod)
    {
    	CEGUI::Key::Scan cegui_key = pInstance->toCEGUIKey(key);
    	if (action == GLFW_PRESS)
    	{
    		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(cegui_key);
    	}
    	else
    	{
    		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(cegui_key);
    	}
    }
    
    void OGLCEGUI::mouseButtonCallback(GLFWwindow* window, int button, int state, int mod)
    {
    	if (state == GLFW_PRESS)
    	{
    		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(pInstance->toCEGUIButton(button));
    	}
    	else
    	{
    		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(pInstance->toCEGUIButton(button));
    	}
    }
    
    void OGLCEGUI::mouseWheelCallback(GLFWwindow* window, double x, double y)
    {
    	if (y < 0.f)
    		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(-1.f);
    	else
    		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(+1.f);
    }
    
    void OGLCEGUI::windowResizedCallback(GLFWwindow* window, int width, int height)
    {
    	CEGUI::System::getSingleton().notifyDisplaySizeChanged(
    		CEGUI::Sizef(static_cast<float>(width), static_cast<float>(height)));
    	glViewport(0, 0, width, height);
    }
    
    void OGLCEGUI::errorCallback(int error, const char* message)
    {
    	CEGUI::Logger::getSingleton().logEvent(message, CEGUI::Errors);
    }

	void OGLCEGUI::setupCallbacks(GLFWwindow *pWindow)
	{
		// input callbacks
		glfwSetCharCallback(pWindow, charCallback);
		glfwSetCursorPosCallback(pWindow, cursorPosCallback);
		glfwSetKeyCallback(pWindow, keyCallback);
		glfwSetMouseButtonCallback(pWindow, mouseButtonCallback);
		glfwSetScrollCallback(pWindow, mouseWheelCallback);

		// window callback
		glfwSetWindowSizeCallback(pWindow, windowResizedCallback);

		// error callback
		glfwSetErrorCallback(errorCallback);
	}


	void OGLCEGUI::init()
	{
		// create renderer and enable extra states
		OpenGL3Renderer& cegui_renderer = OpenGL3Renderer::create(Sizef(800.f, 600.f));
		cegui_renderer.enableExtraStateSettings(true);

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
		Font::setDefaultResourceGroup("fonts");
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

		// notify system of the window size
		System::getSingleton().notifyDisplaySizeChanged(Sizef(800.f, 600.f));
		// load layout
		Window* root = WindowManager::getSingleton().loadLayoutFromFile("application_templates.layout");
		System::getSingleton().getDefaultGUIContext().setRootWindow(root);
	}


}