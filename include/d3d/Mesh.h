#ifndef D3DMESH_H
#define D3DMESH_H


#include <d3d11.h>
#include <D3DX11.h>
#include <windows.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <D3DX10math.h>

#include "d3d/Shader.h"
#include "d3d/d3dDebug.h"

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


	
struct Vertex {
	// Position
	XMFLOAT3 Position;
	// Normal
	XMFLOAT3 Normal;
	// TexCoords
	XMFLOAT2 TexCoords;

	XMFLOAT3 Tangent;

	XMFLOAT3 BiTangent;
};



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
		    this->ambient   = mat.ambient  ;
			this->diffuse   = mat.diffuse  ;
			this->specular  = mat.specular ;
			this->emissive  = mat.emissive ;
			//this->shininess = mat.shininess;
	}
	Material &operator = (const Material &mat)
	{
		this->ambient   = mat.ambient  ;
		this->diffuse   = mat.diffuse  ;
		this->specular  = mat.specular ;
		this->emissive  = mat.emissive ;
		//this->shininess = mat.shininess;

		return *this;
	}

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT4 emissive;
	//float shininess;
};



struct Texture {
	Texture()
	{
		pTexture      = NULL;
	}

	ID3D11ShaderResourceView *pTexture;
	std::string type;
	aiString path;
};

namespace byhj
{
	namespace d3d
	{

		class Mesh {
		public:
			/*  Mesh Data  */
			std::vector<Vertex> VertexData;
			std::vector<unsigned long> IndexData;
			std::vector<Texture> textures;
			Material mat;

			// Constructor
			Mesh(std::vector<Vertex> vertices, std::vector<unsigned long> indices, std::vector<Texture> textures, Material mat,
				ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
			{
				this->VertexData = vertices;
				this->IndexData = indices;
				this->textures = textures;
				this->mat = mat;
				// Now that we have all the required data, set the vertex buffers and its attribute pointers.
				this->init_Mesh(pD3D11Device, pD3D11DeviceContext, hWnd);
			}

		public:
			// Initializes all the buffer objects/arrays
			void init_Mesh(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
			void init_state(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
			void init_buffer(ID3D11Device *pD3D11Device);
			void load_texture(ID3D11Device *pD3D11Device, WCHAR *texFile);
			void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
			void Render(ID3D11DeviceContext *pD3D11DeviceContext, XMFLOAT4X4 model, XMFLOAT4X4 view, XMFLOAT4X4 proj);

			struct MatrixBuffer
			{
				XMFLOAT4X4 Model;
				XMFLOAT4X4 View;
				XMFLOAT4X4 Porj;
			};
			MatrixBuffer cbMatrix;

			struct MaterialBuffer
			{
				XMFLOAT4 ambient;
				XMFLOAT4 diffuse;
				XMFLOAT4 specular;
				XMFLOAT4 emissive;
				float   shininess;
			};

			ID3D11Buffer *m_pIndexBuffer;
			ID3D11Buffer *m_pVertexBuffer;
			ID3D11Buffer *m_pMVPBuffer;

			int m_VertexCount;
			int m_IndexCount;

			ID3D11ShaderResourceView *m_pShaderResourceView;
			ID3D11DepthStencilView   *m_pDepthStencilView;
			ID3D11Texture2D          *m_pDepthStencilBuffer;
			ID3D11RasterizerState    *m_pRasterState;
			ID3D11SamplerState       *m_pTexSamplerState;

		};

	}
}

#endif