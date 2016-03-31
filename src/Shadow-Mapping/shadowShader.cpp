#include "depthShader.h"

void DepthShader::Init(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[3];
	pInputLayoutDesc[0].SemanticName         = "POSITION";
	pInputLayoutDesc[0].SemanticIndex        = 0;
	pInputLayoutDesc[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[0].InputSlot            = 0;
	pInputLayoutDesc[0].AlignedByteOffset    = 0;
	pInputLayoutDesc[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[0].InstanceDataStepRate = 0;

	pInputLayoutDesc[1].SemanticName         = "TEXCOORD";
	pInputLayoutDesc[1].SemanticIndex        = 0;
	pInputLayoutDesc[1].Format               = DXGI_FORMAT_R32G32_FLOAT;
	pInputLayoutDesc[1].InputSlot            = 0;
	pInputLayoutDesc[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[1].InstanceDataStepRate = 0;

	pInputLayoutDesc[2].SemanticName         = "NORMAL";
	pInputLayoutDesc[2].SemanticIndex        = 0;
	pInputLayoutDesc[2].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[2].InputSlot            = 0;
	pInputLayoutDesc[2].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[2].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[2].InstanceDataStepRate = 0;

	unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

	shadowShader.init(pD3D11Device, hWnd);
	shadowShader.attachVS(L"shadow.vsh", pInputLayoutDesc, numElements);
	shadowShader.attachPS(L"shadow.psh");
	shadowShader.end();
}

void DepthShader::Use(ID3D11DeviceContext *pD3D11DeviceContext)

{
	shadowShader.use(pD3D11DeviceContext);
}