#ifndef Torus_H
#define Torus_H

#include "ogl/oglShader.h"
#include "modelMgr.h"

namespace byhj {

	class Torus {
	public:
		void init();
		void update();
		void render();
		void shutdown();

	private:
		OGLShader m_TorusShader = "Torus Shader";
		GLuint m_program;
		struct UniformLoc
		{
			GLuint model;
			GLuint view;
			GLuint proj;
		}uniform_loc;
	};
}
#endif