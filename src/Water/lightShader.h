#ifndef LightShader_H
#define LightShader_H

#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

namespace byhj
{
	class LightShader
	{
	public:
		LightShader() = default;
		~LightShader() = default;

		void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void update();
		void use(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix);
		void shutdown();

		struct CameraBuffer
		{
			XMFLOAT3 camPos;
			float padding;
		};

		D3DMVPMatrix cbMatrix;

		struct LightBuffer
		{
			XMFLOAT4 ambientColor;
			XMFLOAT4 diffuseColor;
			XMFLOAT3 lightDirection;
			FLOAT  padding;
		};
		LightBuffer cbLight;

	private:

		D3DShader m_LightShader;

		ComPtr<ID3D11Buffer> m_pMVPBuffer;
		ComPtr<ID3D11Buffer> m_pLightBuffer;
		ComPtr<ID3D11Buffer> m_CameraBuffer;
		ComPtr<ID3D11SamplerState> m_pTexSamplerState;
		ComPtr<ID3D11InputLayout> m_pInputLayout;
	};
}
#endif