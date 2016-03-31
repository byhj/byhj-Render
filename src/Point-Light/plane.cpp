#include "Plane.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{

Plane::Plane()
{

}

Plane::~Plane()
{

}

void Plane::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
{
 
	init_buffer(pD3D11Device, pD3D11DeviceContext);
	init_shader(pD3D11Device, hWnd);
	init_texture(pD3D11Device);
}

void Plane::Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix)
{


	cbMatrix.model = matrix.model;
	cbMatrix.view  = matrix.view;
	cbMatrix.proj  = matrix.proj;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf() );
	pD3D11DeviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
	pD3D11DeviceContext->PSSetSamplers(0, 1, m_pTexSamplerState.GetAddressOf());

	PlaneShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(m_VertexData.size(), 0, 0);


}

void Plane::Shutdown()
{


}

void Plane::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;
	load_model("../../media/objects/floor.txt");

	///////////////////////////Index Buffer ////////////////////////////////

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth           = sizeof(Vertex)* m_VertexData.size();
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
	IndexBufferDesc.ByteWidth           = sizeof(DWORD)* m_IndexData.size();
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

	////////////////////////////////////////////////////////////////////////////////////////

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


	///////////////////////////////////////Light buffer////////////////////////////////////////
	D3DPointLight pLight;
	XMFLOAT4 color;
	XMFLOAT4 pos;

	color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	pos   = XMFLOAT4(-3.0f, 1.0f, 3.0f, 1.0f);
	pLight.SetLightColor(color);
	pLight.SetLightPos(pos);
	pointLights.push_back(pLight);

	color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	pos   = XMFLOAT4(3.0f, 1.0f, 3.0f, 1.0f);
	pLight.SetLightColor(color);
	pLight.SetLightPos(pos);
	pointLights.push_back(pLight);

	color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	pos   = XMFLOAT4(-3.0f, 1.0f, -3.0f, 1.0f);
	pLight.SetLightColor(color);
	pLight.SetLightPos(pos);
	pointLights.push_back(pLight);

	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pos   = XMFLOAT4(3.0f, 1.0f, -3.0f, 1.0f);
	pLight.SetLightColor(color);
	pLight.SetLightPos(pos);
	pointLights.push_back(pLight);

	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(D3D11_BUFFER_DESC));
	lightBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	lightBufferDesc.ByteWidth      = sizeof(D3DPointLight)* pointLights.size();
	lightBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = 0;
	lightBufferDesc.MiscFlags      = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA lightSub;
	lightSub.pSysMem          = &pointLights[0];
	lightSub.SysMemPitch      = 0;
	lightSub.SysMemSlicePitch = 0;
	hr = pD3D11Device->CreateBuffer(&lightBufferDesc, &lightSub, m_pLightBuffer.GetAddressOf());
	//DebugHR(hr);
	int lightSlot = 0;
	pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, m_pLightBuffer.GetAddressOf());

}

void Plane::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
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

	pInputLayoutDesc.SemanticName         = "NORMAL";
	pInputLayoutDesc.SemanticIndex        = 0;
	pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc.InputSlot            = 0;
	pInputLayoutDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(pInputLayoutDesc);
	

	PlaneShader.init(pD3D11Device, vInputLayoutDesc);
	PlaneShader.attachVS(L"Plane.vsh", "VS", "vs_5_0");
	PlaneShader.attachPS(L"Plane.psh", "PS", "ps_5_0");
	PlaneShader.end();
}



void Plane::init_texture(ID3D11Device *pD3D11Device)
{

	HRESULT hr;
	hr = CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/stone.dds", NULL, &m_pTexture);
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

void Plane::load_model(char *modelFile)
{
	std::ifstream fin;
	char ch;
	int i;
	fin.open(modelFile);

	if (fin.fail())
		return;

	// Read up to the value of vertex count.
	fin.get(ch);
	while (ch != ':')
	{
		fin.get(ch);
	}

	int VertexCount, IndexCount;
	// Read in the vertex count.
	fin >> VertexCount;
	IndexCount = VertexCount;
	m_VertexData.resize(VertexCount);
	m_IndexData.resize(IndexCount);


	//Read up the beginning of the data
	fin.get(ch);
	while (ch != ':')
	{
		fin.get(ch);
	}

	for (int i = 0; i != VertexCount; ++i)
	{
		fin >> m_VertexData[i].Position.x >> m_VertexData[i].Position.y >> m_VertexData[i].Position.z;
		fin >> m_VertexData[i].TexCoord.x >> m_VertexData[i].TexCoord.y;
		fin >> m_VertexData[i].Normal.x   >> m_VertexData[i].Normal.y   >> m_VertexData[i].Normal.z;

		m_IndexData[i] = static_cast<DWORD>(i);
	}
	fin.close();


}


}