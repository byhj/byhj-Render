#ifndef Cube_H
#define Cube_H

#include <windows.h>

#include <vector>

#include "d3d/D3DShader.h"
#include "d3d/D3DUtility.h"


namespace byhj
{


class Cube
{
public:
	Cube();
	~Cube();

	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix);
	void shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};
	struct ModelVertex
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
	};
	ModelVertex  *m_pModelVertex;
	int m_VertexCount;
	int m_IndexCount;
	ModelType  *pModelVertex;

	bool LoadModel(char *modelFile);
	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
	LightBuffer cbLight;

	struct CameraBuffer
	{
		XMFLOAT3 camPos;
		float padding;
	};

	D3DMVPMatrix cbMatrix;
	ComPtr<ID3D11InputLayout> m_pInputLayout;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pLightBuffer;
	ComPtr<ID3D11Buffer> m_CameraBuffer;
	ID3D11ShaderResourceView *m_pTextures[2];
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	D3DShader CubeShader;
};


}


#endif 