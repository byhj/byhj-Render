#ifndef Window_H
#define Window_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include <glfw/glfw3.h>

namespace byhj
{

	class Window
	{
	public:
		Window(): program(0), vao(0), vbo(0), WindowShader("Window Shader") {}
		~Window() {}

	public:
		void init();
		void render(const OGLMVPMatrix &matrix, const glm::vec3 &camPos);
		void shutdown();

	private:
		void init_shader();
		void init_buffer();
		void init_vertexArray();
		void init_texture();

		GLuint vao, vbo, program;
		GLuint texture, tex_loc;
		GLuint model_loc, view_loc, proj_loc;
		OGLShader WindowShader;			
		std::vector<glm::vec3> vecPos;
	};

}

#endif