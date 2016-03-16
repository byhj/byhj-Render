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

	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix);
	void shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
	};


	D3DMVPMatrix cbMatrix;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;

	int m_VertexCount;
	int m_IndexCount;

	D3DShader TriangleShader;
};


}


#endif 