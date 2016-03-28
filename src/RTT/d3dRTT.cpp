#include "D3DRTT.h"

namespace byhj
{

	void D3DRTT::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{
		init_buffer(pD3D11Device, pD3D11DeviceContext);
		init_shader(pD3D11Device, hWnd);
	}

void D3DRTT::init_window(int sw, int sh, int posX, int posY, int width, int height)
{
	m_posX  = posX;
	m_posY  = posY;
	m_width = width;
	m_height = height;
	m_sw = sw;
	m_sh = sh;
}

void D3DRTT::render(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11ShaderResourceView *pTexture, const D3DMVPMatrix &matrix)
{

	m_matrix.model  =  matrix.model;
	m_matrix.view   =  matrix.view;
	m_matrix.proj   =  matrix.proj;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &m_matrix, 0, 0 );
	pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, &m_pMVPBuffer);

	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex); 
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pD3D11DeviceContext->PSSetShaderResources(0, 1, &pTexture);  
	pD3D11DeviceContext->PSSetSamplers( 0, 1, &m_pTexSamplerState );

	m_D3DRTTShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

}

void D3DRTT::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	Vertex *VertexData;
	unsigned long *IndexData;
	m_VertexCount = 6;
	m_IndexCount = 6;

	VertexData = new Vertex[m_VertexCount];
	IndexData = new unsigned long[m_IndexCount];

	memset(VertexData, 0, sizeof(Vertex) * m_VertexCount);

	for (int i = 0; i != m_IndexCount; ++i)
	{
		IndexData[i] = i;
	}

	// Calculate the screen coordinates of the left side of the bitmap.
	float left = (float)((m_sw / 2) * -1) + (float)m_posX;

	// Calculate the screen coordinates of the right side of the bitmap.
	float  right = left + (float)m_width;

	// Calculate the screen coordinates of the top of the bitmap.
	float  top = (float)(m_sh / 2) - (float)m_posY;

	// Calculate the screen coordinates of the bottom of the bitmap.
	float  bottom = top - (float)m_height;

	// First triangle.
	VertexData[0].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	VertexData[0].Tex = XMFLOAT2(0.0f, 0.0f);

	VertexData[1].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	VertexData[1].Tex = XMFLOAT2(1.0f, 1.0f);

	VertexData[2].Pos = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	VertexData[2].Tex = XMFLOAT2(0.0f, 1.0f);

	// Second triangle.
	VertexData[3].Pos = XMFLOAT3(left, top, 0.0f);  // Bottom left.
	VertexData[3].Tex = XMFLOAT2(0.0f, 0.0f);

	VertexData[4].Pos = XMFLOAT3(right, top, 0.0f); // Top right.
	VertexData[4].Tex = XMFLOAT2(1.0f, 0.0f);

	VertexData[5].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	VertexData[5].Tex = XMFLOAT2(1.0f, 1.0f);


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
	mvpBufferDesc.ByteWidth      = sizeof(D3DMVPMatrix);
	mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpBufferDesc.CPUAccessFlags = 0;
	mvpBufferDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);
	DebugHR(hr);

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

}


void D3DRTT::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT result;
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;
	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc;
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

	m_D3DRTTShader.init(pD3D11Device, vInputLayoutDesc);
	m_D3DRTTShader.attach(D3D_VERTEX_SHADER, L"D3DRTT.vsh", "RTT_VS", "vs_5_0");
	m_D3DRTTShader.attach(D3D_PIXEL_SHADER,  L"D3DRTT.psh", "RTT_PS", "ps_5_0");
	m_D3DRTTShader.end();

}

}
