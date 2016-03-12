#include "Terrain.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{

	void Terrain::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{
		init_buffer(pD3D11Device, pD3D11DeviceContext);
		init_shader(pD3D11Device, hWnd);
		init_texture(pD3D11Device);
	}

	void Terrain::Render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix)
	{
		//Update the the mvp matrix
		m_matrix.model = matrix.model;
		m_matrix.view  = matrix.view;
		m_matrix.proj  = matrix.proj;
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &m_matrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &m_pMVPBuffer);
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &m_pLightBuffer);

		// Set vertex buffer stride and offset
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(Vertex);
		offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pTerrainVB, &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(m_pTerrainIB, DXGI_FORMAT_R32_UINT, 0);

		pD3D11DeviceContext->PSSetShaderResources(0, 1, &m_pGrassTexSRV);
		pD3D11DeviceContext->PSSetShaderResources(1, 1, &m_pSlopeTexSRV);
		pD3D11DeviceContext->PSSetShaderResources(2, 1, &m_pRockTexSRV);
		pD3D11DeviceContext->PSSetSamplers(0, 1, &m_pTexSamplerState);

		TerrainShader.use(pD3D11DeviceContext);
		pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

	}

	void Terrain::Shutdown()
	{
		ReleaseCOM(m_pTerrainVB)
		ReleaseCOM(m_pTerrainIB)
		ReleaseCOM(m_pMVPBuffer)
		ReleaseCOM(m_pInputLayout)
	}

	void Terrain::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		HRESULT hr;
		auto dir = TextureMgr::getInstance()->getDir() + "heightmap.bmp";

		load_heightMap(dir.c_str());

		Geometry geom;
		D3DMeshData TerrainMesh;
		geom.createGrid(160.0, 160.0, m_TerrainWidth, m_TerrainHeight, TerrainMesh);

		m_VertexCount = TerrainMesh.VertexData.size();
		for (int i = 0; i != m_VertexCount; ++i)
			TerrainMesh.VertexData[i].position.y = m_HightmapData[i].y / 10.0f;

		calc_normal(TerrainMesh);

		/////////////////////////////Vertex Buffer//////////////////////////////
		m_VertexCount = TerrainMesh.VertexData.size();
		D3D11_BUFFER_DESC TerrainVBDesc;
		TerrainVBDesc.Usage               = D3D11_USAGE_IMMUTABLE;
		TerrainVBDesc.ByteWidth           = sizeof(Vertex) * m_VertexCount;
		TerrainVBDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
		TerrainVBDesc.CPUAccessFlags      = 0;
		TerrainVBDesc.MiscFlags           = 0;
		TerrainVBDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA TerrainVBO;
		TerrainVBO.pSysMem = &TerrainMesh.VertexData[0];
		hr = pD3D11Device->CreateBuffer(&TerrainVBDesc, &TerrainVBO, &m_pTerrainVB);
		DebugHR(hr);

		/////////////////////////////Index Buffer//////////////////////////////

		m_IndexCount =  TerrainMesh.IndexData.size();
		D3D11_BUFFER_DESC TerrainIBDesc;
		TerrainIBDesc.Usage               = D3D11_USAGE_IMMUTABLE;
		TerrainIBDesc.ByteWidth           = sizeof(UINT) * m_IndexCount;
		TerrainIBDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
		TerrainIBDesc.CPUAccessFlags      = 0;
		TerrainIBDesc.MiscFlags           = 0;
		TerrainIBDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA girdIBO;
		girdIBO.pSysMem = &TerrainMesh.IndexData[0];
		hr = pD3D11Device->CreateBuffer(&TerrainIBDesc, &girdIBO, &m_pTerrainIB);
		DebugHR(hr);

		////////////////////////////////Const Buffer//////////////////////////////////////

		D3D11_BUFFER_DESC mvpDesc;
		ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
		mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
		mvpDesc.ByteWidth      = sizeof(D3DMVPMatrix);
		mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		mvpDesc.CPUAccessFlags = 0;
		mvpDesc.MiscFlags      = 0;
		hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);
		DebugHR(hr);



		D3D11_BUFFER_DESC lightDesc;
		ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
		lightDesc.Usage          = D3D11_USAGE_IMMUTABLE;
		lightDesc.ByteWidth      = sizeof(LightBuffer);
		lightDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		lightDesc.CPUAccessFlags = 0;
		lightDesc.MiscFlags      = 0;

		cbLight.ambient  = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
		cbLight.diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		cbLight.lightDir = XMFLOAT3(-0.5f, -1.0f, 0.0f);
		cbLight.pad      = 0.0f;

		D3D11_SUBRESOURCE_DATA lightVBO;
		lightVBO.pSysMem = &cbLight;
		hr = pD3D11Device->CreateBuffer(&lightDesc, &lightVBO, &m_pLightBuffer);
		DebugHR(hr);

	}


	void Terrain::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
	{
		//Shader interface information
		std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;
		D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc;

		pInputLayoutDesc.SemanticName         = "POSITION";
		pInputLayoutDesc.SemanticIndex        = 0;
		pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc.InputSlot            = 0;
		pInputLayoutDesc.AlignedByteOffset    = 0;
		pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		pInputLayoutDesc.SemanticName         = "NORMAL";
		pInputLayoutDesc.SemanticIndex        = 0;
		pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc.InputSlot            = 0;
		pInputLayoutDesc.AlignedByteOffset    = 12;
		pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		pInputLayoutDesc.SemanticName         = "TEXCOORD";
		pInputLayoutDesc.SemanticIndex        = 0;
		pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32_FLOAT;
		pInputLayoutDesc.InputSlot            = 0;
		pInputLayoutDesc.AlignedByteOffset    = 24;
		pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		TerrainShader.init(pD3D11Device, vInputLayoutDesc);
		TerrainShader.attach(D3D_VERTEX_SHADER, L"Terrain.vsh", "TerrainVS", "vs_5_0");
		TerrainShader.attach(D3D_PIXEL_SHADER,  L"Terrain.psh", "TerrainPS", "ps_5_0");
		TerrainShader.end();
	}
	
	void Terrain::init_texture(ID3D11Device *pD3D11Device)
	{
		m_pGrassTexSRV = TextureMgr::getInstance()->loadD3DTexture(pD3D11Device, "grass.dds");
		m_pSlopeTexSRV = TextureMgr::getInstance()->loadD3DTexture(pD3D11Device, "slope.dds");
		m_pRockTexSRV  = TextureMgr::getInstance()->loadD3DTexture(pD3D11Device,  "rock.dds");

		// Create a texture sampler state description.
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		
		HRESULT hr;
		// Create the texture sampler state.
		hr = pD3D11Device->CreateSamplerState(&samplerDesc, &m_pTexSamplerState);
		DebugHR(hr);
	}

	void Terrain::load_heightMap(const char *filename)
	{
		BITMAPFILEHEADER  bitmapFileHeader;
		BITMAPINFOHEADER  bitmapInfoHeader;
		int imageSize;
		unsigned char *bitmapImage;
		unsigned char height;

		FILE *fp;
		int result =  fopen_s(&fp, filename, "rb");

		fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
		fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

		m_TerrainWidth  = bitmapInfoHeader.biWidth;
		m_TerrainHeight = bitmapInfoHeader.biHeight;

		// Calculate the size of the bitmap image data.
		imageSize = m_TerrainWidth * m_TerrainHeight * 3;

		// Allocate memory for the bitmap image data.
		bitmapImage = new unsigned char[imageSize];
		// Move to the beginning of the bitmap data.
		fseek(fp, bitmapFileHeader.bfOffBits, SEEK_SET);

		// Read in the bitmap image data.
		fread(bitmapImage, 1, imageSize, fp);
		fclose(fp);

		///////////////////////////////////////////////////////////////
		int k = 0, index;
		// Read the image data into the height map.
		for (int j = 0; j != m_TerrainHeight; ++j)
		{
			for (int i = 0; i != m_TerrainWidth; ++i)
			{
				height = bitmapImage[k];

				XMFLOAT3 Pos;
				Pos.x = (float)i;
				Pos.y = (float)height;
				Pos.z = (float)j;
				m_HightmapData.push_back(Pos);

				k += 3;
			}
		}

		// Release the bitmap image data.
		delete[] bitmapImage;
		bitmapImage = 0;
	}

	void Terrain::calc_normal(D3DMeshData &mesh)
	{
		for (int i = 0; i != mesh.IndexData.size(); i += 3)
		{
			int index1 = mesh.IndexData[i];
			int index2 = mesh.IndexData[i + 1];
			int index3 = mesh.IndexData[i + 2];
			XMFLOAT3 pos1 = mesh.VertexData[index1].position;
			XMFLOAT3 pos2 = mesh.VertexData[index2].position;
			XMFLOAT3 pos3 = mesh.VertexData[index3].position;

			XMVECTOR v1 = XMLoadFloat3(&pos1);
			XMVECTOR v2 = XMLoadFloat3(&pos2);
			XMVECTOR v3 = XMLoadFloat3(&pos3);

			XMVECTOR edge1 = v1 - v2;
			XMVECTOR edge2 = v2 - v3;

			XMVECTOR n1 = XMVector3Cross(edge1, edge2);
			XMVECTOR normal = XMVector3Normalize(n1);
			XMStoreFloat3(&mesh.VertexData[index1].normal, normal);
			XMStoreFloat3(&mesh.VertexData[index2].normal, normal);
			XMStoreFloat3(&mesh.VertexData[index3].normal, normal);
		}

		for (int i = 0; i != mesh.VertexData.size(); ++i)
		{
			XMVECTOR n = XMLoadFloat3(&mesh.VertexData[i].normal);
			XMVECTOR normal = XMVector3Normalize(n);
			XMStoreFloat3(&mesh.VertexData[i].normal, normal);
		}

	}

}


