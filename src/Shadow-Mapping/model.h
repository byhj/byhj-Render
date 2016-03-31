#ifndef OBJECT_H
#define OBJECT_H

#include "d3d/App.h"
#include "d3d/Shader.h"

namespace byhj
{


class Model
{
public:
	Model()
	{

	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext);

	void Shutdown()
	{
	}

	bool load_model(char *modelFile);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);

private:
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

	int m_VertexCount;
	int m_IndexCount;
	std::vector<Vertex> m_VertexData;
	std::vector<DWORD> m_IndexData;


};

}
#endif