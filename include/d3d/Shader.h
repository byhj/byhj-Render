#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <D3DX11async.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

	void init(ID3D11Device *pD3D11Device, HWND hWnd);
	bool attachVS(WCHAR* Filename, std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc);
	bool attachHS(WCHAR* Filename);
	bool attachDS(WCHAR* Filename);
	bool attachGS(WCHAR* Filename);
	bool attachCS(WCHAR* Filename);
	bool attachPS(WCHAR* Filename);
	void use(ID3D11DeviceContext *pD3D111DeviceContext);
	void Debug(ID3D10Blob *pErrorMessage, HWND hwnd, WCHAR *shaderFileName);
	void end();

private:
	ID3D11VertexShader    *pVS_Shader;
	ID3D11HullShader      *pHS_Shader;
	ID3D11DomainShader    *pDS_Shader;
	ID3D11GeometryShader  *pGS_Shader;
	ID3D11ComputeShader   *pCS_Shader;
	ID3D11PixelShader     *pPS_Shader;

	ID3D11InputLayout  *pInputLayout;
	ID3D11Device *pD3D11Device;
	HWND hWnd;
};


}

}

#endif