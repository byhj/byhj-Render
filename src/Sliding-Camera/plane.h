#include "d3d/d3dShader.h"
#include "d3d/d3dDebug.h"
#include <d3d11.h>
#include <DirectXMath.h> using namespace DirectX;
#include <vector>

namespace byhj
{

class Plane
{
public:
	Plane()
	{
		m_pRenderTargetView   = NULL;
		m_pDepthStencilView   = NULL;
		m_pDepthStencilBuffer = NULL;
		m_pMVPBuffer          = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
		m_pTexture            = NULL;
		m_pTexSamplerState    = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, XMFLOAT4X4 model, XMFLOAT4X4  view, XMFLOAT4X4  proj);

	void Shutdown()
	{
		ReleaseCOM(m_pRenderTargetView  )
		ReleaseCOM(m_pDepthStencilView  )
		ReleaseCOM(m_pMVPBuffer         )
		ReleaseCOM(m_pDepthStencilBuffer)
		ReleaseCOM(m_pVertexBuffer      )
		ReleaseCOM(m_pIndexBuffer       )
		  
		
	}
	int NumFaces;
	int NumVertices;

	struct HeightMapInfo
	{                           // Heightmap structure
		int terrainWidth;		// Width of heightmap
		int terrainHeight;		// Height (Length) of heightmap
		XMFLOAT3 *heightMap;	// Array to store terrain's vertex positions
	};

public:
	bool HeightMapLoad(char* filename, HeightMapInfo &hminfo);
	bool init_buffer(ID3D11Device *pD3D11Device);
	bool init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile);

private:

	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex(){}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x,y,z), texCoord(u, v), normal(nx, ny, nz){}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};

	struct MatrixBuffer
	{
		XMFLOAT4X4 model;
		XMFLOAT4X4 view;
		XMFLOAT4X4 proj;
	};
	MatrixBuffer cbMatrix;

	ID3D11RenderTargetView   *m_pRenderTargetView;
	ID3D11DepthStencilView   *m_pDepthStencilView;
	ID3D11Texture2D          *m_pDepthStencilBuffer;
	ID3D11RasterizerState    *m_pRasterState;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	int m_VertexCount = 0;
	int m_IndexCount = 0;

	d3d::Shader TestShader;
};


}