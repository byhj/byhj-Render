#include "textureMgr.h"

namespace byhj
{
	TextureMgr::TextureMgr()
	{
		m_dir += "textures/";
	}

	TextureMgr::~TextureMgr()
	{
	}

	void TextureMgr::setDir(std::string dir)
	{
		m_dir = dir;
	}
	std::shared_ptr<TextureMgr> TextureMgr::getInstance()
	{
		static std::shared_ptr<TextureMgr> pInsance = std::make_shared<TextureMgr>();

		if (pInsance != nullptr)
		   return pInsance;
	}

	//Use SOIL to load the ogl texture
	void TextureMgr::loadOGLTexture(std::string fileName)
	{
		std::string texFile = m_dir + fileName;
		int width = 0, height = 0;
		auto image = SOIL_load_image(texFile.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		
		if (image == nullptr) {
			std::cerr << "Fail to load the texture file:" << texFile << std::endl;
		} else {
			std::cout << "Load the texture file:" << texFile << std::endl;
		}

		GLuint texId = -1;

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_oglTextures.insert(std::make_pair(fileName, texId));

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TextureMgr::loadOGLTexture(std::vector<std::string> &faces)
	{
		int width = 0, height = 0;

		GLuint tex = SOIL_load_OGL_cubemap(
			(m_dir + faces[0]).c_str(),
			(m_dir + faces[1]).c_str(),
			(m_dir + faces[2]).c_str(),
			(m_dir + faces[3]).c_str(),
			(m_dir + faces[4]).c_str(),
			(m_dir + faces[5]).c_str(),
			SOIL_LOAD_RGBA,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS
		);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_oglTextures.insert(std::make_pair(faces[0], tex));

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}
	void TextureMgr::loadD3DTexture(ID3D11Device *pD3D11Device, std::string filename)
	{
		HRESULT hr;
	
		//std::string texFile = m_dir + filename;
		//std::wstring wTexFile(texFile.begin(), texFile.end());

		//CreateDDSTextureFromFile(pD3D11Device, texFile, NULL, &pTextureSRV);
		//m_d3dTextures.insert(std::make_pair(filename, pTextureSRV));
	}

	GLuint TextureMgr::getOGLTextureByName(std::string fileName)
	{
	     auto iter = m_oglTextures.find(fileName);
		 if (iter != m_oglTextures.end()) {
		     return m_oglTextures[fileName];
		 } else {
		   std::cerr << "Texture "  << fileName << " is not load." << std::endl;
		   return -1;
		 }
	}

	ID3D11ShaderResourceView* TextureMgr::getD3DTextureByName(std::string fileName)
	{
		auto iter = m_d3dTextures.find(fileName);
		if (iter != m_d3dTextures.end()) {
		    return m_d3dTextures[fileName];
		} else {
			std::cerr << "Texture " << fileName << " is not load." << std::endl;
			return nullptr;
		}

	}

}