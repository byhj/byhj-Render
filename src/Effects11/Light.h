#ifndef LIGHT_H
#define LIGHT_H

#include <d3d11.h>

#include <DirectXMath.h> 
using namespace DirectX;


namespace byhj
{

namespace d3d
{

struct DirectionLight 
{
	DirectionLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; 
	XMFLOAT3 Direction;
	FLOAT    padding;
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Position;
	FLOAT    Range;
	XMFLOAT3 Att;
	FLOAT    padding;
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Position;
	FLOAT    Range;
	XMFLOAT3 Direction;
	FLOAT    Spot;
	XMFLOAT3 Att;
	FLOAT    Pad; 
};

struct Material
{
   Material() { ZeroMemory(this, sizeof(this)); }

   XMFLOAT4 Ambient;
   XMFLOAT4 Diffuse;
   XMFLOAT4 Specular; // w = SpecPower
   XMFLOAT4 Reflect;
};

}

}
#endif