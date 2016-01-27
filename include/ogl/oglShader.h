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

class OGLShader
{

public:
	OGLShader() = default;
	OGLShader(std::string name) :m_name(name) {}
	~OGLShader() = default;

public:
	void init();
	void attach(int type, char *filename);
	void link();
	void info();
	void use() const; 
	void end() const;

	GLuint getProgram() const;


private:
	std::string textFileRead( char *fn) ;

	GLuint m_program;  
	std::string m_name;
};

}

#endif

