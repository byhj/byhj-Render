#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/oglEulerCamera.h"

namespace byhj
{

class Terrain
{
public:
	Terrain();
	~Terrain();

	void init();
	void render();
	void shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	GLuint          program;
	GLuint          vao;
	GLuint          tex_displacement;
	GLuint          tex_color;

	float           dmap_depth = 0.0f;
	bool            enable_displacement = true;
	bool            wireframe = false;
	bool            enable_fog = true;
	bool            paused;

	OGLShader dispmapShader;
	struct
	{
		GLint       mvp_matrix;
		GLint       mv_matrix;
		GLint       proj_matrix;
		GLint       dmap_depth;
		GLint       enable_fog;
	} uniforms;

};

}
#endif
