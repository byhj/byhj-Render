#include "d3dModel.h"
#include "modelMgr.h"
#include "textureMgr.h"

namespace byhj
{
	void setColor(aiColor3D &c, XMFLOAT4 &m)
	{
		m.x = c[0];
		m.y = c[1];
		m.z = c[2];
	}

	void setBlend(float blend, D3DMesh::Material &mat)
	{
		mat.ambient.w = blend;
		mat.specular.w = blend;
		mat.diffuse.w = blend;
		mat.emissive.w = blend;
	}
	void D3DModel::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		for (int i = 0; i != m_D3DMeshes.size(); ++i) {
			m_D3DMeshes[i].init(pD3D11Device, pD3D11DeviceContext);
		}
	}
	// Draws the model, and thus all its meshes
	void D3DModel::render(ID3D11DeviceContext *pD3D11DeviceContext)
	{


		for (int i = 0; i < this->m_D3DMeshes.size(); i++) {
			float blendFactor[] ={ 0.4f, 0.4f, 0.4f, 0.3f };
			//if (this->m_D3DMeshes[i].m_material.ambient.w < 1.0f)
		//		pD3D11DeviceContext->OMSetBlendState(Transparency, blendFactor, 0xffffffff);
			//"fine-tune" the blending equation

		// 	pD3D11DeviceContext->UpdateSubresource(m_pMatBuffer, 0, NULL, &this->meshes[i].mat, 0, 0);
		// 	pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &m_pMatBuffer);
			this->m_D3DMeshes[i].render(pD3D11DeviceContext);

			pD3D11DeviceContext->OMSetBlendState(0, 0, 0xffffffff);
		}
	}



	void D3DModel::loadModel(std::string modelFile)
	{
		auto dir = ModelMgr::getInstance()->getDir();
		auto fileDir = dir + modelFile;

		Assimp::Importer importer;
		const aiScene *pScene = importer.ReadFile(dir + modelFile, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!pScene || pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
			std::cerr << "Fail to load the model ( " << modelFile << ") : "
				<< importer.GetErrorString() << std::endl;
		}
		else {
			std::cout << "Load Model: " << fileDir << std::endl;
		}

		m_dir = fileDir.substr(0, fileDir.find_last_of('/') + 1);
		processNode(pScene->mRootNode, pScene);
	}

	void D3DModel::processNode(aiNode *pNode, const aiScene *pScene)
	{
		for (GLuint i = 0; i < pNode->mNumMeshes; ++i) {
			aiMesh *pMesh = pScene->mMeshes[pNode->mMeshes[i]];

			D3DMesh meshData;
			processMesh(pMesh, pScene, meshData);
			m_D3DMeshes.push_back(meshData);
		
		}

		for (GLuint i = 0; i < pNode->mNumChildren; ++i) {
			processNode(pNode->mChildren[i], pScene);
		}
	}


	void D3DModel::processMesh(aiMesh *mesh, const aiScene *scene, D3DMesh &d3dMesh)
	{
		// Data to fill
		std::vector<D3DMesh::Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<D3DMesh::Texture> textures;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++) {
			D3DMesh::Vertex vertex;
			XMFLOAT3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			// Texture Coordinates // Does the mesh contain texture coordinates
			if (mesh->mTextureCoords[0]) {
				XMFLOAT2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texcoord = vec;
			}
			else {
				vertex.texcoord = XMFLOAT2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0) {
			int tt = mesh->mMaterialIndex;
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			aiColor3D ambient;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, ambient))
				setColor(ambient, mat.ambient);
			//std::cout << mat.ambient.w << std::endl;
			aiColor3D diffuse;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
				setColor(diffuse, mat.diffuse);

			aiColor3D  speucular;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, speucular))
				setColor(speucular, mat.specular);

			aiColor3D  emissive;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive))
				setColor(emissive, mat.emissive);

			//float shininess = 0.0;
			//if(AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess))
			//	mat.shininess = shininess;
			float blend;
			material->Get(AI_MATKEY_OPACITY, blend);
			if (blend < 1.0f)
				setBlend(blend, mat);
			//std::cout << mat.ambient.w << std::endl;

		// 1. Diffuse maps
			std::vector<D3DMesh::Texture> diffuseMaps = this->loadD3DTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			// 2. Specular maps
			std::vector<D3DMesh::Texture> specularMaps = this->loadD3DTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			// 3.normal maps
			std::vector<D3DMesh::Texture> normalMaps = this->loadD3DTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		std::swap(d3dMesh, D3DMesh(vertices, indices, textures, mat));

	}


	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
	std::vector<D3DMesh::Texture> D3DModel::loadD3DTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<D3DMesh::Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for (GLuint j = 0; j < m_D3DTextures.size(); j++)
			{
				if (m_D3DTextures[j].name == str.C_Str())
				{
					textures.push_back(m_D3DTextures[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{
				//If texture hasn't been loaded already, load it
				D3DMesh::Texture texture;
				texture.pTextureSRV = nullptr;
				texture.type = typeName;
				texture.name = str.C_Str();
				textures.push_back(texture);
				this->m_D3DTextures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
}