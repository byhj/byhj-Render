#include "RefractShader.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{
	void RefractShader::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
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


		m_RefractShader.init(pD3D11Device, vInputLayoutDesc);
		m_RefractShader.attach(D3D_VERTEX_SHADER, L"light.vsh", "LightVS", "vs_5_0");
		m_RefractShader.attach(D3D_PIXEL_SHADER, L"light.psh", "LightPS", "ps_5_0");
		m_RefractShader.end();


		D3D11_BUFFER_DESC mvpBufferDesc;
		ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
		mvpBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
		mvpBufferDesc.ByteWidth      = sizeof(D3DMVPMatrix);
		mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		mvpBufferDesc.CPUAccessFlags = 0;
		mvpBufferDesc.MiscFlags      = 0;
		hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);
		DebugHR(hr);

		D3D11_BUFFER_DESC clipBufferDesc;
		ZeroMemory(&clipBufferDesc, sizeof(D3D11_BUFFER_DESC));
		clipBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
		clipBufferDesc.ByteWidth      = sizeof(ClipPlaneBuffer);
		clipBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		clipBufferDesc.CPUAccessFlags = 0;
		clipBufferDesc.MiscFlags      = 0;
		hr = pD3D11Device->CreateBuffer(&clipBufferDesc, NULL, &m_pClipBuffer);
		DebugHR(hr);

		///////////////////////////////////////Light buffer////////////////////////////////////////
		D3D11_BUFFER_DESC lightBufferDesc;
		ZeroMemory(&lightBufferDesc, sizeof(D3D11_BUFFER_DESC));
		lightBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth      = sizeof(LightBuffer);
		lightBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags      = 0;

		hr = pD3D11Device->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer);
		DebugHR(hr);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		// Lock the light constant buffer so it can be written to.
		hr = pD3D11DeviceContext->Map(m_pLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		DebugHR(hr);

		// Get a pointer to the data in the constant buffer.
		LightBuffer *dataPtr2 = (LightBuffer*)mappedResource.pData;

		dataPtr2->ambientColor   = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
		dataPtr2->diffuseColor   = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		dataPtr2->lightDirection = XMFLOAT3(0.0f, -1.0f, 0.5f);

		pD3D11DeviceContext->Unmap(m_pLightBuffer.Get(), 0);


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

	void RefractShader::SetTexture(ID3D11Device *pD3D11Device, WCHAR *texFile)
	{
		HRESULT hr;
		hr = CreateDDSTextureFromFile(pD3D11Device, texFile, NULL, &m_pTexture);
	}

	void RefractShader::update()
	{
	}
	void RefractShader::use(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix)
	{
		cbMatrix = matrix;
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf());
		int lightSlot = 0;
		pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, m_pLightBuffer.GetAddressOf());


		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
		pD3D11DeviceContext->PSSetSamplers(0, 1, m_pTexSamplerState.GetAddressOf());

		m_RefractShader.use(pD3D11DeviceContext);
	}

	void RefractShader::shutdown()
	{

	}

}