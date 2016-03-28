#ifndef MeshLoad_H
#define MeshLoad_H
#include<cassert>

#include "ogl/oglShader.h"
#include <gl/glew.h>

namespace byhj {

	class MeshLoad {
	public:
		MeshLoad();
		~MeshLoad();

		void init();
		void update();
		void render();
		void Shutdown();

	private:
		void init_buffer();
		void init_shader();

		GLuint m_program;
		OGLShader m_meshShader;
		struct UniformLocation {
		   GLuint model;
		   GLuint view;
		   GLuint proj;
		   GLuint skybox;
		}uniformLoc;

		GLuint skyboxTex;
	};

}

#endif                                   