#include "cube.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "textureMgr.h"
#include "windowInfo.h"

namespace byhj
{
void Cube::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	init_buffer(pD3D11Device, pD3D11DeviceContext);
	init_shader(pD3D11Device);
	init_texture(pD3D11Device);
}

void Cube::render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &model,
	const XMFLOAT4X4 &view, const XMFLOAT4X4 &proj)
{
   
	cbMatrix.model = model;
	cbMatrix.view  = view;
	cbMatrix.proj  = proj;
	XMVECTOR pos    = XMVectorSet(2.0f, 5.0f, -8.0f, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX view2  = XMMatrixTranspose(XMMatrixLookAtLH(pos, target, up));

	XMMATRIX proj2  = XMMatrixTranspose(XMMatrixPerspectiveFovLH(0.4f*3.14f, 1.0f, 1.0f, 1000.0f));
	
	XMStoreFloat4x4(&cbMatrix.view2, view2);
	XMStoreFloat4x4(&cbMatrix.proj2, proj2);
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf() );

	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex);
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0); 
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto pIceTexture = TextureMgr::getInstance()->getD3DTextureByName("ice.dds");
	auto pProjTexture = TextureMgr::getInstance()->getD3DTextureByName("grate.dds");

	pD3D11DeviceContext->PSSetShaderResources(0, 1, &pIceTexture);
	pD3D11DeviceContext->PSSetShaderResources(1, 1, &pProjTexture);
	
	pD3D11DeviceContext->PSSetSamplers(0, 1, m_pTexSamplerState.GetAddressOf());

	CubeShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

}


void Cube::shutdown()
{

}

void Cube::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{

	HRESULT hr;
	loadModel("./cube.txt");

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
	VBO.pSysMem          = &m_VertexData[0];
	VBO.SysMemPitch      = 0;
	VBO.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
	//DebugHR(hr);

	/////////////////////////////////Index Buffer ///////////////////////////////////////

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth           = sizeof(UINT)* m_IndexCount;
	IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags      = 0;
	IndexBufferDesc.MiscFlags           = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA IBO;
	IBO.pSysMem          = &m_IndexData[0];
	IBO.SysMemPitch      = 0;
	IBO.SysMemSlicePitch = 0;

	hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
	//DebugHR(hr);


	////////////////////////////////MVP Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpBufferDesc;
	ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
	mvpBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpBufferDesc.ByteWidth      = sizeof(MatrixBuffer);
	mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpBufferDesc.CPUAccessFlags = 0;
	mvpBufferDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);
	//DebugHR(hr);


}

void Cube::init_texture(ID3D11Device *pD3D11Device) 
{

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

	HRESULT hr;
	// Create the texture sampler state.
	hr = pD3D11Device->CreateSamplerState(&samplerDesc, &m_pTexSamplerState);
	assert(hr == S_OK);
	TextureMgr::getInstance()->loadD3DTexture(pD3D11Device, "ice.dds");
	TextureMgr::getInstance()->loadD3DTexture(pD3D11Device, "grate.dds");
}


void Cube::init_shader(ID3D11Device *pD3D11Device)
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;

	pInputLayoutDesc.SemanticName = "POSITION";
	pInputLayoutDesc.SemanticIndex = 0;
	pInputLayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc.InputSlot = 0;
	pInputLayoutDesc.AlignedByteOffset = 0;
	pInputLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(pInputLayoutDesc);

	pInputLayoutDesc.SemanticName = "TEXCOORD";
	pInputLayoutDesc.SemanticIndex = 0;
	pInputLayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	pInputLayoutDesc.InputSlot = 0;
	pInputLayoutDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(pInputLayoutDesc);

	pInputLayoutDesc.SemanticName = "NORMAL";
	pInputLayoutDesc.SemanticIndex = 0;
	pInputLayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc.InputSlot = 0;
	pInputLayoutDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(pInputLayoutDesc);
	

	CubeShader.init(pD3D11Device, vInputLayoutDesc);
	CubeShader.attach(D3D_VERTEX_SHADER, L"light.vsh", "VS", "vs_5_0");
	CubeShader.attach(D3D_PIXEL_SHADER,  L"light.psh", "PS", "ps_5_0");
	CubeShader.end();

}

void Cube::loadModel(std::string filename)
{
	std::ifstream fin;
	char ch;
	int i;
	fin.open(filename);

	if (fin.fail()) {
	  return;
	}

	// Read up to the value of vertex count.
	fin.get(ch);
	while (ch != ':') {
		fin.get(ch);
	}

	// Read in the vertex count.
	fin >> m_VertexCount;
	m_IndexCount = m_VertexCount;
	m_VertexData.resize(m_VertexCount);
	m_IndexData.resize(m_IndexCount);

	//Read up the beginning of the data
	fin.get(ch);
	while (ch != ':')
	{
		fin.get(ch);
	}

	for (int i = 0; i != m_VertexCount; ++i)
	{
		fin >> m_VertexData[i].position.x >> m_VertexData[i].position.y >> m_VertexData[i].position.z;
		fin >> m_VertexData[i].texcoord.x >> m_VertexData[i].texcoord.y;
		fin >> m_VertexData[i].normal.x >> m_VertexData[i].normal.y >> m_VertexData[i].normal.z;
		m_IndexData[i] = i;
	}

	fin.close();
}

}