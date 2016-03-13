#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "ogl/oglCamera.h"

#include "cube.h"
#include "plane.h"
#include "window.h"

namespace byhj
{

class OGLRender : public OGLApp
{
public:
	OGLRender() {}
	~OGLRender() {}

	///////////////////////// Triangle and Render //////////////////////////////////
	void v_init();
	void v_update();
	void v_render();
	void v_shutdown();

	/////////////////////////////////Key and Mouse//////////////////////////////////
	void v_movement(GLFWwindow *Triangle);
	void v_keyCallback(GLFWwindow* Triangle, int key, int scancode, int action, int mode);
	void v_mouseCallback(GLFWwindow* Triangle, double xpos, double ypos);
	void v_scrollCallback(GLFWwindow* Triangle, double xoffset, double yoffset);


private:


	byhj::Cube   m_Cube;
	byhj::Plane  m_Plane;
	byhj::Window m_Window;

	OGLCamera m_Camera;
};


}

#endif