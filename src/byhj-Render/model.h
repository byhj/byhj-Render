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
	class Model {
	public:
		Model() = default;
		~Model() = default;

		void loadModel(std::string fileName, LoadType loadType);
		void processNode(aiNode *node, const aiScene *scene);
		void processMesh(aiMesh *mesh, const aiScene *scene, ogl::Mesh &oglMesh);
		void processMesh(aiMesh *mesh, const aiScene *scene, d3d::Mesh &d3dMesh);

		std::vector<ogl::Texture> loadOGLTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		std::vector<d3d::Texture> loadD3DTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	private:
		std::vector<d3d::Mesh> m_D3DMeshes;
		std::vector<d3d::Texture> m_D3DTextures;

		std::vector<ogl::Mesh> m_OGLMeshes;
		std::vector<ogl::Texture> m_OGLTextures;	

		LoadType m_LoadType;
		std::string m_dir;
	};
}
#endif
