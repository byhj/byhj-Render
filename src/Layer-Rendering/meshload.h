#ifndef MeshLoad_H
#define MeshLoad_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

namespace byhj {
	class MeshLoad {
	public:

		void init();
		void update();
		void render();
		void shutdown();
	private:
		void init_buffer();
		void init_shader();
		void init_fbo();

		GLuint m_ubo;
		OGLShader m_showShader = "Show Shader";
		OGLShader m_layerShader = "Layer Shader";
		GLuint show_prog;
		GLuint layer_prog;
		GLuint array_texture, array_depth, layered_fbo;
	};

}

#endif