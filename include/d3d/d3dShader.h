#ifndef D3DSHADER_H
#define D3DSHADER_H

#include <d3d11.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <wrl.h>
using namespace Microsoft::WRL;

#include "shader.h"


namespace byhj
{
	enum  ShaderType{
	  D3D_VERTEX_SHADER,
	  D3D_HULL_SHADER,
	  D3D_DOMAIN_SHADER,
	  D3D_GEOMETRY_SHADER,
	  D3D_COMPUTE_SHADER,
	  D3D_PIXEL_SHADER
	};

class D3DShader : public Shader
{
public:
	D3DShader()  = default;
	~D3DShader() = default;

	void init(ID3D11Device *pD3D11Device, const std::vector<D3D11_INPUT_ELEMENT_DESC> &vInputDesc);
	void attach(ShaderType shaderType, WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);
	void use(ID3D11DeviceContext *pD3D111DeviceContext);
	void end();

private:

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	ComPtr<ID3D11VertexShader  > pVS_Shader = nullptr;
	ComPtr<ID3D11HullShader    > pHS_Shader = nullptr;
	ComPtr<ID3D11DomainShader  > pDS_Shader = nullptr;
	ComPtr<ID3D11GeometryShader> pGS_Shader = nullptr;
	ComPtr<ID3D11ComputeShader > pCS_Shader = nullptr;
	ComPtr<ID3D11PixelShader   > pPS_Shader = nullptr;

	ComPtr<ID3D11InputLayout>  pInputLayout = nullptr;
	ComPtr<ID3D11Device>       pD3D11Device = nullptr;

	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;

};


}

#endif