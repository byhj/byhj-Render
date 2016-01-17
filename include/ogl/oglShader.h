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

class OGLShader : public byhj::Shader
{

public:
	OGLShader();
    OGLShader(std::string name);
	~OGLShader();

public:
	void init();
	void attach(int type, char *filename);
	void link();
	void info();
	void use() const; 
	void end() const;

	GLuint getProgram() const;


private:
	char *textFileRead( char *fn) ;

	GLuint m_program;   
};

}

#endif

