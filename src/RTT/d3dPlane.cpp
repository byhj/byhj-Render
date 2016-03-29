#include "D3DPlane.h"
#include "textureMgr.h"

namespace byhj
{

D3DPlane::D3DPlane()
{

}

D3DPlane::~D3DPlane()
{

}

void D3DPlane::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
{
 
	init_buffer(pD3D11Device, pD3D11DeviceContext);
	init_shader(pD3D11Device, hWnd);
	init_texture(pD3D11Device);
}

void D3DPlane::update()
{

}

void D3DPlane::render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix)
{
	m_matrix.model = matrix.model;
	m_matrix.view  = matrix.view;
	m_matrix.proj  = matrix.proj;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &m_matrix, 0, 0);
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf() );

	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex);
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	pD3D11DeviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
	pD3D11DeviceContext->PSSetSamplers(0, 1, m_pTexSamplerState.GetAddressOf());

	TriangleShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(6, 0, 0);


}

void D3DPlane::shutdown()
{

}

void D3DPlane::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	m_VertexCount = 6;
	Vertex *VertexData = new Vertex[m_VertexCount];


	VertexData[0].Position = XMFLOAT3(-1.0f, 0.0f, 1.0f);  // Bottom left.
	VertexData[1].Position = XMFLOAT3(1.0f, 0.0f,  1.0f);    // Top middle.
	VertexData[2].Position = XMFLOAT3(1.0f, 0.0f, -1.0f);   // Bottom right.

	VertexData[3].Position = XMFLOAT3(1.0f, 0.0f, -1.0f);  // Bottom left.
	VertexData[4].Position = XMFLOAT3(-1.0f, 0.0f, -1.0f);    // Top middle.
	VertexData[5].Position = XMFLOAT3(-1.0f,0.0f, 1.0f);   // Bottom right.

	VertexData[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	VertexData[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	VertexData[2].TexCoord = XMFLOAT2(1.0f, 1.0f);
	VertexData[3].TexCoord = XMFLOAT2(1.0f, 1.0f);
	VertexData[4].TexCoord = XMFLOAT2(1.0f, 0.0f);
	VertexData[5].TexCoord = XMFLOAT2(0.0f, 0.0f);

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth           = sizeof(Vertex)* m_VertexCount;
	VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags      = 0;
	VertexBufferDesc.MiscFlags           = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA VBO;
	VBO.pSysMem          = VertexData;
	VBO.SysMemPitch      = 0;
	VBO.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
	//DebugHR(hr);

	/////////////////////////////////Index Buffer ///////////////////////////////////////
	m_IndexCount = 6;
	unsigned long *IndexData= new unsigned long[m_IndexCount];
	IndexData[0] = 0;  // Bottom left.
	IndexData[1] = 1;  // Top middle.
	IndexData[2] = 2;  // Bottom right.
		IndexData[3] = 3;  // Bottom right.
			IndexData[4] = 4;  // Bottom right.
				IndexData[5] = 5;  // Bottom right.

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth           = sizeof(unsigned long)* m_IndexCount;
	IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags      = 0;
	IndexBufferDesc.MiscFlags           = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA IBO;
	IBO.pSysMem          = IndexData;
	IBO.SysMemPitch      = 0;
	IBO.SysMemSlicePitch = 0;

	hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
	//DebugHR(hr);

	////////////////////////////////////////////////////////////////////////////////////////
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] VertexData;
	VertexData = 0;
	delete[] IndexData;
	IndexData = 0;

	////////////////////////////////Const Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpDesc;
	ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
	mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpDesc.ByteWidth      = sizeof(D3DMVPMatrix);
	mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpDesc.CPUAccessFlags = 0;
	mvpDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);
	//DebugHR(hr);

}

void D3DPlane::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
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

	TriangleShader.init(pD3D11Device, vInputLayoutDesc);
	TriangleShader.attach(D3D_VERTEX_SHADER, L"plane.vsh", "Plane_VS", "vs_5_0");
	TriangleShader.attach(D3D_PIXEL_SHADER,  L"plane.psh", "Plane_PS", "ps_5_0");
	TriangleShader.end();
}



void D3DPlane::init_texture(ID3D11Device *pD3D11Device)
{

	HRESULT hr;
	 m_pTexture = TextureMgr::getInstance()->loadD3DTexture(pD3D11Device, "grass.dds");
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