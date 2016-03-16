#include "ceguiGLFW3.h"

static GLFWwindow* window;

void setupCallbacks()
{
	// input callbacks
	glfwSetCharCallback(window, charCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, mouseWheelCallback);

	// window callback
	glfwSetWindowSizeCallback(window, windowResizedCallback);

	// error callback
	glfwSetErrorCallback(errorCallback);
}

void initGLFW()
{
	// init everything from glfw
	if (glfwInit() != GL_TRUE)
	{
		std::cerr << "glfw could not be initialized!" << std::endl;
		exit(1);
	}

	// create glfw window with size of 800x600px
	window = glfwCreateWindow(800, 600, "CEGUI + glfw3 window", NULL, NULL);
	if (!window)
	{
		std::cerr << "Could not create glfw window!" << std::endl;
		glfwTerminate();
		exit(1);
	}

	// makes this window's gl context the current one
	glfwMakeContextCurrent(window);

	// hide native mouse cursor when it is over the window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// disable VSYNC
	glfwSwapInterval(0);

	// clear error messages
	glGetError();
}

void initCEGUI()
{
	using namespace CEGUI;

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
}

void initWindows()
{


	/////////////////////////////////////////////////////////////
	// Add your gui initialisation code in here.
	// You should preferably use layout loading because you won't
	// have to recompile everytime you change the layout. But you
	// can also use static window creation code here, of course.
	/////////////////////////////////////////////////////////////

	// load layout
	Window* root = WindowManager::getSingleton().loadLayoutFromFile("application_templates.layout");
	System::getSingleton().getDefaultGUIContext().setRootWindow(root);
}

// #ifdef _MSC_VER
// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
// #else
int main(int argc, char* argv[])
//#endif
{
	using namespace CEGUI;

	// init glfw
	initGLFW();

	// init cegui
	initCEGUI();

	// setup glfw callbacks
	setupCallbacks();

	// notify system of the window size
	System::getSingleton().notifyDisplaySizeChanged(Sizef(800.f, 600.f));
	glViewport(0, 0, 800, 600);

	// initialise windows and setup layout
	initWindows();

	// set gl clear color
	glClearColor(0, 0, 0, 255);

	float time = glfwGetTime();

	OpenGL3Renderer* renderer = static_cast<OpenGL3Renderer*>(System::getSingleton().getRenderer());

	// repeat until a quit is requested
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		// clear screen
		glClear(GL_COLOR_BUFFER_BIT);

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

		// swap buffers
		glfwSwapBuffers(window);

		// poll events
		glfwPollEvents();
	}

	// destroy system and renderer
	System::destroy();
	OpenGL3Renderer::destroy(*renderer);
	renderer = 0;

	// destroy glfw window
	glfwDestroyWindow(window);

	// cleanup glfw
	glfwTerminate();

	return 0;
}
