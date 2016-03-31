#include "depthShader.h"

namespace byhj
{


	void DepthShader::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
{

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
	pInputLayoutDesc.AlignedByteOffset = 0;
	pInputLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(pInputLayoutDesc);

	pInputLayoutDesc.SemanticName = "NORMAL";
	pInputLayoutDesc.SemanticIndex = 0;
	pInputLayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc.InputSlot = 0;
	pInputLayoutDesc.AlignedByteOffset = 0;
	pInputLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(pInputLayoutDesc);

	depthShader.init(pD3D11Device, vInputLayoutDesc);
	depthShader.attachVS(L"depth.vsh", "VS", "vs_5_0");
	depthShader.attachPS(L"depth.psh", "PS", "ps_5_0");
	depthShader.end();


	HRESULT hr;
	D3D11_BUFFER_DESC mvpBufferDesc;
	ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
	mvpBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	mvpBufferDesc.ByteWidth = sizeof(d3d::MatrixBuffer);
	mvpBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mvpBufferDesc.CPUAccessFlags = 0;
	mvpBufferDesc.MiscFlags = 0;
	hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);

}

void DepthShader::Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix )
{
	depthShader.use(pD3D11DeviceContext);

	cbMatrix = matrix;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf() );
}

void DepthShader::Shutdown()
{
	depthShader.end();
}

}