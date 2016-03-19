#ifndef OGLModel_H
#define OGLModel_H

#include "oglMesh.h"

#include <vector>
#include <SOIL.h>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "textureMgr.h"

namespace byhj
{
	class OGLModel {
	public:
		OGLModel() = default;
		~OGLModel() = default;

		void render(GLuint program);
		void renderInstance(GLuint program, GLuint amount);

		void loadModel(std::string fileName);
		void processNode(aiNode *node, const aiScene *scene);
		void processMesh(aiMesh *mesh, const aiScene *scene, OGLMesh &oglMesh);

		std::vector<OGLMesh::Texture> loadOGLTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	
		int getMeshCount() const;
		GLuint getMeshVAO(int index) const;

	private:
		std::vector<OGLMesh::Texture> m_OGLTextures;
		std::vector<OGLMesh> m_OGLMeshes;
		std::string m_dir;
	};
}
#endif
