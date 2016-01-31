#ifndef Planet_H
#define Planet_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"
#include "ogl/oglShader.h"

namespace byhj
{
	class Planet
	{
	public:

		void Init(int sw, int sh);
		void Update(const glm::mat4 &camMat);
		void Render();
		void Shutdown();

	private:
		void init_buffer();
		void init_shader();

		struct UniformLoc
		{
			GLuint model;
			GLuint view;
			GLuint proj;
		}uniform_loc;

		GLuint m_vao;
		GLfloat m_Aspect = 1.0f;
		OGLShader m_PlanetShader = "Planet Shader";
		GLuint m_Program;
		byhj::Model m_Model;
	};
}
#endif