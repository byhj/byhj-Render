#include "Model.h"

namespace byhj
{


void Model::Render(ID3D11DeviceContext *pD3D11DeviceContext)
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

bool Model::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
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
	IndexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_IndexCount;
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

	return true;
}


bool Model::load_model(char *modelFile)
{
	std::ifstream fin;
	char ch;
	int i;
	fin.open(modelFile);

	if (fin.fail())
		return false;

	// Read up to the value of vertex count.
	fin.get(ch);
	while (ch != ':')
	{
		fin.get(ch);
	}


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
		fin >> m_VertexData[i].Position.x >> m_VertexData[i].Position.y >> m_VertexData[i].Position.z;
		fin >> m_VertexData[i].TexCoord.x >> m_VertexData[i].TexCoord.y;
		fin >> m_VertexData[i].Normal.x >> m_VertexData[i].Normal.y >> m_VertexData[i].Normal.z;

		m_IndexData[i] = static_cast<DWORD>(i);
	}
	fin.close();

	return true;
}

}