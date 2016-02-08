#ifndef Bezier_H
#define Bezier_H

#include "ogl/oglShader.h"
#include "utility.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace byhj {

	class Bezier {
	public:
		Bezier();
		~Bezier();
        
		void init();
		void update();
		void render();
		void shutdown();

	private:
		void init_buffer();
		void init_vertexArray();
		void init_shader();
		
		struct  BezierVar
		{
			GLuint mv_loc;
			GLuint proj_loc;
			GLuint vbo;
			GLuint vao;
			GLuint program;
		}m_bezierVar;
		
		struct CvVar {
			GLuint color_loc;
			GLuint mvp_loc;
			GLuint ibo;
			GLuint program;
		}m_cvVar;
		glm::vec3 patch_data[16];
		OGLShader m_CVShader = "CV Shader";
		OGLShader m_BezierShader = "Bezier Shader";
	};
}
#endif
