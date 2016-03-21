#ifndef D3DModel_H
#define D3DModel_H

#include "d3dMesh.h"

#include <vector>
#include <SOIL.h>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "textureMgr.h"

#pragma comment(lib, "assimp")

namespace byhj
{

	class D3DModel {
	public:
		D3DModel() = default;
		~D3DModel() = default;

		void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
		void render(ID3D11DeviceContext *pD3D11DeviceContext);
		
		void loadModel(std::string fileName);
		void processNode(aiNode *node, const aiScene *scene);
		void processMesh(aiMesh *mesh, const aiScene *scene, D3DMesh &d3dMesh);

		std::vector<D3DMesh::Texture> loadD3DTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


	private:
		std::vector<D3DMesh> m_D3DMeshes;
		std::string m_dir;
	};
}
#endif
