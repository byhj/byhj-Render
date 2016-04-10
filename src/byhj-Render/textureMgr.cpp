#include "textureMgr.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include <assert.h>

namespace byhj
{
	/*
	string 转换为 wstring
	*/
	std::wstring c2w(const char *pc)
	{
		std::wstring val = L"";

		if (NULL == pc)
		{
			return val;
		}
		//size_t size_of_ch = strlen(pc)*sizeof(char);
		//size_t size_of_wc = get_wchar_size(pc);
		size_t size_of_wc;
		size_t destlen = mbstowcs(0, pc, 0);
		if (destlen == (size_t)(-1))
		{
			return val;
		}
		size_of_wc = destlen + 1;
		wchar_t * pw  = new wchar_t[size_of_wc];
		mbstowcs(pw, pc, size_of_wc);
		val = pw;
		delete pw;
		return val;
	}
	/*
	wstring 转换为 string
	*/
	std::string w2c(const wchar_t * pw)
	{
		std::string val = "";
		if (!pw)
		{
			return val;
		}
		size_t size= wcslen(pw)*sizeof(wchar_t);
		char *pc = NULL;
		if (!(pc = (char*)malloc(size)))
		{
			return val;
		}
		size_t destlen = wcstombs(pc, pw, size);
		/*转换不为空时，返回值为-1。如果为空，返回值0*/
		if (destlen == (size_t)(0))
		{
			return val;
		}
		val = pc;
		delete pc;
		return val;
	}


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
	GLuint TextureMgr::loadOGLTexture(std::string fileName, bool alpha)
	{
		std::string texFile = m_dir + fileName;
		int width = 0, height = 0;
		auto image = SOIL_load_image(texFile.c_str(), &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
		
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
		glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0,
			         alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_oglTextures.insert(std::make_pair(fileName, texId));

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		return texId;
	}

	GLuint TextureMgr::loadOGLDDS(std::string fileName)
	{
		std::string texFile = m_dir + fileName;
		std::wstring wtexFile = c2w(texFile.c_str());

		GLuint textureID;			// Create a texture ID as a GLuint
		ILuint imageID;				// Create an image ID as a ULuint
		ilInit();   //初始化IL

		ilGenImages(1, &imageID); 		// Generate the image ID
		ilBindImage(imageID); 			// Bind the image
#ifdef _UNICODE
		ILboolean success = ilLoadImage(wtexFile.c_str()); 	// Load the image file
#else
		ILboolean success = ilLoadImage(texFile.c_str()); 	// Load the image file
#endif
		if (success) {
			std::cout << ilGetInteger(IL_IMAGE_FORMAT) << std::endl;
			glGenTextures(1, &textureID); //创建Opengl纹理接口
			glBindTexture(GL_TEXTURE_2D, textureID);
			//设置纹理的过滤和环绕模式

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//将加载的纹理数据转化为OpenGL格式
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			//将数据传入纹理对象中
			glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
				0,				// Pyramid level (for mip-mapping) - 0 is the top level
				ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
				ilGetInteger(IL_IMAGE_WIDTH),	// Image width
				ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
				0,				// Border width in pixels (can either be 1 or 0)
				ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
				GL_UNSIGNED_BYTE,		// Image data type
				ilGetData());			// The actual image data itself
		} else {
			std::cout << "Fail to load the texture:" << texFile << std::endl;
			return -1;
		}
		ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
		
		std::cout << "Load the texture:" << texFile << std::endl;

		m_oglTextures.insert(std::make_pair(fileName, textureID));

		//return success
		return textureID;

	/*
		std::string texFile = m_dir + fileName;
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);
		//pointer to the image data
		BYTE* bits(0);
		//image width and height
		unsigned int width(0), height(0);
		//OpenGL's image ID to map to
		GLuint gl_texID;
		FREE_IMAGE_COLOR_TYPE type;

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(texFile.c_str(), 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(texFile.c_str());
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			return false;

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, texFile.c_str());


		bits = FreeImage_GetBits(dib);
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		type  = FreeImage_GetColorType(dib);
		std::cout << type << std::endl;
		//if this somehow one of these failed (they shouldn't), return failure
		if ((bits == 0) || (width == 0) || (height == 0))
			return false;

		glGenTextures(1, &gl_texID);
		glBindTexture(GL_TEXTURE_2D, gl_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0,   GL_RGB, 
			          width, height, 0,  GL_RGB,
			          GL_UNSIGNED_BYTE, bits);

		//Free FreeImage's copy of the data
		FreeImage_Unload(dib);

		std::cout << "Load the texture:" << fileName << std::endl;

		m_oglTextures.insert(std::make_pair(fileName, gl_texID));

		return gl_texID;
		*/
	}

	GLuint TextureMgr::loadOGLTexture(std::vector<std::string> &texArray)
	{
		if (m_oglTextures.find(texArray[0]) != m_oglTextures.end()) {
		  return m_oglTextures[texArray[0]];
		}

		GLuint texId;
		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &texId);
		glTextureStorage3D(texId, 1, GL_RGBA32F, 512, 512, texArray.size());
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		for (int i = 0; i != texArray.size(); ++i) {
			std::string texFile = m_dir + texArray[i];
			int width = 0, height = 0;
			auto image = SOIL_load_image(texFile.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
			glTextureSubImage3D(texId, 0, 0, 0, i, 512, 512, 1, GL_RGBA, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}


		m_oglTextures.insert(std::make_pair(texArray[0], texId));

		return texId;
	}
	GLuint TextureMgr::loadOGLTexture(std::string texName, std::vector<std::string> &faces)
	{
		if (m_oglTextures.find(texName) != m_oglTextures.end()) {
			return m_oglTextures[texName];
		}

		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char *image;
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (GLuint i = 0; i < faces.size(); ++i) {
			image = SOIL_load_image( (m_dir + faces[i] ).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			if (!image)
				std::cout << "Cannot load the cube map texture" << std::endl;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		m_oglTextures.insert(std::make_pair(texName, textureID));

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return textureID;
	}
	 ID3D11ShaderResourceView * TextureMgr::loadD3DTexture(ID3D11Device *pD3D11Device, std::string filename)
	{
	    if (m_d3dTextures.find(filename) != m_d3dTextures.end()) {
		    return m_d3dTextures[filename];
		}

		std::string texFile = m_dir + filename;
		std::wstring wTexFile(texFile.begin(), texFile.end());
		HRESULT hr;
		if (filename.substr(filename.find('.')) == ".dds") {
			hr = DirectX::CreateDDSTextureFromFile(pD3D11Device, wTexFile.c_str(), NULL, &pTextureSRV);
		}
		else {
			hr = DirectX::CreateWICTextureFromFile(pD3D11Device, wTexFile.c_str(), NULL, &pTextureSRV);
		}
		if (hr == S_OK) {
			std::cout << "Load the texture file:" << texFile << std::endl;
		}
		else {
			std::cerr << "Fail to load the texture file:" << texFile << std::endl;
		}
		assert(hr == S_OK);

		m_d3dTextures.insert(std::make_pair(filename, pTextureSRV));
		
		return pTextureSRV;
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