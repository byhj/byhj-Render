#ifndef OGLAPP_H
#define OGLAPP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <memory>

#include "app.h"
#include "oglRender.h"
#include "oglFont.h"
#include "windowInfo.h"

#define USE_ANT

namespace byhj 
{

class OGLApp : public App
{
    private:

	public:
		OGLApp() = default;
		~OGLApp() = default;

	public:

		void setRender(Render *pRender);
		void setFont(OGLFont *pFont);

protected:
	Render*  m_pRender = nullptr;
	OGLFont *m_pFont = new OGLFont;
	static  OGLApp* app;

		void v_run() override;
		void v_end() override;

		virtual void v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
		virtual void v_Movement(GLFWwindow *window) {}
		virtual void v_MouseCallback(GLFWwindow* window, double xpos, double ypos) {}
		virtual void v_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}

		static void glfw_key(GLFWwindow * window, int key, int scancode, int action, int mode);
		static void glfw_mouse(GLFWwindow* window, double xpos, double ypos);
		static void glfw_scroll(GLFWwindow* window, double xoffset, double yoffset);
		static void glfw_mouseButton(GLFWwindow *window, int x, int y, int z);
		static void glfw_char(GLFWwindow *window, unsigned int x);

		void countFps();
	protected:
		std::string m_GLRenderer;
		std::string m_GLVersion;
		std::string m_GLSLVersion;
		GLuint m_fps = 0;

		float GetAspect() const ;
	};  //class

}


#endif  //