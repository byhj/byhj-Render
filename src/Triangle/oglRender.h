#ifndef OGLRender_H
#define OGLRender_H

#include "render.h"
#include "Triangle.h"
#include "oglRender.h"

#include <gl/glew.h>

namespace byhj
{
	class OGLRender : public Render
	{
	public:
		OGLRender();
		~OGLRender();

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();

	private:
		void init_buffer();
		void init_shader();
		void init_texture();

		GLuint m_program;
		GLuint m_vao; m_vbo;
		Triangle m_triangle;
		OGLShader m_TriangleShader;
	};
}
#endif