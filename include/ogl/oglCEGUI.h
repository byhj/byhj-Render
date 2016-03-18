
#ifndef _OGLCEGUI_H
#define _OGLCEGUI_H

/**************************************************************************
* The following libs (and corresponding headers) are needed to compile and to link:
* CEGUIBase
* CEGUIOpenGLRenderer
* CEGUICoreWindowRendererSet
* default CEGUI xml parser (and dependencies)
* GLFW3
* OpengGL
* glm headers (as part of CEGUIBase)
***************************************************************************/

#include <iostream>
#include <memory>


#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <GLFW/glfw3.h>

#pragma  comment(lib, "CEGUIBase-0_d.lib")
#pragma  comment(lib, "CEGUIOpenGLRenderer-0_d.lib")
#pragma  comment(lib, "CEGUICoreWindowRendererSet_d.lib")


using namespace CEGUI;

namespace byhj {

	class OGLCEGUI {
	public:

		void init();
		void render();
		void shutdown();
		void setupCallbacks(GLFWwindow *pWindow);

		static void charCallback(GLFWwindow* window, unsigned int char_pressed);
		static void cursorPosCallback(GLFWwindow* window, double x, double y);
		static void keyCallback(GLFWwindow* window, int key, int scan, int action, int mod);
		static void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
		static void mouseWheelCallback(GLFWwindow* window, double x, double y);
		static void windowResizedCallback(GLFWwindow* window, int width, int height);
		static void errorCallback(int error, const char* message);

		static std::shared_ptr<OGLCEGUI> getInstance();


	private:
		CEGUI::MouseButton toCEGUIButton(int button);
		CEGUI::Key::Scan toCEGUIKey(int glfwKey);

		OpenGL3Renderer* renderer;
		static std::shared_ptr<OGLCEGUI> pInstance;

	};
}


#endif