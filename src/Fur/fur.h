#ifndef Fur_H
#define Fur_H

#include "ogl/oglShader.h"
#include "modelMgr.h"
#include "ogl/oglEulerCamera.h"

namespace byhj
{
	class Fur {
	public:
		Fur() = default;
		~Fur() = default;

		void init();
		void update();
		void render();
		void shutdown();

	private:
		void init_buffer();
		void init_shader();
		
		struct Uniform_Loc {
			GLuint model;
			GLuint view;
			GLuint proj;
		};
		Uniform_Loc furLoc, baseLoc;
		GLuint base_prog, fur_prog;
		OGLShader m_FurShader = "Fur Shader";
		OGLShader m_BaseShader = "Base Shader";
	};
}
#endif