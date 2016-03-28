#ifndef D3DRTT_H
#define D3DRTT_H

#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

namespace byhj
{
	class D3DRTT
	{
	public:
		D3DRTT() = default;
		~D3DRTT() = default;

		void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void render(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11ShaderResourceView *pTexture, const D3DMVPMatrix &matrix);
		void update();
		void shutdown();

		void init_window(int sw, int sh, int posX, int posY, int width, int height);
		void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
		void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);

	private:
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


		int m_VertexCount;
		int m_IndexCount;


		int m_posX;
		int m_posY;
		int m_width;
		int m_height;
		int m_sw;
		int m_sh;

		D3DMVPMatrix m_matrix;
		D3DShader m_D3DRTTShader;
	};

}

#endif