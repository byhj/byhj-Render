#ifndef Model_H
#define Model_H

#include "mesh.h"

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace byhj
{
	class Model {
	public:
		Model() = default;
		~Model() = default;

		void loadModel(std::string fileName);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	private:
		std::vector<Mesh> m_Meshes;
	};
}
#endif
