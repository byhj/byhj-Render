#ifndef Mesh_H
#define Mesh_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <d3d11.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <windows.h>
#include <DirectXMath.h>

#include "d3d/D3DUtility.h"

using namespace DirectX;

namespace byhj {

class D3DMesh {
public:
	struct Material
	{
		Material()
		{
			ambient   = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			diffuse   = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			specular  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			emissive  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			// shininess = 32.0f;
		}
		Material (const Material &mat)
		{
			this->ambient   = mat.ambient;
			this->diffuse   = mat.diffuse;
			this->specular  = mat.specular;
			this->emissive  = mat.emissive;
			//this->shininess = mat.shininess;
		}
		Material &operator = (const Material &mat)
		{
			this->ambient   = mat.ambient;
			this->diffuse   = mat.diffuse;
			this->specular  = mat.specular;
			this->emissive  = mat.emissive;
			//this->shininess = mat.shininess;

			return *this;
		}

		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT4 emissive;
		//float shininess;
	};

	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 texcoord;
	};

	struct Texture {
		ID3D11ShaderResourceView *pTextureSRV;
		std::string type;
		std::string name;
	};

public:

	D3DMesh() = default;
	~D3DMesh() = default;
	D3DMesh(const std::vector<Vertex> &vertices, const std::vector<UINT> &indices, 
		    const std::vector<Texture> textures, const Material &mat)
	{
		m_VertexData = vertices;
		m_IndexData  = indices;
		m_Textures  = textures;
		m_material = mat;
	}

	// Initializes all the buffer objects/arrays
	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void update();
	void render(ID3D11DeviceContext *pD3D11DeviceContext);
    void shutdown();
	bool isBlend() {
	   if (m_material.ambient.w < 1.0f) {
	      return true;
	   }else {
		   return false;
	   }
	}
private:
	void init_state(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_buffer(ID3D11Device *pD3D11Device);
	void init_texture(ID3D11Device *pD3D11Device);


	struct MaterialBuffer
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT4 emissive;
		//float   shininess;
	};

	std::vector<Vertex> m_VertexData;
	std::vector<UINT> m_IndexData;
	std::vector<Texture> m_Textures;
	Material m_material;

	ID3D11Buffer *m_pIndexBuffer;
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pMaterialBuffer;

	UINT m_VertexCount;
	UINT m_IndexCount;
	ID3D11ShaderResourceView *m_pShaderResourceView;
};

}

#endif