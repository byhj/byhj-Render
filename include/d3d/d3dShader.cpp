#include "Shader.h"

#include <d3dcompiler.h>
#include "d3d/Utility.h"

namespace byhj
{

namespace d3d
{


void Shader::init(ID3D11Device *pD3D11Device, const std::vector<D3D11_INPUT_ELEMENT_DESC> &vInputDesc)
{
	this->pD3D11Device = pD3D11Device;
	this->vInputLayoutDesc = vInputDesc;

}

HRESULT Shader::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			//OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			std::cout << reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()) << std::endl;
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}


void Shader::attachVS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{	
	HRESULT result;
	ID3DBlob* VertexShaderBuffer = 0;

	CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel, &VertexShaderBuffer);
	

	// Create the vertex shader from the buffer.
	result = pD3D11Device->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &pVS_Shader);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto numElements = vInputLayoutDesc.size();
	result = pD3D11Device->CreateInputLayout(&vInputLayoutDesc[0], numElements, VertexShaderBuffer->GetBufferPointer(), 
	                                     	VertexShaderBuffer->GetBufferSize(), &pInputLayout);


	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	VertexShaderBuffer->Release();
	VertexShaderBuffer = 0;

}

void Shader::attachHS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{	
	HRESULT result;
	ID3DBlob* errorMessage = 0;
	ID3DBlob* HullShaderBuffer = 0;

	CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel, &HullShaderBuffer);

	// Create the vertex shader from the buffer.
	result = pD3D11Device->CreateHullShader(HullShaderBuffer->GetBufferPointer(), HullShaderBuffer->GetBufferSize(), NULL, &pHS_Shader);

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	HullShaderBuffer->Release();
	HullShaderBuffer = 0;
}
void Shader::attachDS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{	
	HRESULT result;
	ID3DBlob* errorMessage = 0;
	ID3DBlob* DomainShaderBuffer = 0;

	CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel, &DomainShaderBuffer);

	// Create the vertex shader from the buffer.
	result = pD3D11Device->CreateDomainShader(DomainShaderBuffer->GetBufferPointer(), DomainShaderBuffer->GetBufferSize(), NULL, &pDS_Shader);

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	DomainShaderBuffer->Release();
	DomainShaderBuffer = 0;

}

void Shader::attachGS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{	
	HRESULT result;
	ID3DBlob* errorMessage = 0;
	ID3DBlob* GeometryShaderBuffer = 0;

	CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel, &GeometryShaderBuffer);

	// Create the vertex shader from the buffer.
	result = pD3D11Device->CreateGeometryShader(GeometryShaderBuffer->GetBufferPointer(), GeometryShaderBuffer->GetBufferSize(), NULL, &pGS_Shader);

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	GeometryShaderBuffer->Release();
	GeometryShaderBuffer = 0;

}

void Shader::attachCS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{	
	HRESULT result;
	ID3DBlob* errorMessage = 0;
	ID3DBlob* ComputeShaderBuffer = 0;


	CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel, &ComputeShaderBuffer);

	// Create the vertex shader from the buffer.
	result = pD3D11Device->CreateComputeShader(ComputeShaderBuffer->GetBufferPointer(), ComputeShaderBuffer->GetBufferSize(), NULL, &pCS_Shader);


	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	ComputeShaderBuffer->Release();
	ComputeShaderBuffer = 0;

}

void Shader::attachPS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{	
	HRESULT result;
	ID3DBlob* errorMessage = 0;
	ID3DBlob* PixelShaderBuffer = 0;


	CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel, &PixelShaderBuffer);

	// Create the vertex shader from the buffer.
	result = pD3D11Device->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &pPS_Shader);


	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	PixelShaderBuffer->Release();
	PixelShaderBuffer = 0;

}

void Shader::use(ID3D11DeviceContext *pD3D11DeviceContext)
{
	pD3D11DeviceContext->IASetInputLayout(pInputLayout.Get());

	//Not Conside Computer now
	if (pVS_Shader != 0)
		pD3D11DeviceContext->VSSetShader(pVS_Shader.Get(), NULL, 0);
	if (pHS_Shader != 0)						   
		pD3D11DeviceContext->HSSetShader(pHS_Shader.Get(), NULL, 0);
	if (pDS_Shader != 0)						
		pD3D11DeviceContext->DSSetShader(pDS_Shader.Get(), NULL, 0);
	if (pGS_Shader != 0)					
		pD3D11DeviceContext->GSSetShader(pGS_Shader.Get(), NULL, 0);
	if (pVS_Shader != 0)						  
		pD3D11DeviceContext->PSSetShader(pPS_Shader.Get(), NULL, 0);
}

void Shader::end()
{

}

}

}