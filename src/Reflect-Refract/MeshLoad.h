#ifndef MeshLoad_H
#define MeshLoad_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "modelMgr.h"
#include "RotationGui.h"
#include "ogl/oglShader.h"

namespace byhj
{
	class MeshLoad
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
			GLuint tex;
			GLuint camPos;
		}uniform_loc;

		GLfloat m_Aspect = 1.0f;
		OGLShader m_MeshLoadShader = "Model Shader";
		GLuint m_program;
		RotationGui m_RotationGui;

	};
}
#endif