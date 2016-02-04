#ifndef Shader_H
#define Shader_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "shader.h"

#ifdef _WIN32
#include <windows.h>
#define WINDOW_PLATFORM
#endif

namespace byhj
{

class OGLShader : public Shader
{

public:
	OGLShader();
	OGLShader(std::string name);
	~OGLShader();

public:
	void init();
	void attach(int type, const char *filename);
	void link();
	void info();
	void use(); 
	void end();

	GLuint getProgram();

	std::string textFileRead(const char *fn);
private:

	GLuint m_program;  
	std::string m_name;
};

}

#endif

