#ifndef Model_H
#define Model_H

#include "mesh.h"

#include <vector>
#include <SOIL.h>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "textureMgr.h"

namespace byhj
{
	enum LoadType {
		OGL,
		D3D
	};

	class Model {
	public:
		Model() = default;
		~Model() = default;

		void draw(GLuint program);
		void loadModel(std::string fileName, LoadType loadType);
		void processNode(aiNode *node, const aiScene *scene);
		void processMesh(aiMesh *mesh, const aiScene *scene, OGLMesh &oglMesh);
	//	void processMesh(aiMesh *mesh, const aiScene *scene, d3d::Mesh &d3dMesh);

		std::vector<OGLMesh::Texture> loadOGLTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	//	std::vector<d3d::Texture> loadD3DTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	private:

		std::vector<OGLMesh> m_OGLMeshes;
		std::vector<OGLMesh::Texture> m_OGLTextures;	

		LoadType m_LoadType;
		std::string m_dir;
	};
}
#endif
