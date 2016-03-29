#ifndef Triangle_H
#define Triangle_H

#include <windows.h>
#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

namespace byhj
{


class Triangle
{
public:
	Triangle();
	~Triangle();

	void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void Update();
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix);
	void Shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
	};


	d3d::MatrixBuffer cbMatrix;
	ComPtr<ID3D11InputLayout> m_pInputLayout;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;

	int m_VertexCount;
	int m_IndexCount;

	D3DShader TriangleShader;
};


}


#endif 