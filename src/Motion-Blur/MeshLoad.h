#ifndef MeshLoad_H
#define MeshLoad_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "modelMgr.h"
#include "ogl/oglShader.h"

namespace byhj
{
	class MeshLoad
	{
	public:

		void init();
		void update();
		void render();
        void shutdown();

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
			GLuint prevMVP;
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

	};
}
#endif