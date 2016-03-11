#include "object.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{


void Object::Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,  
					const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj)
{

	cbMatrix.model  = Model;
	cbMatrix.view   = View;
	cbMatrix.proj   = Proj;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0 );
	pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, m_pMVPBuffer.GetAddressOf());


	//////////////////////////////////////////////////////////////////////////////
	static float frameTime = 0.0f;
	// Increment the frame time counter.
	frameTime += 0.0001f;
	if(frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}
	cbNoise.frameTime = frameTime;
	cbNoise.scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);
	cbNoise.scales       = XMFLOAT3(1.0f, 2.0f, 3.0f);
	cbNoise.padding = 0.0f;
	pD3D11DeviceContext->UpdateSubresource(m_pNoiseBuffer.Get(), 0, NULL, &cbNoise, 0, 0 );
	pD3D11DeviceContext->VSSetConstantBuffers(1, 1, m_pNoiseBuffer.GetAddressOf());

	int lightSlot = 0;
	pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, m_pDistortBuffer.GetAddressOf());

	///////////////////////////////////////////////////////////////////////////////
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex); 
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0); 
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pD3D11DeviceContext->PSSetShaderResources(0, 3, m_pTextures);  
	pD3D11DeviceContext->PSSetSamplers( 0, 1, m_pTexSamplerState.GetAddressOf() );
	pD3D11DeviceContext->PSSetSamplers( 1, 1, m_pTexSamplerState1.GetAddressOf() );
	ObjectShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

}

bool Object::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	LoadModel("../../media/objects/square.txt");


	Vertex *VertexData = new Vertex[m_VertexCount];
	unsigned long * IndexData = new unsigned long[m_IndexCount];

	for (int i = 0; i != m_VertexCount; ++i)
	{
		VertexData[i].Position = XMFLOAT3(m_pModelVertex[i].x,  m_pModelVertex[i].y,  m_pModelVertex[i].z);
		VertexData[i].TexCoord = XMFLOAT2(m_pModelVertex[i].u,  m_pModelVertex[i].v);
		VertexData[i].Normal   = XMFLOAT3(m_pModelVertex[i].nx, m_pModelVertex[i].ny, m_pModelVertex[i].nz);
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
	//DebugHR(hr);

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
	//DebugHR(hr);


	////////////////////////////////MVP Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpBufferDesc;	
	ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
	mvpBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpBufferDesc.ByteWidth      = sizeof(d3d::MatrixBuffer);
	mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpBufferDesc.CPUAccessFlags = 0;
	mvpBufferDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);
	//DebugHR(hr);

	///////////////////////////////////////Light buffer////////////////////////////////////////

	D3D11_BUFFER_DESC noiseBufferDesc;	
	ZeroMemory(&noiseBufferDesc, sizeof(D3D11_BUFFER_DESC));
	noiseBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	noiseBufferDesc.ByteWidth      = sizeof(NoiseBuffer);
	noiseBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	noiseBufferDesc.CPUAccessFlags = 0;
	noiseBufferDesc.MiscFlags      = 0;

	hr = pD3D11Device->CreateBuffer(&noiseBufferDesc, NULL, &m_pNoiseBuffer);

	//////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC distortBufferDesc;	
	ZeroMemory(&distortBufferDesc, sizeof(D3D11_BUFFER_DESC));
	distortBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	distortBufferDesc.ByteWidth      = sizeof(DistortionBuffer);
	distortBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	distortBufferDesc.CPUAccessFlags = 0;
	distortBufferDesc.MiscFlags      = 0;

	cbDistortion.distortion1 = XMFLOAT2(0.1f, 0.2f);
	cbDistortion.distortion2 = XMFLOAT2(0.1f, 0.3f);
	cbDistortion.distortion3 = XMFLOAT2(0.1f, 0.1f);
	cbDistortion.distortionScale = 0.8f;
	cbDistortion.distortionBias  = 0.5f;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA distortSub;
	distortSub.pSysMem          = &cbDistortion;
	distortSub.SysMemPitch      = 0;
	distortSub.SysMemSlicePitch = 0;
	hr = pD3D11Device->CreateBuffer(&distortBufferDesc, &distortSub, &m_pDistortBuffer);
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


	D3D11_SAMPLER_DESC samplerDesc2;
	samplerDesc2.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc2.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc2.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc2.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc2.MipLODBias = 0.0f;
	samplerDesc2.MaxAnisotropy = 1;
	samplerDesc2.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc2.BorderColor[0] = 0;
	samplerDesc2.BorderColor[1] = 0;
	samplerDesc2.BorderColor[2] = 0;
	samplerDesc2.BorderColor[3] = 0;
	samplerDesc2.MinLOD = 0;
	samplerDesc2.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pD3D11Device->CreateSamplerState(&samplerDesc2, &m_pTexSamplerState1);

	hr = CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/fire01.dds", NULL,  &m_pTextures[0]);
	//DebugHR(hr);
	hr = CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/noise01.dds", NULL, &m_pTextures[1]);
	//DebugHR(hr);
	hr = CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/alpha01.dds", NULL, &m_pTextures[2]);
	//DebugHR(hr);
	return true;
}


bool Object::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
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
	

	ObjectShader.init(pD3D11Device, vInputLayoutDesc);
	ObjectShader.attachVS(L"light.vsh", "VS", "vs_5_0");
	ObjectShader.attachPS(L"light.psh", "PS", "ps_5_0");
	ObjectShader.end();

	return true;
}

bool Object::LoadModel(char *modelFile)
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
	while(ch != ':')
	{
		fin.get(ch);
	}

	// Read in the vertex count.
	fin >> m_VertexCount;
	m_IndexCount = m_VertexCount;

	m_pModelVertex  = new ModelVertex[m_VertexCount];
	if (!m_pModelVertex)
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
		fin >> m_pModelVertex[i].x  >> m_pModelVertex[i].y >> m_pModelVertex[i].z;
		fin >> m_pModelVertex[i].u  >> m_pModelVertex[i].v;
		fin >> m_pModelVertex[i].nx >> m_pModelVertex[i].ny >> m_pModelVertex[i].nz;
	}
	fin.close();

	return true;

}


}