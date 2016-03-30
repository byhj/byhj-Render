
#include "d3d/D3DShader.h"
#include "d3d/d3dUtility.h"

namespace byhj
{


class Plane
{
public:
	Plane();
	~Plane();

	void render(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11ShaderResourceView *pPlaneTexSRV, const D3DMVPMatrix &matrix, XMFLOAT4X4 Plane);
	void shutdown();

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

	struct ReflectMatrixBuffer
	{
		XMFLOAT4X4  model;
		XMFLOAT4X4  view;
		XMFLOAT4X4  proj;
		XMFLOAT4X4  PlaneMat;

	};
	ReflectMatrixBuffer cbMatrix;

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

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pLightBuffer;
	ComPtr<ID3D11Buffer> m_CameraBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	ComPtr<ID3D11InputLayout> m_pInputLayout;

	int m_VertexCount;
	int m_IndexCount;
	D3DShader PlaneShader;
};


}