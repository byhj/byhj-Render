#ifndef OGLAPP_H
#define OGLAPP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "AntTweakBar.h"

#include <iostream>
#include <string>
#include <memory>

#include "app.h"
#include "oglRender.h"
#include "oglFTFont.h"
#include "windowInfo.h"

#include <memory>

#pragma comment(lib, "glfw3")
#pragma comment(lib, "glew32")
#pragma comment(lib, "opengl32")

#define USE_ANT
//#define USE_FONT
#define USE_CAMERA

#ifdef USE_CEGUI
#include "ogl/oglCEGUI.h"
#endif


#ifdef USE_CAMERA
#include "ogl/oglEulerCamera.h"
//#include "ogl/oglSphereCamera.h"
#endif

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

	OGLFTFont   m_pFont;
//	OGLCamera m_camera;
	int sw ;
	int sh ;
	static  OGLApp* app;


	virtual void v_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	virtual void v_movement(GLFWwindow *window);
	virtual void v_mouseCallback(GLFWwindow* window, double xpos, double ypos);
	virtual void v_scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


	static void glfw_key(GLFWwindow * window, int key, int scancode, int action, int mode);
	static void glfw_mouse(GLFWwindow* window, double xpos, double ypos);
	static void glfw_scroll(GLFWwindow* window, double xoffset, double yoffset);
	static void glfw_mouseButton(GLFWwindow *window, int x, int y, int z);
	static void glfw_char(GLFWwindow *window, unsigned int x);
	static void glfw_resize(GLFWwindow* window, int width, int height);

	void resizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	 void countFps();
	 float GetAspect() const;
  protected:
		std::string m_GLRenderer;
		std::string m_GLVersion;
		std::string m_GLSLVersion;
		GLuint m_fps = 0;
		GLFWwindow *pWindow;
	};  //class

}


#endif  //