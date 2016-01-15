#ifndef D3DSHADER_H
#define D3DSHADER_H

#include <d3d11.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace byhj
{

namespace d3d
{

class Shader
{
public:
	Shader(): pVS_Shader(0), pPS_Shader(0), pHS_Shader(0), pDS_Shader(0),
		      pGS_Shader(0), pCS_Shader(0), pInputLayout(0), pD3D11Device(0) {}
	~Shader() {}

	void init(ID3D11Device *pD3D11Device, const std::vector<D3D11_INPUT_ELEMENT_DESC> &vInputDesc);

	void attachVS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);
	void attachHS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);
	void attachDS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);
	void attachGS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);
	void attachCS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);
	void attachPS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);
	void use(ID3D11DeviceContext *pD3D111DeviceContext);
	void end();

private:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	ComPtr<ID3D11VertexShader  > pVS_Shader;
	ComPtr<ID3D11HullShader    > pHS_Shader;
	ComPtr<ID3D11DomainShader  > pDS_Shader;
	ComPtr<ID3D11GeometryShader> pGS_Shader;
	ComPtr<ID3D11ComputeShader > pCS_Shader;
	ComPtr<ID3D11PixelShader   > pPS_Shader;

	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;
	ComPtr<ID3D11InputLayout>  pInputLayout;
	ComPtr<ID3D11Device>       pD3D11Device;
};


}
}

#endif