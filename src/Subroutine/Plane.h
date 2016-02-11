#ifndef Plane_H
#define Plane_H

#include "ogl/oglShader.h"

namespace byhj {
	class Plane  {
	
	public:
		Plane() = default;
		~Plane() = default;

		void init();
		void update();
		void render();
		void shutdown();

	private:
		void init_shader();

		GLuint m_program;
		GLuint subroutines[2];
		GLuint sub_loc;
		OGLShader m_SubShader ={ "Subroutine Shader" };
	};
}
#endif 