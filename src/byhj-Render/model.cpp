#include "model.h"
#include "modelMgr.h"

namespace byhj
{
	// Draws the model, and thus all its meshes
	void Model::draw(GLuint program)
	{
		glUseProgram(program);

		for (GLuint i = 0; i < m_OGLMeshes.size(); i++)
		{
			m_OGLMeshes[i].draw(program);
		}

		glUseProgram(0);
	}


	void Model::loadModel(std::string modelFile, LoadType loadType)
	{
		auto dir = ModelMgr::getInstance()->getDir();
		auto fileDir = dir + modelFile;

		Assimp::Importer importer;
		const aiScene *pScene = importer.ReadFile("E:/GitHub/byhj-Render/media/objects/" + modelFile, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!pScene|| pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
			std::cerr << "Fail to load the model ( " << modelFile << ") : "  
				      << importer.GetErrorString() << std::endl;
		}

		m_LoadType = loadType;
		m_dir = modelFile.substr(0, fileDir.find_last_of('/'));
		processNode(pScene->mRootNode, pScene);
	}

	void Model::processNode(aiNode *pNode, const aiScene *pScene)
	{
		for (GLuint i = 0; i < pNode->mNumMeshes; ++i) {
			aiMesh *pMesh = pScene->mMeshes[pNode->mMeshes[i]];

	        if (m_LoadType == LoadType::OGL) {
				OGLMesh meshData;
				processMesh(pMesh, pScene, meshData);
				m_OGLMeshes.push_back(meshData);
			} else {
				//d3d::Mesh meshData;
				//processMesh(pMesh, pScene, meshData);
				//m_D3DMeshes.push_back(meshData);
			}
		}

		for (GLuint i = 0; i < pNode->mNumChildren; ++i) {
		   processNode(pNode->mChildren[i], pScene);
		}
	}

	void Model::processMesh(aiMesh *mesh, const aiScene *scene, OGLMesh &oglMesh)
	{
		// Data to fill
		std::vector<OGLMesh::Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<OGLMesh::Texture> textures;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++) {
			OGLMesh::Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
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
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texcoord = vec;
			}
			else {
				vertex.texcoord = glm::vec2(0.0f, 0.0f);
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
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			std::vector<OGLMesh::Texture> diffuseMaps = this->loadOGLTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<OGLMesh::Texture> specularMaps = this->loadOGLTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		std::swap(oglMesh, OGLMesh(vertices, indices, textures) );

	}
/*
	void Model::processMesh(aiMesh *mesh, const aiScene *scene, d3d::Mesh &d3dMesh)
	{
		// Data to fill
		std::vector<d3d::Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<d3d::Texture> textures;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++) {
			d3d::Vertex vertex;
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
			} else {
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
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			std::vector<d3d::Texture> diffuseMaps = this->loadD3DTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<d3d::Texture> specularMaps = this->loadD3DTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
          std::swap(d3dMesh, d3d::Mesh(vertices, indices, textures) );

	}
*/

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
	std::vector<OGLMesh::Texture> Model::loadOGLTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<OGLMesh::Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for (GLuint j = 0; j < m_OGLTextures.size(); j++)
			{
				if (m_OGLTextures[j].path == str)
				{
					textures.push_back(m_OGLTextures[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   
					// If texture hasn't been loaded already, load it

				TextureMgr::getInstance()->loadOGLTexture(m_dir + str.C_Str());
				OGLMesh::Texture texture;
				texture.id = TextureMgr::getInstance()->getOGLTextureByName(str.C_Str());
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				this->m_OGLTextures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}


/*
	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
	std::vector<d3d::Texture> Model::loadD3DTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<d3d::Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for (GLuint j = 0; j < m_OGLTextures.size(); j++)
			{
				if (m_OGLTextures[j].path == str)
				{
					textures.push_back(m_D3DTextures[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // If texture hasn't been loaded already, load it
			//	TextureMgr::getInstance()->loadD3DTexture(m_dir + str.C_Str());
			//	d3d::Texture texture;
			//	texture.id = TextureFromFile(str.C_Str(), m_dir);
			//	texture.type = typeName;
			//	texture.path = str;
			//	textures.push_back(texture);
			//	this->m_OGLTextures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
	*/
}