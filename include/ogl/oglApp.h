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
#include "ogl/oglCamera.h"
#include "windowInfo.h"
#include <memory>

#define USE_ANT

namespace byhj 
{

class OGLApp : public App
{
    private:

	public:
		OGLApp();
		~OGLApp();

	public:

	    void v_run();

		virtual void v_init()    {}
		virtual void v_render()  {}
		virtual void v_update()  {}
		virtual void v_shutdown(){}
protected:

	OGLFont   m_pFont;
	OGLCamera m_camera;
	int sw ;
	int sh ;
	static  OGLApp* app;


	void v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		m_camera.key_callback(window, key, scancode, action, mode);
	}
	void v_Movement(GLFWwindow *window)
	{
		m_camera.movement(window);
	}
	void v_MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		m_camera.mouse_callback(window, xpos, ypos);
	}
	void v_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_camera.scroll_callback(window, xoffset, yoffset);
	}

		static void glfw_key(GLFWwindow * window, int key, int scancode, int action, int mode);
		static void glfw_mouse(GLFWwindow* window, double xpos, double ypos);
		static void glfw_scroll(GLFWwindow* window, double xoffset, double yoffset);
		static void glfw_mouseButton(GLFWwindow *window, int x, int y, int z);
		static void glfw_char(GLFWwindow *window, unsigned int x);

		 void countFps();
		 float GetAspect() const;
	protected:
		std::string m_GLRenderer;
		std::string m_GLVersion;
		std::string m_GLSLVersion;
		GLuint m_fps;
		GLFWwindow *pWindow;
	
	};  //class

}


#endif  //