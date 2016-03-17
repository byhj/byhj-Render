#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "plane.h"
#include "grass.h"
#include "ogl/oglCamera.h"

#include <memory>
#include <gl/glew.h>

namespace byhj
{
	class OGLRender : public byhj::OGLApp
	{
	public:
		OGLRender();
		~OGLRender();

		void v_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) override;
		void v_movement(GLFWwindow *window) override;
		void v_mouseCallback(GLFWwindow* window, double xpos, double ypos) override;
		void v_scrollCallback(GLFWwindow* window, double xoffset, double yoffset) override;

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();

	private:
		Plane m_plane;
		Grass m_grass;
		OGLCamera m_camera;
		OGLMVPMatrix m_matrix;
	};
}
#endif