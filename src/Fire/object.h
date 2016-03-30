#ifndef OBJECT_H
#define OBJECT_H

#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

namespace byhj
{


class Object
{
public:
	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeivceContext, HWND hWnd);
	void update();
	void render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix);
	void shutdown();


	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);
private:
	bool loadModel(char *modelFile);

	D3DMVPMatrix cbMatrix;

	struct DistortionBuffer
	{    
		XMFLOAT2 distortion1;
		XMFLOAT2 distortion2;
		XMFLOAT2 distortion3;
		FLOAT    distortionScale;
		FLOAT    distortionBias;
	};
	DistortionBuffer cbDistortion;

	struct NoiseBuffer
	{
		FLOAT   frameTime;
		XMFLOAT3 scrollSpeeds;
		XMFLOAT3 scales;
		FLOAT    padding;
	};
	NoiseBuffer cbNoise;

	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};
	struct ModelVertex
	{
		float x, y , z;
		float u, v;
		float nx, ny, nz;
	};
	ModelVertex  *m_pModelVertex;

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer>  m_pNoiseBuffer;
	ComPtr<ID3D11Buffer>  m_pDistortBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState1;
	ComPtr<ID3D11InputLayout> m_pInputLayout;
	ID3D11ShaderResourceView *m_pTextures[3];

	int m_VertexCount;
	int m_IndexCount;

	D3DShader ObjectShader;
};

}
#endif