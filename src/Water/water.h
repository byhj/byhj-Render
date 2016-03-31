#ifndef WATER_H
#define WATER_H
 
#include "d3d/d3dShader.h"

namespace byhj
{


class Water
{
public:
	Water() = default;
	~Water() = default;

	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void update();
	void render(ID3D11DeviceContext *pD3D11DeviceContext);
	void shutdown();

	bool LoadModel(char *modelFile);

private:

	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);

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

	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	int m_VertexCount;
	int m_IndexCount;
};



}

#endif