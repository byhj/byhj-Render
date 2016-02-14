#ifndef MeshLoad_H
#define MeshLoad_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"
#include "LightGUI.h"
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
		void init_fbo();

		struct UniformLoc
		{
			GLuint model;
		    GLuint view;
			GLuint proj;
			GLuint ambient;
		}uniform_loc;
		
		struct GBuffer {
			GLuint fbo;
			GLuint posTex;
			GLuint normalTex;
			GLuint colorTex;
		}gbuffer;
		GLuint planeVAO, planeVBO;
		OGLShader m_LightShader;
		OGLShader m_DeferredShader;

		GLuint m_DeferredProgram;
		GLuint m_LightProgram;
		GLfloat sw, sh, aspect;
		byhj::Model m_Model;
		LightGui m_LightGui;
		RotationGui m_RotationGui;

	};
}
#endif