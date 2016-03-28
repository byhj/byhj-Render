#ifndef OGLRTT_H
#define OGLRTT_H

#include "ogl/oglUtility.h"
#include "ogl/oglShader.h"

namespace byhj {

	class OGLRTT {
	public:
		OGLRTT() = default;
		~OGLRTT() = default;

		void init(glm::vec2 pos, glm::vec2 size);
		void update();
		void render(GLuint rttTex);
		void shutdown();

	private:
		const int m_IndexCount = 6;
		const int m_VertexCount = 6;

		void init_buffer(glm::vec2 pos, glm::vec2 size);
		void init_vertexArray();
		void init_shader();

		GLuint m_vao = OGLVALUE;
		GLuint m_vbo = OGLVALUE;
		GLuint m_ibo = OGLVALUE;
		GLuint m_program = OGLVALUE;
		GLuint tex_loc;
		OGLShader m_rttShader = "OGLRTT Shader";
		std::vector<GLuint> m_IndexData;
		std::vector<GLfloat> m_VertexData;
	}; 
}

#endif