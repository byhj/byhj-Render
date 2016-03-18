#ifndef Scene_H
#define Scene_H

#include "ogl/oglShader.h"
#include "utility.h"
#include "geometry.h"

namespace byhj {
	class Scene {
	public:
		Scene();
		~Scene();

		void init();
		void update();
		void render();
		void shutdown();

	private:
		void init_buffer();
		void init_shader();
		void init_vertexArray();
		
		GLuint mvp_loc;
		OGLMeshData m_mesh;
		GLuint m_program;
		GLuint m_vao, m_vbo, m_ibo;
		OGLShader m_SceneShader;
	};
}
#endif