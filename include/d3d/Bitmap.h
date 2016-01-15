#ifndef BITMAP_H
#define BITMAP_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "d3d/Shader.h"
#include "d3d/Utility.h"

using namespace DirectX;


namespace byhj
{
	class Bitmap
	{
	public:
		Bitmap() = default;
		Bitmap(const Bitmap &) = default;
		~Bitmap() = default;


		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd, WCHAR *texFile);
		void Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,
			const XMFLOAT4X4 &View, const XMFLOAT4X4&Proj);
		void Update();
		void Shutdown();
		void SetPos(int sw, int sh, int posX, int posY, int width, int height);


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

		d3d::Shader D3DRTTShader;
	};

}

#endif