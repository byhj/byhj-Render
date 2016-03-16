#include "Model.h"
#include <vector>
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{

Model::Model()
{

}

Model::~Model()
{

}

void Model::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	m_SdkMesh.Create(pD3D11Device, L"Tiny\\tiny.sdkmesh");

	init_buffer(pD3D11Device, pD3D11DeviceContext);
	init_shader(pD3D11Device);
	init_texture(pD3D11Device);
}

void Model::update()
{

}

void Model::render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix)
{
	////////////////////////////////////////////////////////////////////////////////////////

	CubeShader.use(pD3D11DeviceContext);

	// Set vertex buffer stride and offset.=
	unsigned int stride;
	unsigned int offset;
	stride = m_SdkMesh.GetVertexStride(0, 0);
	offset = 0;
	auto pVertexBuffer = m_SdkMesh.GetVB11(0, 0);
	auto pIndexBuffer = m_SdkMesh.GetIB11(0);
	auto IndexFormat = m_SdkMesh.GetIBFormat11(0);
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(pIndexBuffer, IndexFormat, 0);

	cbMatrix.model = matrix.model;
	cbMatrix.view  = matrix.view;
	cbMatrix.proj  = matrix.proj;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf());
	pD3D11DeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

	for (UINT subIndex = 0; subIndex<m_SdkMesh.GetNumSubsets(0); ++subIndex)
	{
		auto pSubMesh = m_SdkMesh.GetSubset(0, subIndex);
		auto PrimType = m_SdkMesh.GetPrimitiveType11( (SDKMESH_PRIMITIVE_TYPE)pSubMesh->PrimitiveType );
		pD3D11DeviceContext->IASetPrimitiveTopology(PrimType);

       //Ignores most of the material information in the mesh to use only a simple shader
		auto pTextureSRV = m_SdkMesh.GetMaterial(pSubMesh->MaterialID)->pDiffuseRV11;
		pD3D11DeviceContext->PSSetShaderResources(0, 1, &pTextureSRV);
		
		auto IndexCount = (UINT)pSubMesh->IndexCount;
		auto VertexStart =  (UINT)pSubMesh->VertexStart;

		pD3D11DeviceContext->DrawIndexed(IndexCount, 0, VertexStart);
	}


}

void Model::shutdown()
{
	m_SdkMesh.Destroy();
	CubeShader.end();

}

void Model::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;
	////////////////////////////////Const Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpDesc;
	ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
	mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpDesc.ByteWidth      = sizeof(D3DMVPMatrix);
	mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpDesc.CPUAccessFlags = 0;
	mvpDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);

	D3D11_BUFFER_DESC lightDesc;
	ZeroMemory(&lightDesc, sizeof(D3D11_BUFFER_DESC) );
	lightDesc.Usage =  D3D11_USAGE_DYNAMIC;
	lightDesc.ByteWidth = sizeof(XMFLOAT4);
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightDesc.MiscFlags = 0;
	hr = pD3D11Device->CreateBuffer(&lightDesc, NULL, &m_pLightBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	pD3D11DeviceContext->Map(m_pLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	auto pLightData = reinterpret_cast<LightBuffer*>(mappedResource.pData);
	pLightData->lightPos = XMFLOAT4(-0.577f, 0.577f, -0.577f, 0.f);
	pD3D11DeviceContext->Unmap(m_pLightBuffer.Get(), 0);

	int lightSlot = 0;
	pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, &m_pLightBuffer);

}

void Model::init_shader(ID3D11Device *pD3D11Device)
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
	
	inputLayoutDesc.SemanticName         = "NORMAL";
	inputLayoutDesc.SemanticIndex        = 0;
	inputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayoutDesc.InputSlot            = 0;
	inputLayoutDesc.AlignedByteOffset    = 12;
	inputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(inputLayoutDesc);

	inputLayoutDesc.SemanticName         = "TEXCOORD";
	inputLayoutDesc.SemanticIndex        = 0;
	inputLayoutDesc.Format               = DXGI_FORMAT_R32G32_FLOAT;
	inputLayoutDesc.InputSlot            = 0;
	inputLayoutDesc.AlignedByteOffset    = 24;
	inputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(inputLayoutDesc);

	CubeShader.init(pD3D11Device, vInputLayoutDesc);
	CubeShader.attach(D3D_VERTEX_SHADER, L"Model.vsh", "VS", "vs_5_0");
	CubeShader.attach(D3D_PIXEL_SHADER,  L"Model.psh", "PS", "ps_5_0");
}

void Model::init_texture(ID3D11Device *pD3D11Device)
{
	
	// Create the sample state
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

	pD3D11Device->CreateSamplerState(&samplerDesc, &m_pSamplerLinear);

}


}