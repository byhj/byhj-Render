#include "reflect.h"
#include "textureMgr.h"


namespace byhj {


bool Reflect::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	LoadModel("../../media/objects/floor.txt");

	Vertex*VertexData;
	unsigned long *IndexData;
	HRESULT result;

	VertexData = new Vertex[m_VertexCount];
	if (!VertexData)
	{
		return false;
	}

	IndexData = new unsigned long[m_IndexCount];
	if (!IndexData)
	{
		return false;
	}

	for (int i = 0; i != m_VertexCount; ++i)
	{
		VertexData[i].Pos    = XMFLOAT3(pModelVertex[i].x, pModelVertex[i].y, pModelVertex[i].z);
		VertexData[i].Tex    = XMFLOAT2(pModelVertex[i].tu, pModelVertex[i].tv);
		VertexData[i].Normal = XMFLOAT3(pModelVertex[i].nx, pModelVertex[i].ny, pModelVertex[i].nz);
		IndexData[i] = i;
	}

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth           = sizeof(Vertex) * m_VertexCount;
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
	DebugHR(hr);

	/////////////////////////////////Index Buffer ///////////////////////////////////////

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
	IBO.pSysMem          = IndexData;
	IBO.SysMemPitch      = 0;
	IBO.SysMemSlicePitch = 0;

	hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
	DebugHR(hr);


	////////////////////////////////MVP Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpBufferDesc;
	ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
	mvpBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpBufferDesc.ByteWidth      = sizeof(MatrixBuffer);
	mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpBufferDesc.CPUAccessFlags = 0;
	mvpBufferDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);
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
	hr = pD3D11DeviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	DebugHR(hr);

	// Get a pointer to the data in the constant buffer.
	LightBuffer *dataPtr2 = (LightBuffer*)mappedResource.pData;

	dataPtr2->ambientColor   = XMFLOAT4(0.35f, 0.35f, 0.35f, 0.15f);
	dataPtr2->diffuseColor   = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	dataPtr2->lightDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	dataPtr2->specularPower  = 32.0f;
	dataPtr2->specularColor  = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	pD3D11DeviceContext->Unmap(m_pLightBuffer, 0);

	int lightSlot = 0;
	pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, &m_pLightBuffer);


	D3D11_BUFFER_DESC cameraBufferDesc;
	cameraBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth           = sizeof(CameraBuffer);
	cameraBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags           = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = pD3D11Device->CreateBuffer(&cameraBufferDesc, NULL, &m_CameraBuffer);
	DebugHR(hr);

	// Lock the camera constant buffer so it can be written to.
	hr = pD3D11DeviceContext->Map(m_CameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	DebugHR(hr);

	// Get a pointer to the data in the constant buffer.
	CameraBuffer *dataPtr3 = (CameraBuffer*)mappedResource.pData;
	dataPtr3->camPos = XMFLOAT3(0.0f, 0.0f, -3.0f);
	dataPtr3->padding = 0.0f;
	pD3D11DeviceContext->Unmap(m_CameraBuffer, 0);

	int bufferSlot = 1;
	pD3D11DeviceContext->VSSetConstantBuffers(bufferSlot, 1, &m_CameraBuffer);

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
	DebugHR(hr);

	TextureMgr::getInstance()->loadD3DTexture(pD3D11Device, "blue01.dds");

	DebugHR(hr);


	return true;
}


bool Reflect::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT result;

	std::vector<D3D11_INPUT_ELEMENT_DESC> pInputLayoutDesc(3);
	pInputLayoutDesc[0].SemanticName = "POSITION";
	pInputLayoutDesc[0].SemanticIndex = 0;
	pInputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[0].InputSlot = 0;
	pInputLayoutDesc[0].AlignedByteOffset = 0;
	pInputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[0].InstanceDataStepRate = 0;

	pInputLayoutDesc[1].SemanticName = "TEXCOORD";
	pInputLayoutDesc[1].SemanticIndex = 0;
	pInputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	pInputLayoutDesc[1].InputSlot = 0;
	pInputLayoutDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[1].InstanceDataStepRate = 0;

	pInputLayoutDesc[2].SemanticName = "NORMAL";
	pInputLayoutDesc[2].SemanticIndex = 0;
	pInputLayoutDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[2].InputSlot = 0;
	pInputLayoutDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[2].InstanceDataStepRate = 0;


	unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

	ReflectShader.init(pD3D11Device, pInputLayoutDesc);
	ReflectShader.attach(D3D_VERTEX_SHADER,  L"Reflect.vsh", "VS", "vs_5_0");
	ReflectShader.attach(D3D_PIXEL_SHADER,   L"Reflect.psh", "PS", "ps_5_0");
	ReflectShader.end();

	return true;
}

bool Reflect::LoadModel(char *modelFile)
{
	std::ifstream fin;
	char ch;
	int i;
	fin.open(modelFile);

	if (fin.fail())
	{
		return false;
	}
	// Read up to the value of vertex count.
	fin.get(ch);
	while (ch != ':')
	{
		fin.get(ch);
	}

	// Read in the vertex count.
	fin >> m_VertexCount;
	m_IndexCount = m_VertexCount;

	pModelVertex  = new ModelVertex[m_VertexCount];
	if (!pModelVertex)
	{
		return false;
	}

	//Read up the beginning of the data
	fin.get(ch);
	while (ch != ':')
	{
		fin.get(ch);
	}

	for (int i = 0; i != m_VertexCount; ++i)
	{
		fin >> pModelVertex[i].x >> pModelVertex[i].y >> pModelVertex[i].z;
		fin >> pModelVertex[i].tu >> pModelVertex[i].tv;
		fin >> pModelVertex[i].nx >> pModelVertex[i].ny >> pModelVertex[i].nz;
	}
	fin.close();

	return true;

}

}