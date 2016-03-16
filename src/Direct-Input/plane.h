#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <d3d11.h>
#include <DirectXMath.h> 
using namespace DirectX;

#include "Common.h"
#include "d3d/d3dShader.h"
#include "d3d/D3DUtility.h"

#pragma comment(lib, "DirectXTK")

namespace byhj
{

	class Plane
	{

	public:
		Plane()   = default;
		~Plane()  = default;

	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix);
		void Shutdown();

	private:

		void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
		void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
		void init_texture(ID3D11Device *pD3D11Device);

		int m_VertexCount = 0;
		int m_IndexCount = 0;
		byhj::MatrixBuffer m_cbMatrix;
		byhj::LightBuffer cbLight;
		D3DShader TestShader;

		ComPtr<ID3D11Buffer> m_pVertexBuffer;
		ComPtr<ID3D11Buffer> m_pIndexBuffer;
		ComPtr<ID3D11Buffer> m_pMVPBuffer;
		ComPtr<ID3D11Buffer> m_pLightBuffer;
		ComPtr<ID3D11ShaderResourceView> m_pTexture;
		ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	};


}


#endif