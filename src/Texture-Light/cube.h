#ifndef CUBE_H
#define CUBE_H

#include "d3d/D3DShader.h"

#include <string>
#include <DirectXMath.h>
using namespace DirectX;
    
namespace byhj
{


class Cube
{
public:
	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &model,
		          const XMFLOAT4X4 &view, const XMFLOAT4X4 &proj);
	void shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device);
	void init_texture(ID3D11Device *PD3D11Device);

	void loadModel(std::string filename);

	struct MatrixBuffer
	{
		XMFLOAT4X4  model;
		XMFLOAT4X4  view;
		XMFLOAT4X4  proj;

		XMFLOAT4X4  view2;
		XMFLOAT4X4  proj2;
	};
	MatrixBuffer cbMatrix;

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texcoord;
		XMFLOAT3 normal;
	};

	std::vector<Vertex> m_VertexData;
	std::vector<UINT>  m_IndexData;
	UINT m_VertexCount;
	UINT m_IndexCount;

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	ComPtr<ID3D11InputLayout> m_pInputLayout;
	D3DShader CubeShader;
};


}

#endif