#include "MeshLoad.h"
#include "modelMgr.h"
#include "ogl/oglEulerCamera.h"

namespace byhj
{
	void MeshLoad::init()
	{
		init_buffer();
		init_shader();
	}

	void MeshLoad::update()
	{

	}

	void MeshLoad::render()
	{
		float t;

		unsigned int current_time = GetTickCount();

		t = (float)(current_time & 0xFFFFF) / (float)0x3FFF;

		GLuint * data;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Reset atomic counter
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer);
		data = (GLuint *)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);
		data[0] = 0;
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

		// Clear head-pointer image
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
		glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, current_width, current_height, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Bind head-pointer image for read-write
		glBindImageTexture(0, head_pointer_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

		// Bind linked-list buffer for write
		glBindImageTexture(1, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

		glUseProgram(render_scene_prog);

		    glm::mat4 model_matrix = glm::mat4(1.0f);
			glm::mat4 view_matrix =  OGLEulerCamera::getInstance()->getViewMat();
			glm::mat4 projection_matrix = glm::frustum(-1.0f, 1.0f, aspect, -aspect, 1.0f, 40.f);

		glUniformMatrix4fv(render_scene_uniforms.model_matrix, 1, GL_FALSE, &model_matrix[0][0]);
		glUniformMatrix4fv(render_scene_uniforms.view_matrix, 1, GL_FALSE, &view_matrix[0][0]);
		glUniformMatrix4fv(render_scene_uniforms.projection_matrix, 1, GL_FALSE, &projection_matrix[0][0]);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ModelMgr::getInstance()->render(render_scene_prog);

		glDisable(GL_BLEND);

		glBindVertexArray(quad_vao);
		glUseProgram(resolve_program);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	}

	void MeshLoad::shutdown()
	{
	}

	void MeshLoad::init_buffer()
	{
		GLuint * data;
		render_scene_prog = -1;

		// Create head pointer texture
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &head_pointer_texture);
		glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindImageTexture(0, head_pointer_texture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);

		// Create buffer for clearing the head pointer texture
		glGenBuffers(1, &head_pointer_clear_buffer);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint), NULL, GL_STATIC_DRAW);
		data = (GLuint *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		memset(data, 0x00, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint));
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

		// Create the atomic counter buffer
		glGenBuffers(1, &atomic_counter_buffer);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_buffer);
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

		// Create the linked list storage buffer
		glGenBuffers(1, &linked_list_buffer);
		glBindBuffer(GL_TEXTURE_BUFFER, linked_list_buffer);
		glBufferData(GL_TEXTURE_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * 3 * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);

		// Bind it to a texture (for use as a TBO)
		glGenTextures(1, &linked_list_texture);
		glBindTexture(GL_TEXTURE_BUFFER, linked_list_texture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, linked_list_buffer);
		glBindTexture(GL_TEXTURE_BUFFER, 0);

		glBindImageTexture(1, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

		glGenVertexArrays(1, &quad_vao);
		glBindVertexArray(quad_vao);

		static const GLfloat quad_verts[] =
		{
			-0.0f, -1.0f,
			1.0f, -1.0f,
			-0.0f,  1.0f,
			1.0f,  1.0f,
		};

		glGenBuffers(1, &quad_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_verts), quad_verts, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glClearDepth(1.0f);

		ModelMgr::getInstance()->loadOGLModel("armadillo.obj");
	}


	void MeshLoad::init_shader()
	{
		AppShader.init();
		AppShader.attach(GL_VERTEX_SHADER, "build.vert");
		AppShader.attach(GL_FRAGMENT_SHADER, "build.frag");
		AppShader.info();
		AppShader.link();
		render_scene_prog  = AppShader.getProgram();
		render_scene_uniforms.model_matrix = glGetUniformLocation(render_scene_prog, "model_matrix");
		render_scene_uniforms.view_matrix = glGetUniformLocation(render_scene_prog, "view_matrix");
		render_scene_uniforms.projection_matrix = glGetUniformLocation(render_scene_prog, "projection_matrix");
		render_scene_uniforms.aspect = glGetUniformLocation(render_scene_prog, "aspect");
		render_scene_uniforms.time = glGetUniformLocation(render_scene_prog, "time");

		BlitShader.init();
		BlitShader.attach(GL_VERTEX_SHADER, "resolve.vert");
		BlitShader.attach(GL_FRAGMENT_SHADER, "resolve.frag");
		BlitShader.link();
		BlitShader.info();
		resolve_program = BlitShader.getProgram();
		
	}
}
