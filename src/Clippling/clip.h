#ifndef Clip_H
#define Clip_H

#include <GL/glew.h>

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/oglModel.h"

namespace byhj
{

class Clip
{
public:
	Clip();
	~Clip();

	void init();
	void update();
	void render();
	void shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	OGLShader m_clipShader{ "Clip Shader" };
	GLuint    m_program = 0;

	struct UnfiormLoc {
	    GLuint model;
		GLuint view;
		GLuint proj;
		GLuint clipPlane;
		GLuint clipSphere;
	}m_uniformLoc;

	OGLModel m_model;
};

}
#endif
