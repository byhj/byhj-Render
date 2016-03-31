#include "water.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{
	void Water::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		init_buffer(pD3D11Device, pD3D11DeviceContext);
	}
void Water::render(ID3D11DeviceContext *pD3D11DeviceContext)
{
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex);
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

}

void Water::shutdown()
{

}

bool Water::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	LoadModel("../../media/objects/water.txt");

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

}

bool Water::LoadModel(char *modelFile)
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

