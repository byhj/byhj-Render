#include "model.h"

namespace byhj
{
	void Model::loadModel(std::string modelFile)
	{
		Assimp::Importer importer;
		const aiScene *pScene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!pScene|| pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
			std::cerr << "Fail to load the model ( " << modelFile << ") : "  
				      << importer.GetErrorString() << std::endl;
		}

		processNode(pScene->mRootNode, pScene);
	}

	void Model::processNode(aiNode *pNode, const aiScene *pScene)
	{
		for (GLuint i = 0; i < pNode->mNumMeshes; ++i) {
			aiMesh *pMesh = pScene->mMeshes[pNode->mMeshes[i]];
			m_Meshes.push_back(processMesh(pMesh, pScene));
		}

		for (GLuint i = 0; i < pNode->mNumChildren; ++i) {
		   processNode(pNode->mChildren[i], pScene);
		}
	}
}