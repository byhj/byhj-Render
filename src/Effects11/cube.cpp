#include "cube.h"

#include "geometry.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{

	Cube::Cube(){}

	Cube::~Cube(){}


	void Cube::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{
		init_buffer(pD3D11Device, pD3D11DeviceContext);
		init_shader(pD3D11Device, hWnd);
		init_texture(pD3D11Device);
		init_light();
		m_EffectHelper.init(pD3D11Device);
	}

	void Cube::render(ID3D11DeviceContext *pD3D11DeviceContext, D3DMVPMatrix matrix)
	{
		m_EffectHelper.render(pD3D11DeviceContext);

		// Set vertex buffer stride and offset
		unsigned int stride;
		unsigned int offset;
		stride = sizeof( Vertex );
		offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pCubeVB, &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(m_pCubeIB, DXGI_FORMAT_R32_UINT, 0);
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		cbMatrix.model = matrix.model;
		cbMatrix.view  = matrix.view;
		cbMatrix.proj  = matrix.proj;


		D3DX11_TECHNIQUE_DESC techDesc;
		ID3DX11EffectTechnique* activeTech = m_EffectHelper.m_pEffectLight1Tech;
		activeTech->GetDesc(&techDesc);

		for ( UINT p = 0; p<techDesc.Passes; ++p )
		{
			m_EffectHelper.setDiffuseTex(m_pDiffuseTexSRV);
			m_EffectHelper.setWorld(cbMatrix.model);
			m_EffectHelper.setView(cbMatrix.view);
			m_EffectHelper.setProj(cbMatrix.proj);
			// m_EffectHelper.setDirLight(m_DirLights);
			// m_EffectHelper.setMaterial(m_BoxMat);
			activeTech->GetPassByIndex(p)->Apply(0, pD3D11DeviceContext);
			pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);
		}

	}

	void Cube::shutdown()
	{
		m_EffectHelper.shutdown();
		ReleaseCOM(m_pCubeVB)
		ReleaseCOM(m_pCubeIB)
	}

	void Cube::init_light()
	{
		m_DirLights[0].Ambient  = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		m_DirLights[0].Diffuse  = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
		m_DirLights[0].Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
		m_DirLights[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

		m_DirLights[1].Ambient  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		m_DirLights[1].Diffuse  = XMFLOAT4(1.4f, 1.4f, 1.4f, 1.0f);
		m_DirLights[1].Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
		m_DirLights[1].Direction = XMFLOAT3(-0.707f, 0.0f, 0.707f);

		m_BoxMat.Ambient  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		m_BoxMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_BoxMat.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
	}


	void Cube::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{

		init_light();

		HRESULT hr;
		D3DMeshData box;
		Geometry geoGen;
		geoGen.createCube(0.1f, 0.1f, 0.1f, box);

		m_IndexCount  = box.IndexData.size();
		m_VertexCount = box.VertexData.size();

		D3D11_BUFFER_DESC cubeVBDesc;
		cubeVBDesc.Usage               = D3D11_USAGE_IMMUTABLE;
		cubeVBDesc.ByteWidth           = sizeof( D3DVertex )* m_VertexCount;
		cubeVBDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
		cubeVBDesc.CPUAccessFlags      = 0;
		cubeVBDesc.MiscFlags           = 0;
		cubeVBDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA shapeVBO;
		shapeVBO.pSysMem = &box.VertexData[0];
		hr = pD3D11Device->CreateBuffer(&cubeVBDesc, &shapeVBO, &m_pCubeVB);
		//DebugHR(hr);


		D3D11_BUFFER_DESC cubeIBDesc;
		cubeIBDesc.Usage               = D3D11_USAGE_IMMUTABLE;
		cubeIBDesc.ByteWidth           = sizeof( UINT )* m_IndexCount;
		cubeIBDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
		cubeIBDesc.CPUAccessFlags      = 0;
		cubeIBDesc.MiscFlags           = 0;
		cubeIBDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA shapeIBO;
		shapeIBO.pSysMem = &box.IndexData[0];
		hr = pD3D11Device->CreateBuffer(&cubeIBDesc, &shapeIBO, &m_pCubeIB);
		//DebugHR(hr);

	}

void Cube::init_texture(ID3D11Device *pD3D11Device)
{
	HRESULT hr;
	hr = CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/WoodCrate01.dds", 0,  &m_pDiffuseTexSRV);
	//DebugHR(hr);
}

void Cube::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{


}


}