#include "oglApp.h"

namespace byhj
{

std::shared_ptr<OGLApp> OGLApp::m_oglApp = nullptr;

void OGLApp::v_run()
{	
    m_oglApp = std::make_shared<OGLApp>(*this);

	std::cout << "Starting GLFW context" << std::endl;
	if (!glfwInit()) 
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}


#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	GLFWwindow *Triangle = glfwCreateWindow(windowInfo.Width, windowInfo.Height,
		                                    windowInfo.title.c_str(), nullptr, nullptr);
	glfwSetWindowPos(Triangle, windowInfo.posX, windowInfo.posY);
	glfwMakeContextCurrent(Triangle);

	//Key and Mouse callback function
	glfwSetKeyCallback(Triangle, glfw_key);
	glfwSetCursorPosCallback(Triangle, glfw_mouse);
	glfwSetScrollCallback(Triangle, glfw_scroll);

	// GLFW Options
//	glfwSetInputMode(Triangle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (Triangle == NULL)
	{
		std::cerr << "Failed to create GLFW Triangle" << std::endl;
		glfwTerminate();
		return ;
	}	
	glewExperimental = GL_TRUE;

	//Check the GLSL and OpenGL status 
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return ;
	}
	const GLubyte *renderer = glGetString( GL_RENDERER );  
	const GLubyte *vendor = glGetString( GL_VENDOR );  
	const GLubyte *version = glGetString( GL_VERSION );  
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );  
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major); //opengl 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	m_pRender->v_init();
	m_pGui->v_init(windowInfo.Width, windowInfo.Height);

	glViewport(0, 0, windowInfo.Width, windowInfo.Height);

	while (!glfwWindowShouldClose(Triangle)) 
	{
		glfwPollEvents();
		v_Movement(Triangle);

		m_pRender->v_update();
		m_pGui->v_update();

		m_pRender->v_render();
		m_pGui->v_render();

		glfwSwapBuffers(Triangle);
	}
	m_pGui->v_shutdown();
	m_pRender->v_shutdown();

	glfwTerminate();
}

float OGLApp::GetAspect() const
{
	return static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight);
}

int OGLApp::getSW() const 
{
	return ScreenWidth;
}

int OGLApp::getSH() const 
{
	return ScreenHeight;
}


}