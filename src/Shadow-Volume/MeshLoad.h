#ifndef MeshLoad_H
#define MeshLoad_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "modelMgr.h"
#include "ogl/oglEulerCamera.h"

#include "utility.h"
#include "ogl/oglShader.h"
#include "LightGUI.h"

namespace byhj
{
	class MeshLoad
	{
	public:

		void Init();
		void Update();
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
		}uniform_loc;


		GLuint planeVAO, planeVBO;
		GLuint planeTex, benchTex;
		GLuint scene_prog, shadow_prog;
		GLuint fbo, depthTex;
		GLuint m_tex = OGL_ONE;
		OGLShader PlaneShader = "Plane Shader";
		OGLShader m_MeshLoadShader = "Mesh Shader";
		GLuint texLocs[2];

		GLuint lightProgram, shadowProgram;
		LightGui m_lightGui;
	};
}
#endif