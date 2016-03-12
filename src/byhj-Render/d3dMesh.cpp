#include "d3dMesh.h"
#include <sstream>

namespace byhj {

	void D3DMesh::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd) 
	{
	    init_buffer(pD3D11Device);
		init_texture(pD3D11Device);
		init_shader(pD3D11Device, hWnd);
		init_state(pD3D11Device, pD3D11DeviceContext);
	}

	void D3DMesh::update()
	{
	}

	void D3DMesh::render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix)
	{
		// Bind appropriate textures
		int diffuseNr = 1;
		int specularNr = 1;
		for (int i = 0; i < m_Textures.size(); i++)
		{
			// Retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = m_Textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "texture_specular")
				ss << specularNr++; // Transfer GLuint to stream
			number = ss.str();

			pD3D11DeviceContext->PSSetShaderResources(i, 1, &m_Textures[i].id);
			// Now set the sampler to the correct texture unit
		}

		// Set vertex buffer stride and offset.=
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(Vertex);
		offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		cbMatrix.Model = matrix.model;
		cbMatrix.View  = matrix.view;
		cbMatrix.Porj  = matrix.proj;

		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &m_pMVPBuffer);

		pD3D11DeviceContext->RSSetState(m_pRasterState);

		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}

	void D3DMesh::shutdown()
	{
	}

	void D3DMesh::init_buffer(ID3D11Device *pD3D11Device)
	{
		HRESULT hr;
		///////////////////////////Vertex Buffer ////////////////////////////////
		m_VertexCount =m_VertexData.size();

		// Set up the description of the static vertex buffer.
		D3D11_BUFFER_DESC VertexBufferDesc;
		VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth           = sizeof(Vertex) * m_VertexCount;
		VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags      = 0;
		VertexBufferDesc.MiscFlags           = 0;
		VertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		D3D11_SUBRESOURCE_DATA VBO;
		VBO.pSysMem          = &m_VertexData[0];
		VBO.SysMemPitch      = 0;
		VBO.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
		DebugHR(hr);

		/////////////////////////////////Index Buffer ///////////////////////////////////////

		m_IndexCount = m_IndexData.size();

		// Set up the description of the static index buffer.
		D3D11_BUFFER_DESC IndexBufferDesc;
		IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_IndexCount;
		IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags      = 0;
		IndexBufferDesc.MiscFlags           = 0;
		IndexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		D3D11_SUBRESOURCE_DATA IBO;
		IBO.pSysMem          = &m_IndexData[0];
		IBO.SysMemPitch      = 0;
		IBO.SysMemSlicePitch = 0;

		hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
		DebugHR(hr);

		///////////////////////////////////////////////////////////////////
		D3D11_BUFFER_DESC mvpDesc;
		ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
		mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
		mvpDesc.ByteWidth      = sizeof(MatrixBuffer);
		mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		mvpDesc.CPUAccessFlags = 0;
		mvpDesc.MiscFlags      = 0;
		hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);
		DebugHR(hr);

	}
	void D3DMesh::init_state(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		HRESULT hr;
		//////////////////////Raterizer State/////////////////////////////
		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.FrontCounterClockwise = false;
		hr = pD3D11Device->CreateRasterizerState(&rasterDesc, &m_pRasterState);
		DebugHR(hr);
	}
}