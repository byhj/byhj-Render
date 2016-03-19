#ifndef Asteroid_H
#define Asteroid_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "modelMgr.h"
#include "LightGUI.h"
#include "RotationGui.h"
#include "ogl/oglShader.h"

namespace byhj
{
	class Asteroid
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
			GLuint ambient;
		}uniform_loc;

		GLuint m_vao;
		GLfloat m_Aspect = 1.0f;
		OGLShader m_AsteroidShader = "Asteroid Shader";
		GLuint m_Program;
		LightGui m_LightGui;
		RotationGui m_RotationGui;

	};
}
#endif