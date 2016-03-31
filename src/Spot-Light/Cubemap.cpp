#include "Cubemap.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{


void Skymap::createSphere(ID3D11Device*pD3D11Device, int LatLines, int LongLines)
{
	XMMATRIX sphereWorld;
	XMMATRIX Rotationx;
	XMMATRIX Rotationy;

	HRESULT hr;

	NumSphereVertices = ((LatLines-2) * LongLines) + 2;
	NumSphereFaces  = ((LatLines-3)*(LongLines)*2) + (LongLines*2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for(DWORD i = 0; i < LatLines-2; ++i)
	{
		spherePitch = (i+1) * (3.14/(LatLines-1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for(DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28/(LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);

			currVertPos = XMVector3TransformNormal( XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), Rotationx * Rotationy );	
			//currVertPos = XMVector3Normalize(currVertPos);
			vertices[i*LongLines+j+1].pos.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines+j+1].pos.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines+j+1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices-1].pos.x =  0.0f;
	vertices[NumSphereVertices-1].pos.y =  0.0f;
	vertices[NumSphereVertices-1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData; 

	ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
	vertexBufferData.pSysMem = &vertices[0];
	hr = pD3D11Device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &m_pVertexBuffer);
	//DebugHR(hr);

	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for(DWORD l = 0; l < LongLines-1; ++l)
	{
		indices[k] = 0;
		indices[k+1] = l+1;
		indices[k+2] = l+2;
		k += 3;
	}

	indices[k] = 0;
	indices[k+1] = LongLines;
	indices[k+2] = 1;
	k += 3;

	for(DWORD i = 0; i < LatLines-3; ++i)
	{
		for(DWORD j = 0; j < LongLines-1; ++j)
		{
			indices[k]   = i*LongLines+j+1;
			indices[k+1] = i*LongLines+j+2;
			indices[k+2] = (i+1)*LongLines+j+1;

			indices[k+3] = (i+1)*LongLines+j+1;
			indices[k+4] = i*LongLines+j+2;
			indices[k+5] = (i+1)*LongLines+j+2;

			k += 6; // next quad
		}

		indices[k]   = (i*LongLines)+LongLines;
		indices[k+1] = (i*LongLines)+1;
		indices[k+2] = ((i+1)*LongLines)+LongLines;

		indices[k+3] = ((i+1)*LongLines)+LongLines;
		indices[k+4] = (i*LongLines)+1;
		indices[k+5] = ((i+1)*LongLines)+1;

		k += 6;
	}

	for(DWORD l = 0; l < LongLines-1; ++l)
	{
		indices[k] = NumSphereVertices-1;
		indices[k+1] = (NumSphereVertices-1)-(l+1);
		indices[k+2] = (NumSphereVertices-1)-(l+2);
		k += 3;
	}

	indices[k] = NumSphereVertices-1;
	indices[k+1] = (NumSphereVertices-1)-LongLines;
	indices[k+2] = NumSphereVertices-2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	hr = pD3D11Device->CreateBuffer(&indexBufferDesc, &iinitData, &m_pIndexBuffer);
	//DebugHR(hr);
}

void Skymap::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC InputLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;

	InputLayout.SemanticName         = "POSITION";
	InputLayout.SemanticIndex        = 0;
	InputLayout.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	InputLayout.InputSlot            = 0;
	InputLayout.AlignedByteOffset    = 0;
	InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	InputLayout.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(InputLayout);

	InputLayout.SemanticName         = "TEXCOORD";
	InputLayout.SemanticIndex        = 0;
	InputLayout.Format               = DXGI_FORMAT_R32G32_FLOAT;
	InputLayout.InputSlot            = 0;
	InputLayout.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	InputLayout.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(InputLayout);

	InputLayout.SemanticName         = "NORMAL";
	InputLayout.SemanticIndex        = 0;
	InputLayout.Format               = DXGI_FORMAT_R32G32_FLOAT;
	InputLayout.InputSlot            = 0;
	InputLayout.AlignedByteOffset    = 20;
	InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	InputLayout.InstanceDataStepRate = 0;
	vInputLayoutDesc.push_back(InputLayout);

	SkymapShader.init(pD3D11Device, vInputLayoutDesc);
	SkymapShader.attachVS(L"cubemap.vsh", "VS", "vs_5_0");
	SkymapShader.attachPS(L"cubemap.psh", "PS", "ps_5_0");
	SkymapShader.end();

}

void Skymap::load_texture(ID3D11Device *pD3D11Device, WCHAR *texFile)
{
	HRESULT hr;

	//D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	//loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* SMTexture = 0;
	hr = CreateDDSTextureFromFile(pD3D11Device, texFile, 0, &m_pShaderResourceView);
	//DebugHR(hr);

	/*
	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format                      = SMTextureDesc.Format;
	SMViewDesc.ViewDimension               = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels       = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	hr = pD3D11Device->CreateShaderResourceView(SMTexture, &SMViewDesc, &m_pShaderResourceView);
	*/

	////////////////////////Describe the Sample State///////////////////////////
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD         = 0;
	sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;
	//Create the Sample State
	hr = pD3D11Device->CreateSamplerState( &sampDesc, &m_pTexSamplerState );

	////////////////////////////////////////////////////////////////////////
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FrontCounterClockwise = false;
	hr = pD3D11Device->CreateRasterizerState(&rasterDesc, &m_pRSCullNone);
	//DebugHR(hr);

	//////////////////////////////////////////////////////////////////////
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable    = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL;
	hr = pD3D11Device->CreateDepthStencilState(&dssDesc, &m_pDSLessEqual);
	//DebugHR(hr);

	///////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC mvpDesc;	
	ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
	mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpDesc.ByteWidth      = sizeof(byhj::MatrixBuffer);
	mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpDesc.CPUAccessFlags = 0;
	mvpDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);
	//DebugHR(hr);

}

void Skymap::Render(ID3D11DeviceContext *pD3D11DeviceContext, const MatrixBuffer &mvpMatrix)
{
	// Set vertex buffer stride and offset.=
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex); 
	offset = 0;

	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetVertexBuffers( 0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	cbMatrix.Model = mvpMatrix.Model;
	cbMatrix.View  = mvpMatrix.View;
	cbMatrix.Proj  = mvpMatrix.Proj;

	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0 );
	pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, m_pMVPBuffer.GetAddressOf());
	pD3D11DeviceContext->PSSetShaderResources( 0, 1, m_pShaderResourceView.GetAddressOf());
	pD3D11DeviceContext->PSSetSamplers( 0, 1, m_pTexSamplerState.GetAddressOf());

	SkymapShader.use(pD3D11DeviceContext);

	pD3D11DeviceContext->OMSetDepthStencilState(m_pDSLessEqual.Get(), 0);

	pD3D11DeviceContext->RSSetState(m_pRSCullNone.Get());
	pD3D11DeviceContext->DrawIndexed(NumSphereFaces * 3, 0, 0 );
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pD3D11DeviceContext->OMSetDepthStencilState(NULL, 0);
}

}