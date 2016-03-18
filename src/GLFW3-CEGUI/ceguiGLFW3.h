
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

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <GLFW/glfw3.h>

#pragma  comment(lib, "CEGUIBase-0_d.lib")
#pragma  comment(lib, "CEGUIOpenGLRenderer-0_d.lib")
#pragma  comment(lib, "CEGUICoreWindowRendererSet_d.lib")
#pragma  comment(lib, "opengl32")
#pragma  comment(lib, "glew32")
#pragma  comment(lib, "glfw3")

using namespace CEGUI;

CEGUI::MouseButton toCEGUIButton(int button)
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

CEGUI::Key::Scan toCEGUIKey(int glfwKey)
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

void charCallback(GLFWwindow* window, unsigned int char_pressed)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(char_pressed);
}

void cursorPosCallback(GLFWwindow* window, double x, double y)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(x, y);
}

void keyCallback(GLFWwindow* window, int key, int scan, int action, int mod)
{
	CEGUI::Key::Scan cegui_key = toCEGUIKey(key);
	if (action == GLFW_PRESS)
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(cegui_key);
	}
	else
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(cegui_key);
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod)
{
	if (state == GLFW_PRESS)
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(toCEGUIButton(button));
	}
	else
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(toCEGUIButton(button));
	}
}

void mouseWheelCallback(GLFWwindow* window, double x, double y)
{
	if (y < 0.f)
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(-1.f);
	else
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(+1.f);
}

void windowResizedCallback(GLFWwindow* window, int width, int height)
{
	CEGUI::System::getSingleton().notifyDisplaySizeChanged(
		CEGUI::Sizef(static_cast<float>(width), static_cast<float>(height)));
	glViewport(0, 0, width, height);
}

void errorCallback(int error, const char* message)
{
	CEGUI::Logger::getSingleton().logEvent(message, CEGUI::Errors);
}

