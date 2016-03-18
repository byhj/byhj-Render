#include "triangle.h"
#include <vector>

namespace byhj
{

Triangle::Triangle()
{

}

Triangle::~Triangle()
{

}

void Triangle::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{

	init_buffer(pD3D11Device, pD3D11DeviceContext);
	init_shader(pD3D11Device);
}

void Triangle::Update()
{

}

void Triangle::Render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix)
{

	cbMatrix.model = matrix.model;
	cbMatrix.view  = matrix.view;
	cbMatrix.proj  = matrix.proj;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0);
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &m_pMVPBuffer);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	TriangleShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(3, 0, 0);


}

void Triangle::Shutdown()
{
	TriangleShader.end();

	ReleaseCOM(m_pInputLayout)
	ReleaseCOM(m_pVS)
	ReleaseCOM(m_pPS)
	ReleaseCOM(m_pMVPBuffer)
	ReleaseCOM(m_pVertexBuffer)
	ReleaseCOM(m_pIndexBuffer)

}

void Triangle::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	m_VertexCount = 3;
	Vertex *VertexData = new Vertex[m_VertexCount];

	VertexData[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	VertexData[1].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	VertexData[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.

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
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	////////////////////////////////Const Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpDesc;
	ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
	mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpDesc.ByteWidth      = sizeof(D3DMVPMatrix);
	mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpDesc.CPUAccessFlags = 0;
	mvpDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);


}

void Triangle::init_shader(ID3D11Device *pD3D11Device)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc;

	inputLayoutDesc.SemanticName         = "POSITION";
	inputLayoutDesc.SemanticIndex        = 0;
	inputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayoutDesc.InputSlot            = 0;
	inputLayoutDesc.AlignedByteOffset    = 0;
	inputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(inputLayoutDesc);
	
	inputLayoutDesc.SemanticName         = "COLOR";
	inputLayoutDesc.SemanticIndex        = 0;
	inputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayoutDesc.InputSlot            = 0;
	inputLayoutDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(inputLayoutDesc);


	TriangleShader.init(pD3D11Device, vInputLayoutDesc);
	TriangleShader.attach(D3D_VERTEX_SHADER, L"triangle.vsh", "VS", "vs_4_0");
	TriangleShader.attach(D3D_PIXEL_SHADER,	 L"triangle.psh",  "PS", "ps_4_0");

}



}