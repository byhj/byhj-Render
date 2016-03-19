#ifndef TessTriangle_H
#define TessTriangle_H


#include "ogl/oglShader.h"
#include <glfw/glfw3.h>
#include "TessGui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace byhj
{


	class TessTriangle
	{
	public:
		TessTriangle()	 = default;
		~TessTriangle()	 = default;

		void init();
		void render();
		void update();
		void shutdown();

	private:
		void init_buffer();
		void init_vertexArray();
		void init_shader();

		struct UniformLoc
		{
			GLuint model;
			GLuint view;
			GLuint proj;
			GLuint lightDir;
			GLuint diffuse;
			GLuint ambient;
			GLuint tessInner;
			GLuint tessOuter;
		}uniform_loc;

		OGLShader TessShader = {"TessTriangle Shader"};

		GLuint vao, vbo, ibo;
		GLuint program;
		TessGui m_TessGUI;
};

}
#endif