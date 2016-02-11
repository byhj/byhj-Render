#include "oglApp.h"

namespace byhj
{

	OGLApp::OGLApp()
	{
	}

	OGLApp::~OGLApp()
	{

	}

OGLApp * OGLApp::app = nullptr;

	////////////////////////////////////////////////////////////////////////////////////////

	void OGLApp::glfw_key(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

#ifdef USE_ANT
		TwEventKeyGLFW(key, action);
#endif
		app->v_keyCallback(window, key, scancode, action, mode);
	}

	////////////////////////////////////////////////////////////////////////////////////////

	void OGLApp::glfw_mouse(GLFWwindow* window, double xpos, double ypos)
	{
#ifdef USE_ANT
		TwEventMousePosGLFW(xpos, ypos);
#endif
		app->v_mouseCallback(window, xpos, ypos);
	}

	////////////////////////////////////////////////////////////////////////////////////////

	void OGLApp::glfw_scroll(GLFWwindow* window, double xoffset, double yoffset)
	{
#ifdef USE_ANT
		TwEventMouseWheelGLFW(xoffset);
#endif
		app->v_scrollCallback(window, xoffset, yoffset);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	void OGLApp::glfw_mouseButton(GLFWwindow *window, int x, int y, int z)
	{
		TwEventMouseButtonGLFW(x, y);
	}
	void OGLApp::glfw_char(GLFWwindow *window, unsigned int x)
	{
		TwEventCharGLFW(x, GLFW_PRESS);
	}

	////////////////////////////////////////////////////////////////////////////////////////

void OGLApp::v_run()
{	
	app = this;
	std::cout << "Starting GLFW context" << std::endl;
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}
	sw = WindowInfo::getInstance()->getWidth();
	sh = WindowInfo::getInstance()->getHeight();

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	pWindow = glfwCreateWindow(sw, sh, WindowInfo::getInstance()->getTitle().c_str(), nullptr, nullptr);
	glfwSetWindowPos(pWindow, WindowInfo::getInstance()->getPosX() - 100, WindowInfo::getInstance()->getPosY() - 100);
	glfwMakeContextCurrent(pWindow);

	glfwSetCursorPosCallback(pWindow, glfw_mouse);          // - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetScrollCallback(pWindow, glfw_scroll);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetKeyCallback(pWindow, glfw_key);                         // - Directly redirect GLFW key events to AntTweakBar
#ifdef USE_ANT
	glfwSetMouseButtonCallback(pWindow, glfw_mouseButton); // - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetCharCallback(pWindow, glfw_char);                      // - Directly redirect GLFW char events to AntTweakBar
#endif
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// GLFW Options
	//	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// GLFW Options

	//	glfwSetInputMode(Triangle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (pWindow == NULL)
	{
		std::cerr << "Failed to create GLFW pWindow" << std::endl;
		glfwTerminate();
		return;
	}
	glewExperimental = GL_TRUE;

	//Check the GLSL and OpenGL status 
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return;
	}
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	m_GLRenderer = (const char *)renderer;
	m_GLVersion  = (const char *)version;
	m_GLSLVersion = (const char *)glslVersion;

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "GL Vendor    :" << vendor << std::endl;
	std::cout << "GL Renderer  : " << renderer << std::endl;
	std::cout << "GL Version (std::string)  : " << version << std::endl;
	std::cout << "GL Version (integer) : " << major << "." << minor << std::endl;
	std::cout << "GLSL Version : " << glslVersion << std::endl;
	std::cout << "--------------------------------------------------------------------------------"
		<< std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions

	glViewport(0, 0, sw, sh);
	m_pFont.init(sw, sh);

	v_init();
	while (!glfwWindowShouldClose(pWindow))
	{
		glfwPollEvents();
		v_movement(pWindow);

		countFps();

		static GLfloat lastFrame = static_cast<float>(glfwGetTime());
		GLfloat currentFrame = static_cast<float>(glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		v_update();
		v_render();

		 m_pFont.render("Graphics card: " + m_GLRenderer, 10, sh - 30);
		 m_pFont.render("GL Version: " + m_GLVersion, 10, sh - 60);
		 m_pFont.render("GLSL Version: " + m_GLSLVersion, 10, sh - 90);
		 m_pFont.render("FPS: " + std::to_string(m_fps), 10, 30);

		glfwSwapBuffers(pWindow);
	}

	v_shutdown();

	glfwTerminate();
}



void OGLApp::countFps()
{
	static float currTime = glfwGetTime();
	static float lastTime = glfwGetTime();
	static float Time = 0.0f;

	currTime = glfwGetTime();
	Time += currTime - lastTime;

	static int cnt = 0;
	if (Time >= 1.0f)
	{
		m_fps = cnt;
		cnt = 0;
		Time = 0.0f;
	}
	++cnt;
	lastTime = currTime;
}

}