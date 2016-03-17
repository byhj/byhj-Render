#include "Plane.h"
#include "DirectXTK/WICTextureLoader.h"

#include <array>

namespace byhj
{

	void Plane::Init(ID3D11Device *pD3D11Device,ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{
		init_buffer(pD3D11Device, pD3D11DeviceContext);
		init_shader(pD3D11Device, hWnd);
		init_texture(pD3D11Device);
	}

	void Plane::Render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix)
	{
		m_cbMatrix.Model = matrix.model;
		m_cbMatrix.View  = matrix.view;
		m_cbMatrix.Proj  = matrix.proj;
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &m_cbMatrix, 0, 0 );
		pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, m_pMVPBuffer.GetAddressOf());
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, m_pLightBuffer.GetAddressOf());

		// Set vertex buffer stride and offset
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(Vertex); 
		offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//Set the texture for shader resoucres and the texture  samplers status
		pD3D11DeviceContext->PSSetShaderResources( 0, 1, m_pTexture.GetAddressOf() );
		pD3D11DeviceContext->PSSetSamplers( 0, 1, m_pTexSamplerState.GetAddressOf());

		TestShader.use(pD3D11DeviceContext);

		pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

	}

	void Plane::Shutdown()
	{

	}

	void Plane::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		///////////////////////////Index Buffer ////////////////////////////////
		m_VertexCount = 4;
		std::array<byhj::Vertex, 4> VertexData = 
		{
			byhj::Vertex(-1.0f, 0.0f, -1.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f),
			byhj::Vertex(-1.0f, 0.0f,  1.0f,  0.0f, 100.0f, 0.0f, 1.0f, 0.0f),
			byhj::Vertex( 1.0f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
			byhj::Vertex( 1.0f, 0.0f, -1.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		};
		for (auto val : VertexData) {
		    m_VertexData.push_back(val.pos);
		}
		// Set up the description of the static vertex buffer.
		D3D11_BUFFER_DESC VertexBufferDesc;
		VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth           = sizeof(byhj::Vertex) * m_VertexCount;
		VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags      = 0;
		VertexBufferDesc.MiscFlags           = 0;
		VertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		D3D11_SUBRESOURCE_DATA VBO;
		VBO.pSysMem          = &VertexData;
		VBO.SysMemPitch      = 0;
		VBO.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		HRESULT hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
		//DebugHR(hr);

		/////////////////////////////////Index Buffer ///////////////////////////////////////
		m_IndexCount = 6;
		std::array<DWORD, 6>  IndexData = 		
		{
			0,  1,  2,
			0,  2,  3,
		};
		for (auto val : IndexData) {
		  m_IndexData.push_back(val);
		}
		// Set up the description of the static index buffer.
		D3D11_BUFFER_DESC IndexBufferDesc;
		IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_IndexCount;
		IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags      = 0;
		IndexBufferDesc.MiscFlags           = 0;
		IndexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		D3D11_SUBRESOURCE_DATA IBO;
		IBO.pSysMem          = &IndexData;
		IBO.SysMemPitch      = 0;
		IBO.SysMemSlicePitch = 0;

		hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);

		////////////////////////////////Const Buffer//////////////////////////////////////

		D3D11_BUFFER_DESC mvpDesc;	
		ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
		mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
		mvpDesc.ByteWidth      = sizeof(byhj::MatrixBuffer);
		mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		mvpDesc.CPUAccessFlags = 0;
		mvpDesc.MiscFlags      = 0;
		pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);

		////////////////////////////////////////////////////////////////////////////////
		D3D11_BUFFER_DESC cbLightDesc;
		ZeroMemory(&cbLightDesc, sizeof(D3D11_BUFFER_DESC));
		cbLightDesc.Usage          = D3D11_USAGE_DYNAMIC;
		cbLightDesc.ByteWidth      = sizeof(LightBuffer);
		cbLightDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		cbLightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbLightDesc.MiscFlags      = 0;
		hr = pD3D11Device->CreateBuffer(&cbLightDesc, NULL, &m_pLightBuffer);
		//DebugHR(hr);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = pD3D11DeviceContext->Map(m_pLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		//DebugHR(hr);
		LightBuffer *plightData = (LightBuffer *)mappedResource.pData;

		plightData->ambient   = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		plightData->diffuse   = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		plightData->lightDir  = XMFLOAT3(0.25f, 0.5f, -1.0f);
		plightData->padding1  = 0.0f;
		plightData->lightPos  = XMFLOAT3(1.0f, 1.0f, 3.0f);
		plightData->range     = 100.0f;
		plightData->att       = XMFLOAT3(0.0f, 0.2f, 0.0f);
		plightData->padding2  = 0.0f;

		pD3D11DeviceContext->Unmap(m_pLightBuffer.Get(), 0);
	}

	void Plane::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
	{
		//Shader interface infomation
		D3D11_INPUT_ELEMENT_DESC InputLayout;
		std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;

		InputLayout.SemanticName         = "POSITION";
		InputLayout.SemanticIndex        = 0;
		InputLayout.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		InputLayout.InputSlot            = 0;
		InputLayout.AlignedByteOffset    = 0;
		InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayout.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(InputLayout);               

		InputLayout.SemanticName         = "TEXCOORD";
		InputLayout.SemanticIndex        = 0;
		InputLayout.Format               = DXGI_FORMAT_R32G32_FLOAT;
		InputLayout.InputSlot            = 0;
		InputLayout.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayout.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(InputLayout);  

		InputLayout.SemanticName         = "NORMAL";
		InputLayout.SemanticIndex        = 0;
		InputLayout.Format               = DXGI_FORMAT_R32G32_FLOAT;
		InputLayout.InputSlot            = 0;
		InputLayout.AlignedByteOffset    = 20;
		InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayout.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(InputLayout);     

		TestShader.init(pD3D11Device, vInputLayoutDesc);
		TestShader.attach(D3D_VERTEX_SHADER, L"Plane.vsh", "VS", "vs_5_0");
		TestShader.attach(D3D_PIXEL_SHADER, L"Plane.psh", "PS", "ps_5_0");
		TestShader.end();
	}

	void Plane::init_texture(ID3D11Device *pD3D11Device)
	{
		LPCWSTR  texFile = L"../../media/textures/grass.jpg";

		HRESULT hr;
		//Use shaderResourceView to make texture to the shader
		hr = CreateWICTextureFromFile(pD3D11Device, texFile, NULL, &m_pTexture);
		//DebugHR(hr);

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
}

