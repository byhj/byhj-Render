#ifndef D3DBitmap_H
#define D3DBitmap_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "d3d/d3dShader.h"
#include "d3d/Utility.h"

using namespace DirectX;


namespace byhj
{
	class D3DBitmap
	{
	public:
		D3DBitmap() = default;
		D3DBitmap(const D3DBitmap &) = default;
		~D3DBitmap() = default;


		void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd, WCHAR *texFile);
		void render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,
			const XMFLOAT4X4 &View, const XMFLOAT4X4&Proj);
		void update();
		void shutdown();
		void setPos(int sw, int sh, int posX, int posY, int width, int height);


	private:
		void init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
		void init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
		void init_texture(ID3D11Device *pD3D11Device, WCHAR *texFile);

		d3d::MatrixBuffer cbMatrix;

		struct  Vertex
		{
			XMFLOAT3 Pos;
			XMFLOAT2 Tex;
		};

		ID3D11RenderTargetView   *m_pRenderTargetView;
		ID3D11Buffer             *m_pMVPBuffer;
		ID3D11Buffer             *m_pVertexBuffer;
		ID3D11Buffer             *m_pIndexBuffer;
		ID3D11SamplerState       *m_pTexSamplerState;
		ID3D11InputLayout        *m_pInputLayout;
		ID3D11ShaderResourceView *m_pTextureSRV;

		int m_VertexCount;
		int m_IndexCount;

		int m_posX;
		int m_posY;
		int m_width;
		int m_height;
		int m_sw;
		int m_sh;

		D3DShader D3DRTTShader;
	};

}

#endif