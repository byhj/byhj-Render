#ifndef MeshLoad_H
#define MeshLoad_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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


		GLuint cubeVAO, planeVAO, cubeVBO, planeVBO;
		GLuint woodTex;
		GLuint scene_prog, shadow_prog;
		GLuint fbo, depth_tex;
		GLuint m_vao = OGL_ONE;
		GLuint m_vbo = OGL_ONE;
		GLuint m_ibo = OGL_ONE;
		GLuint m_tex = OGL_ONE;
		GLuint m_program = OGL_ONE;
		OGLShader PlaneShader = "Plane Shader";
		GLfloat m_Aspect = 1.0f;
		OGLShader m_MeshLoadShader = "Mesh Shader";
		GLuint tex_loc, tex1_loc;
		byhj::Model m_Model;
		GLuint lightProgram, shadowProgram;

	};
}
#endif