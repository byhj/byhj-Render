#ifndef Utility_H
#define Utility_H

#include <string>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <DirectXMath.h>

using namespace DirectX;

namespace byhj
{
#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])
#define SAFE_DELETE(ptr) if (ptr != nullptr) { delete ptr; ptr = nullptr; }
#define GL_BUFFER_OFFSET(offset)  (GLvoid*)(NULL + offset) 

const GLuint OGL_ZERO = 0x00000000;
const GLuint OGL_ONE  = 0xffffffff;

struct OGL_MVP {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

struct D3D_MVP {
	XMFLOAT4X4 model;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
};

bool test(bool, std::string, int, char*);

#ifdef _DEBUG
#define CHECK_OGL_UNIFORM(exp , description) \
        if (test( (int)exp, description, __LINE__, __FILE__) \
    {_asm{int 3} }
#else
#define CHECK_OGL_UNIFORM(exp, description)
#endif


}
#endif