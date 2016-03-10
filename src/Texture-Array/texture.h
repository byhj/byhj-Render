#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include <array>
#include <glm/glm.hpp>

#include "ogl/oglShader.h"
#include "ktx.h"

const int TEX_NUM = 256;

namespace byhj
{

class Texture
{
public:
	Texture();
	~Texture();

	void init();
	void update();
	void render();
	void shutdown();

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	OGLShader TextureArrayShader = { "TextureArray Shader" };

	GLuint program;
	GLuint tex_loc;
	GLuint mvp_loc;
	GLuint vao;
	GLuint ubo;

	GLuint tex_alien_array = 0;

    std::array<float, TEX_NUM> droplet_x_offset;
	std::array<float, TEX_NUM> droplet_rot_speed;
	std::array<float, TEX_NUM> droplet_fall_speed;

};

}
#endif
