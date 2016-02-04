#include "oglShader.h"
#include <fstream>
#include <iomanip>

namespace byhj
{
	OGLShader::OGLShader()
		:m_program(0), m_name("")
	{

	}

	OGLShader::OGLShader(std::string name) 
		:m_name(name) 
	{

	}

	OGLShader::~OGLShader()
	{

	}
//read the Shader code
std::string OGLShader::textFileRead(const char *fn) {  //read the OGLShader code

	FILE *fp = nullptr;  
	char *content = NULL;  
	int count=0;  

	if (fn != NULL) {  

		fp = fopen(fn, "r");
		if (!fp){
#ifdef WINDOW_PLATFORM
			MessageBox(NULL, L"Can not open the Shader file", L"Error",  MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "Can not open the Shader file:" << fn << std::endl;

#endif		
			fclose(fp);  
			return content;
		}
		else {  
			fseek(fp, 0, SEEK_END);  
			count = ftell(fp);  
			rewind(fp);  

			if (count > 0) {  
				content = (char *)malloc(sizeof(char) * (count+1));  
				count = fread(content,sizeof(char),count,fp);  
				content[count] = '\0';  
			}  
			fclose(fp);  
		} 
	}  
	std::string shardSrc(content);
	free(content);

	return shardSrc;
}  



void OGLShader::init()
{
	m_program = glCreateProgram();
	std::cout << "------------------------------" << m_name << " Message "<< "------------------------------" << std::endl;
}


//Attach different type shader
void OGLShader::attach(int type, const char* filename) 
{
	std::string src = textFileRead(filename);
	auto mem = src.c_str();
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, (const GLchar**)(&mem), 0);

	glCompileShader(handle);

	GLint compileSuccess=0;
	GLchar compilerSpew[256];

	glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);

	if (!compileSuccess) 
	{
		glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
		printf("OGLShader%s\n%s\ncompileSuccess=%d\n",filename, compilerSpew, compileSuccess);

		while(1);;
	}
	glAttachShader(m_program, handle); 

}

void OGLShader::link()
{
	glLinkProgram(m_program);  

	GLint linkSuccess;
	GLchar compilerSpew[256];
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkSuccess); //输出连接信息
	if(!linkSuccess) 
	{
		glGetProgramInfoLog(m_program, sizeof(compilerSpew), 0, compilerSpew);
		printf("ShaderLinker:\n%s\nlinkSuccess=%d\n",compilerSpew,linkSuccess);
		while(1);;
	}

	std::cout << m_name << " linked successful" << std::endl;
	
}

void OGLShader::info()
{
	std::cout << "                             " << m_name << " Interface                             " 
		      << std::endl;

	GLint outputs = 0;
	glGetProgramInterfaceiv(m_program, GL_PROGRAM_INPUT,  GL_ACTIVE_RESOURCES, &outputs);
	static const GLenum props[] = {GL_TYPE, GL_LOCATION};
	GLint params[2];
	GLchar name[64];
	const char *type_name;

	if (outputs > 0)
		std::cout << "***Input***" << std::endl;

	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(m_program, GL_PROGRAM_INPUT, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(m_program, GL_PROGRAM_INPUT, i, 2, props, 2, NULL, params);
		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}

	glGetProgramInterfaceiv(m_program, GL_PROGRAM_OUTPUT,  GL_ACTIVE_RESOURCES, &outputs);
	if (outputs > 0)
		std::cout << "***Onput***" << std::endl;

	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(m_program, GL_PROGRAM_OUTPUT, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(  m_program, GL_PROGRAM_OUTPUT, i, 2, props, 2, NULL, params);

		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}

	glGetProgramInterfaceiv(m_program, GL_UNIFORM_BLOCK,  GL_ACTIVE_RESOURCES, &outputs);
	if (outputs > 0)
		std::cout << "***Uniform Block***" << std::endl;

	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(m_program, GL_UNIFORM_BLOCK, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(  m_program, GL_UNIFORM_BLOCK, i, 2, props, 2, NULL, params);

		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}

	glGetProgramInterfaceiv(m_program, GL_UNIFORM,  GL_ACTIVE_RESOURCES, &outputs);
	if (outputs > 0)
		std::cout << "***Uniform***" << std::endl;
	if (outputs > 100)
		return ;
	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(m_program, GL_UNIFORM, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(  m_program, GL_UNIFORM, i, 2, props, 2, NULL, params);

		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
}

void OGLShader::use() 
{
	glUseProgram(m_program);
}
void OGLShader::end() 
{
	glUseProgram(0);
	glDeleteProgram(m_program);
}

GLuint OGLShader::getProgram() 
{
	return m_program;
}

}