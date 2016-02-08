#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/oglShader.h"
#include "FogGui.h"
namespace byhj
{
	class Cube
	{
	public:

	Cube()  = default;
	~Cube() = default;

	void init();
	void update();
	void render();
	void shutdown();

	private:

		void init_buffer();
		void init_vertexArray();
		void init_shader();
		void init_texture();

	    GLuint m_program;
	    GLuint m_vao;
	    GLuint m_ibo;
	    GLuint m_vbo;

		GLuint u_model;
		GLuint u_view;
		GLuint u_proj;
		GLuint u_boxTex;
	   std::vector<GLfloat> m_VertexData;
	   std::vector<GLuint>  m_IndexData;
	   FogGui m_fogGui;

	   OGLShader m_CubeShader ={ "CubeShader" };
	};
}
#endif