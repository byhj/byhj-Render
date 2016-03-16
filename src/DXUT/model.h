#ifndef Cube_H
#define Cube_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "SDKmesh.h"

#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

using namespace DirectX;

namespace byhj
{

class Model
{

public:
	Model();
	~Model();

public:
	void update();
	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix);
	void shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device);
	void init_texture(ID3D11Device *pD3D11Device);

	struct LightBuffer
	{
		XMFLOAT4 lightPos;
	};

	ComPtr<ID3D11Buffer      >  m_pMVPBuffer         = nullptr;
	ComPtr<ID3D11Buffer      >  m_pLightBuffer       = nullptr;
	ComPtr<ID3D11SamplerState>  m_pSamplerLinear     = nullptr;

	CDXUTSDKMesh   m_SdkMesh;
	D3DMVPMatrix cbMatrix;
	D3DShader CubeShader;
};


}

#endif