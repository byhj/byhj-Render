#ifndef Model_H
#define Model_H

#include "ogl/oglModel.h"
#include "d3d/d3dModel.h"


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
		void drawInstance(GLuint program, GLuint amount);

		void loadModel(std::string fileName, LoadType loadType);
		void processNode(aiNode *node, const aiScene *scene);
		void processMesh(aiMesh *mesh, const aiScene *scene, OGLMesh &oglMesh);
		void processMesh(aiMesh *mesh, const aiScene *scene, D3DMesh &d3dMesh);

		std::vector<OGLMesh::Texture> loadOGLTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		std::vector<D3DMesh::Texture> loadD3DTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		int getMeshCount() const;
		GLuint getMeshVAO(int index) const;

	private:
		std::vector<OGLMesh::Texture> m_OGLTextures;
		std::vector<OGLMesh> m_OGLMeshes;
		std::vector<D3DMesh::Texture> m_D3DTextures;
		std::vector<D3DMesh> m_D3DMeshes;

		LoadType m_LoadType;
		std::string m_dir;
	};
}
#endif
