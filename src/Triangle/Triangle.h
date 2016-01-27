#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "ogl/oglShader.h"

namespace byhj
{
	class Triangle 
	{
	public:

	Triangle()  = default;
	~Triangle() = default;
   
	GLfloat *getVertexData();
	GLuint  *getIndexData();
	GLsizei getVertexSize();
	GLsizei getIndexSize();

	void init();
	void update();
	void render();
	void shutdown();

	GLuint getVBO();
	GLuint getVAO();
	GLuint getIBO();
	GLuint getProgram();

	private:

		void init_buffer();
		void init_vertexArray();
		void init_shader();

		GLuint m_program;
		GLuint m_vao;
		GLuint m_ibo;
		GLuint m_vbo;

		std::vector<GLfloat> m_VertexData;
		std::vector<GLuint>  m_IndexData;
		OGLShader m_TriangleShader;
	};
}
#endif