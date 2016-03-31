#ifndef WALL_H
#define WALL_H

#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

namespace byhj
{


class Wall
{
public:
	Wall() = default;
	~Wall() = default;

	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContect);
	void update();
	void render(ID3D11DeviceContext *pD3D11DeviceContext);
	void shutdown();


private:

	bool load_model(char *modelFile);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);

	struct  Vertex
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

	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;


	int m_VertexCount = 0;
	int m_IndexCount  = 0;

	D3DShader WallShader;
};



}

#endif