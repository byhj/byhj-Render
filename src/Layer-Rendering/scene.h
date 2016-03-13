#ifndef Scene_H
#define Scene_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

namespace byhj
{

class Scene
{
public:
	Scene();
	~Scene();

    void init();
	void update();
	void render();
	void shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_fbo();

	GLuint width, height;
	GLuint program;
	GLuint vao, transform_ubo, array_texture, array_depth, layered_fbo;
	GLuint program_layer, program_show;

	OGLShader LayerShader, ShowShader;
};

}
#endif
