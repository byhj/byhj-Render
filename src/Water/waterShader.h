#ifndef WaterShader_H
#define WaterShader_H

#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

namespace byhj
{
	class WaterShader
	{
	public:
		WaterShader() = default;
		~WaterShader() = default;

		struct ReflectMatrixBuffer
		{
			XMFLOAT4X4  model;
			XMFLOAT4X4  view;
			XMFLOAT4X4  proj;
			XMFLOAT4X4  reflectMat;

		};
		ReflectMatrixBuffer cbMatrix;

		struct WaterBufer
		{
			FLOAT waterTranslation;
			FLOAT reflectRefractScale;
			XMFLOAT2 padding;
		};
		WaterBufer water;

		void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void update();
		void use(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix, const XMFLOAT4X4 &relfectMat);
		void shutdown();

	private:
		D3DShader m_WaterShader;
		D3DMVPMatrix m_cbMatrix;

		ComPtr<ID3D11Buffer> m_pMVPBuffer;
		ComPtr<ID3D11Buffer> m_pWaterBuffer;
		ComPtr<ID3D11ShaderResourceView> m_pTexture;
		ComPtr<ID3D11SamplerState> m_pTexSamplerState;
		ComPtr<ID3D11InputLayout> m_pInputLayout;
	};
}
#endif
