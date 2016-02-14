#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/oglShader.h"

namespace byhj
{
	class Cube
	{
	public:

		struct Vertex {
			Vertex() {}
			Vertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty)
				:pos(px, py, pz), normal(nx, ny, nz), tc(tx, ty)
			{

			}
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 tc;
			glm::vec3 tangent;
			glm::vec3 bitangent;
		};
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
		void calcTangent();

	    GLuint m_program;
	    GLuint m_vao;
	    GLuint m_ibo;
	    GLuint m_vbo;

		GLuint u_model;
		GLuint u_view;
		GLuint u_proj;
		GLuint normalTex, colorTex;
		std::vector<Vertex> m_VertexData;

	   OGLShader m_CubeShader ={ "CubeShader" };
	};
}
#endif