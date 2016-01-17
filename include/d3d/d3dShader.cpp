#include "d3dShader.h"

#include <d3dcompiler.h>
#include "d3d/Utility.h"

namespace byhj
{

void D3DShader::init(ID3D11Device *pD3D11Device, const std::vector<D3D11_INPUT_ELEMENT_DESC> &vInputDesc)
{
	this->pD3D11Device = pD3D11Device;
	this->vInputLayoutDesc = vInputDesc;

}

HRESULT D3DShader::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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


void D3DShader::attach(ShaderType shaderType, WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{	
	HRESULT result;
	ID3DBlob* pShaderBlob = nullptr;

	CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel, &pShaderBlob);
	
	if (shaderType == D3D_VERTEX_SHADER)
	{

	result = pD3D11Device->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, &pVS_Shader);

	auto numElements = vInputLayoutDesc.size();
	result = pD3D11Device->CreateInputLayout(&vInputLayoutDesc[0], numElements, pShaderBlob->GetBufferPointer(), 
	                                     	pShaderBlob->GetBufferSize(), &pInputLayout);

	} else if (shaderType == D3D_PIXEL_SHADER) {
		result = pD3D11Device->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, &pPS_Shader);

	} else if (shaderType == D3D_GEOMETRY_SHADER) {
	   result = pD3D11Device->CreateGeometryShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, &pGS_Shader);
	
	} else if (shaderType == D3D_HULL_SHADER) {
		result = pD3D11Device->CreateHullShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, &pHS_Shader);
	
	} else if (shaderType == D3D_GEOMETRY_SHADER) {
	
		result = pD3D11Device->CreateDomainShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, &pDS_Shader);
	} 

	// Release the shader buffer since they are no longer needed.
	pShaderBlob->Release();
	pShaderBlob = 0;

}


void D3DShader::use(ID3D11DeviceContext *pD3D11DeviceContext)
{
	pD3D11DeviceContext->IASetInputLayout(pInputLayout.Get());

	if (pVS_Shader != nullptr) {
		pD3D11DeviceContext->VSSetShader(pVS_Shader.Get(), NULL, 0);
    }
	if (pHS_Shader  != nullptr) {						   
		pD3D11DeviceContext->HSSetShader(pHS_Shader.Get(), NULL, 0);
	}
	if (pDS_Shader  != nullptr) {					
		pD3D11DeviceContext->DSSetShader(pDS_Shader.Get(), NULL, 0);
	}
	if (pGS_Shader != nullptr) {					
		pD3D11DeviceContext->GSSetShader(pGS_Shader.Get(), NULL, 0);
	}
	if (pVS_Shader  != nullptr) {					  
		pD3D11DeviceContext->PSSetShader(pPS_Shader.Get(), NULL, 0);
	}
}

void D3DShader::end()
{

}


}