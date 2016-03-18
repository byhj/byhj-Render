#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <d3d11.h>
#include <DirectXMath.h>

#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

using namespace DirectX;

namespace byhj
{

class Triangle
{

public:
	Triangle();
	~Triangle();

public:
	void Update();
	void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix);
	void Shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device);

	struct Vertex
	{
		XMFLOAT3 Position;
	};

	ID3D11InputLayout  *m_pInputLayout       = nullptr;
	ID3D11VertexShader *m_pVS                = nullptr;
	ID3D11PixelShader  *m_pPS                = nullptr;
	ID3D11Buffer       *m_pVertexBuffer      = nullptr;
	ID3D11Buffer       *m_pIndexBuffer       = nullptr;
	ID3D11Buffer       *m_pMVPBuffer         = nullptr;

	int m_IndexCount = 0;
	int m_VertexCount = 0;
	D3DMVPMatrix cbMatrix;
	D3DShader TriangleShader;
};


}

#endif