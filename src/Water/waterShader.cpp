#include "waterShader.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{
	void WaterShader::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{


		HRESULT hr;

		D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc;
		std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;

		pInputLayoutDesc.SemanticName         = "POSITION";
		pInputLayoutDesc.SemanticIndex        = 0;
		pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc.InputSlot            = 0;
		pInputLayoutDesc.AlignedByteOffset    = 0;
		pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		pInputLayoutDesc.SemanticName         = "TEXCOORD";
		pInputLayoutDesc.SemanticIndex        = 0;
		pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32_FLOAT;
		pInputLayoutDesc.InputSlot            = 0;
		pInputLayoutDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		pInputLayoutDesc.SemanticName         = "NORMAL";
		pInputLayoutDesc.SemanticIndex        = 0;
		pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc.InputSlot            = 0;
		pInputLayoutDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);


		m_WaterShader.init(pD3D11Device, vInputLayoutDesc);
		m_WaterShader.attach(D3D_VERTEX_SHADER, L"water.vsh", "WaterVS", "vs_5_0");
		m_WaterShader.attach(D3D_PIXEL_SHADER,  L"water.psh", "WaterPS", "ps_5_0");
		m_WaterShader.end();

		////////////////////////////////MVP Buffer//////////////////////////////////////


		D3D11_BUFFER_DESC mvpBufferDesc;
		ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
		mvpBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
		mvpBufferDesc.ByteWidth      = sizeof(ReflectMatrixBuffer);
		mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		mvpBufferDesc.CPUAccessFlags = 0;
		mvpBufferDesc.MiscFlags      = 0;
		hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);
		//DebugHR(hr);

		D3D11_BUFFER_DESC waterBufferDesc;
		ZeroMemory(&waterBufferDesc, sizeof(D3D11_BUFFER_DESC));
		waterBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		waterBufferDesc.ByteWidth = sizeof(WaterBufer);
		waterBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		waterBufferDesc.MiscFlags = 0;
		waterBufferDesc.CPUAccessFlags =0;
		hr = pD3D11Device->CreateBuffer(&waterBufferDesc, NULL, &m_pWaterBuffer);
		///////////////////////////////////////Light buffer////////////////////////////////////////

		// Create a texture sampler state description.
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		hr = pD3D11Device->CreateSamplerState(&samplerDesc, &m_pTexSamplerState);
		//DebugHR(hr);

	}


	void WaterShader::update()
	{

	}
	void WaterShader::use(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix, const XMFLOAT4X4 &relfectMat)
	{
		cbMatrix.model  = matrix.model;
		cbMatrix.view   = matrix.view;
		cbMatrix.proj   = matrix.proj;
		cbMatrix.reflectMat = relfectMat;
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf());


		// Update the position of the water to simulate motion.
		static FLOAT m_waterTranslation = 0.0f;

		m_waterTranslation += 0.0001f;
		if (m_waterTranslation > 1.0f)
		{
			m_waterTranslation -= 1.0f;
		}
		water.reflectRefractScale = 0.01f;
		water.waterTranslation = m_waterTranslation;
		m_WaterShader.use(pD3D11DeviceContext);
		pD3D11DeviceContext->UpdateSubresource(m_pWaterBuffer.Get(), 0, NULL, &water, 0, 0);
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, m_pWaterBuffer.GetAddressOf());
	}

	void WaterShader::shutdown()
	{

	}


}






