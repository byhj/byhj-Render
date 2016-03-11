#ifndef MeshLoad_H
#define MeshLoad_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/oglCamera.h"

#include "model.h"
#include "utility.h"
#include "ogl/oglShader.h"

namespace byhj
{
	class MeshLoad
	{
	public:

		void Init();
		void Update();
		void Render(const OGLCamera &camera);
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
		byhj::Model m_Model;
		GLuint texLocs[2];

		GLuint lightProgram, shadowProgram;

	};
}
#endif