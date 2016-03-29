#include "Triangle.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{

Triangle::Triangle()
{

}

Triangle::~Triangle()
{

}

void Triangle::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
{
 
	init_buffer(pD3D11Device, pD3D11DeviceContext);
	init_shader(pD3D11Device, hWnd);
	init_texture(pD3D11Device);
}

void Triangle::Update()
{

}

void Triangle::Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix)
{


	cbMatrix.model = matrix.model;
	cbMatrix.view  = matrix.view;
	cbMatrix.proj  = matrix.proj;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf() );
	pD3D11DeviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
	pD3D11DeviceContext->PSSetSamplers(0, 1, m_pTexSamplerState.GetAddressOf());

	TriangleShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(3, 0, 0);


}

void Triangle::Shutdown()
{

}

void Triangle::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	m_VertexCount = 3;
	Vertex *VertexData = new Vertex[m_VertexCount];


	VertexData[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	VertexData[1].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);    // Top middle.
	VertexData[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);   // Bottom right.

	VertexData[0].TexCoord = XMFLOAT2(0.0f, 1.0f);
	VertexData[1].TexCoord = XMFLOAT2(0.5f, 0.0f);
	VertexData[2].TexCoord = XMFLOAT2(1.0f, 1.0f);

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
	m_IndexCount = 3;
	unsigned long *IndexData= new unsigned long[m_IndexCount];
	IndexData[0] = 0;  // Bottom left.
	IndexData[1] = 1;  // Top middle.
	IndexData[2] = 2;  // Bottom right.

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

	// Set vertex buffer stride and offset.=
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex);
	offset = 0;
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0); 
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	////////////////////////////////Const Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpDesc;
	ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
	mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpDesc.ByteWidth      = sizeof(d3d::MatrixBuffer);
	mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpDesc.CPUAccessFlags = 0;
	mvpDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);
	//DebugHR(hr);

}

void Triangle::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
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
	TriangleShader.attachVS(L"triangle.vsh", "VS", "vs_5_0");
	TriangleShader.attachPS(L"triangle.psh", "PS", "ps_5_0");
	TriangleShader.end();
}



void Triangle::init_texture(ID3D11Device *pD3D11Device)
{

	HRESULT hr;
	hr = CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/stone.dds", NULL,  &m_pTexture);
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