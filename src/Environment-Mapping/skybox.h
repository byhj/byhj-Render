#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/oglShader.h"

namespace byhj
{
	class Skybox
	{
	public:

	Skybox()  = default;
	~Skybox() = default;

	void init();
	void update();
	void render();
	void shutdown();

	private:

		void init_buffer();
		void init_vertexArray();
		void init_shader();
		void init_texture();

		struct UniformLoc {
			GLuint model;
			GLuint view;
			GLuint proj;
			GLuint skytex;
		}uniform_loc;

		GLuint m_program;
		GLuint m_vao;
		GLuint m_ibo;
		GLuint m_vbo;
		GLuint m_skyboxTex;
	   std::vector<GLfloat> m_VertexData;
	   std::vector<GLuint>  m_IndexData;

	   OGLShader m_SkyboxShader = "SkyboxShader";
	};
}

#endif