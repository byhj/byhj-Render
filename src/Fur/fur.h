#ifndef Fur_H
#define Fur_H

#include "ogl/oglShader.h"
#include "modelMgr.h"

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
		}uniform_loc;

		GLuint m_program;
		OGLShader m_FurShader = "Fur Shader";
	};
}
#endif