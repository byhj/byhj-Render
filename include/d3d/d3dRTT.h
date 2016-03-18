#ifndef D3DRTT_H
#define D3DRTT_H

#include "d3d/Shader.h"
#include "d3d/d3dDebug.h"
#include <d3d11.h>
#include <xnamath.h>

namespace byhj
{

	namespace d3d
	{

		class RTT
		{
		public:
			RTT()
			{
				m_pInputLayout        = NULL;
				m_pMVPBuffer          = NULL;
				m_pVertexBuffer       = NULL;
				m_pIndexBuffer        = NULL;
			}

			void Render(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11ShaderResourceView *pTexture, const XMFLOAT4X4  &Model,
				const XMFLOAT4X4 &View, const XMFLOAT4X4  &Proj);

			void shutdown()
			{
				ReleaseCOM(m_pRenderTargetView)
					ReleaseCOM(m_pMVPBuffer)
					ReleaseCOM(m_pVertexBuffer)
					ReleaseCOM(m_pIndexBuffer)
			}

			void init_window(float posX, float posY, float width, float height, float aspect);
			bool init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
			bool init_shader(ID3D11Device *pD3D11Device, HWND hWnd);

		private:

			struct MatrixBuffer
			{
				XMFLOAT4X4  model;
				XMFLOAT4X4  view;
				XMFLOAT4X4  proj;

			};
			MatrixBuffer cbMatrix;

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

			float m_posX  ;
			float m_posY  ;
			float m_width ;
			float m_height;
			float m_aspect;
			Shader D3DRTTShader;
		};

	}

}

#endif