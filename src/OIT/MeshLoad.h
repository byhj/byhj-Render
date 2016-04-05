#ifndef MeshLoad_H
#define MeshLoad_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "modelMgr.h"
#include "ogl/oglShader.h"

#define MAX_FRAMEBUFFER_WIDTH 2048
#define MAX_FRAMEBUFFER_HEIGHT 2048
#define FRUSTUM_DEPTH 2000.0f

namespace byhj
{
	class MeshLoad
	{
	public:

		void init();
		void update();
		void render();
        void shutdown();

	private:
		void init_buffer();
		void init_shader();

		struct UniformLoc
		{
			GLuint model;
		    GLuint view;
			GLuint proj;
			GLuint ambient;
		}uniform_loc;


		OGLShader AppShader;
		OGLShader BlitShader;

		// Program to construct the linked list (renders the transparent objects)
		GLuint  list_build_program;

		// Head pointer image and PBO for clearing it
		GLuint  head_pointer_texture;
		GLuint  head_pointer_clear_buffer;
		// Atomic counter buffer
		GLuint  atomic_counter_buffer;
		// Linked list buffer
		GLuint  linked_list_buffer;
		GLuint  linked_list_texture;

		// Program to render the scene
		GLuint render_scene_prog;
		struct
		{
			GLint aspect;
			GLint time;
			GLint model_matrix;
			GLint view_matrix;
			GLint projection_matrix;
		} render_scene_uniforms;

		// Program to resolve 
		GLuint resolve_program;

		// Full Screen Quad
		GLuint  quad_vbo;
		GLuint  quad_vao;

		GLint current_width;
		GLint current_height;

		// Member variables
		float aspect;

	};
}
#endif