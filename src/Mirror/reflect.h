#include "d3d/d3dApp.h"
#include "d3d/d3dShader.h"
#include "d3d/D3DUtility.h"

#include "textureMgr.h"

namespace byhj {

class Reflect
{
public:
	Reflect();
	~Reflect();

	void Render(ID3D11DeviceContext *pD3D11DeviceContext,ID3D11ShaderResourceView *pReflectTexSRV, const XMFLOAT4X4 &Model,  
		const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj, XMFLOAT4X4 reflect)
	{
		cbMatrix.model   = Model;
		cbMatrix.view    = View;
		cbMatrix.proj    = Proj;
		cbMatrix.reflect = reflect;

		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0 );
		pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, &m_pMVPBuffer);

		unsigned int stride;
		unsigned int offset;
		stride = sizeof(Vertex); 
		offset = 0;

		pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pD3D11DeviceContext->PSSetShaderResources(0, 1, &m_pTexture);  
		pD3D11DeviceContext->PSSetShaderResources(1, 1, &pReflectTexSRV);  
		pD3D11DeviceContext->PSSetSamplers( 0, 1, &m_pTexSamplerState );

		ReflectShader.use(pD3D11DeviceContext);
		pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}

	void shutdown()
	{
		ReleaseCOM(m_pRenderTargetView  )
		ReleaseCOM(m_pMVPBuffer         )
		ReleaseCOM(m_pLightBuffer       )
		ReleaseCOM(m_pVertexBuffer      )
		ReleaseCOM(m_pIndexBuffer       )
	}

	bool LoadModel(char *modelFile);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile, ID3D11ShaderResourceView *m_pTexture);

private:

	struct CameraBuffer
	{
		XMFLOAT3 camPos;
		float padding;
	};

	struct MatrixBuffer
	{
		XMFLOAT4X4  model;
		XMFLOAT4X4  view;
		XMFLOAT4X4  proj;
		XMFLOAT4X4  reflect;

	};
	MatrixBuffer cbMatrix;

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
	LightBuffer cbLight;


	struct ModelVertex
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	ModelVertex  *pModelVertex;

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Normal;
	};

	ID3D11RenderTargetView   *m_pRenderTargetView;
	ID3D11Buffer             *m_pMVPBuffer;
	ID3D11Buffer             *m_pLightBuffer;
	ID3D11Buffer             *m_CameraBuffer;
	ID3D11Buffer             *m_pVertexBuffer;
	ID3D11Buffer             *m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTexture;
	ID3D11SamplerState       *m_pTexSamplerState;
	ID3D11InputLayout        *m_pInputLayout;

	int m_VertexCount;
	int m_IndexCount;
	D3DShader ReflectShader;
};

}