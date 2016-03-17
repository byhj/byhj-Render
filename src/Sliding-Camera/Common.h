#ifndef COMMON_H
#define COMMON_H

#include <windows.h>
#include <DirectXMath.h> using namespace DirectX;

namespace byhj
{

struct Vertex	
{
	Vertex(){}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x,y,z), texCoord(u, v), normal(nx, ny, nz){}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};

struct MatrixBuffer
{
	XMFLOAT4X4  Model;
	XMFLOAT4X4  View;
	XMFLOAT4X4  Proj;
};

struct  LightBuffer
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;

	XMFLOAT3 lightDir;
	float    padding1;

	XMFLOAT3 lightPos;
	float    range;

	XMFLOAT3 att;
	float    padding2;
};

}
#endif