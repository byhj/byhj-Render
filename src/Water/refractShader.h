#ifndef RefractShader_H
#define RefractShader_H

#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

namespace byhj
{
	class RefractShader
	{
	public:
		RefractShader() = default;
		~RefractShader() = default;

		void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void update();
		void use(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix);
		void shutdown();
		void RefractShader::SetTexture(ID3D11Device *pD3D11Device, WCHAR *texFile);

		struct ClipPlaneBuffer
		{
			XMFLOAT4 clipPlane;
		};

		D3DMVPMatrix cbMatrix;

		struct LightBuffer
		{
			XMFLOAT4 ambientColor;
			XMFLOAT4 diffuseColor;
			XMFLOAT3 lightDirection;
			FLOAT  padding;
		};

	private:
		D3DShader m_RefractShader;

		ComPtr<ID3D11Buffer> m_pMVPBuffer;
		ComPtr<ID3D11Buffer> m_pLightBuffer;
		ComPtr<ID3D11Buffer> m_pClipBuffer;
		ComPtr<ID3D11ShaderResourceView> m_pTexture;
		ComPtr<ID3D11SamplerState> m_pTexSamplerState;
		ComPtr<ID3D11InputLayout> m_pInputLayout;
	};
}
#endif