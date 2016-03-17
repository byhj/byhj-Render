#ifndef Grass_H
#define Grass_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

#include <glfw/glfw3.h>

namespace byhj
{

	class Grass
	{
	public:
		Grass(): program(0), vao(0), vbo(0), GrassShader("Grass Shader") {}
		~Grass() {}

	public:
		void Init();
		void Render(glm::vec3 camPos, const  OGLMVPMatrix &matrix);
		void Shutdown();

	private:
		void init_shader();
		void init_buffer();
		void init_vertexArray();
		void init_texture();

		GLuint vao, vbo, program;
		GLuint texture, tex_loc;
		GLuint model_loc, view_loc, proj_loc;
		OGLShader GrassShader;			
		std::vector<glm::vec3> vecPos;
	};

}

#endif