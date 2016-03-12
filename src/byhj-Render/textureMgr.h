#ifndef textureMgr_H
#define textureMgr_H

#include "Mgr.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <GL/glew.h>
#include <d3d11.h>
#include <memory>
#include <FreeImage.h>
#include <SOIL.h>

#include <FreeImage.h>
#pragma comment(lib, "FreeImage")

#include <IL/il.h>
#pragma comment(lib, "DevIL.lib")

#include "DirectXTK/DDSTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")

namespace byhj
{
	class TextureMgr : public Mgr
	{
	public:
		TextureMgr();
	   ~TextureMgr();

	   static std::shared_ptr<TextureMgr>  getInstance();

	   void   setDir(std::string dir);
	   void   loadOGLTexture(std::vector<std::string> &texArray);
	   GLuint   loadOGLTexture(std::string fileName, bool alpha = false);
	   GLuint   loadOGLDDS(std::string fileName);
	   void   loadOGLTexture(std::string texName, std::vector<std::string> &faces);
	   void   loadOGLTexture(std::string fileName, GLenum image_format, GLint internal_format);
	   ID3D11ShaderResourceView * loadD3DTexture(ID3D11Device *pD3D11Device, std::string fileName);

	   GLuint getOGLTextureByName(std::string fileName) ;
	   ID3D11ShaderResourceView * getD3DTextureByName(std::string name) ;
	   ID3D11ShaderResourceView *pTextureSRV = nullptr;
	private:
          std::unordered_map<std::string, GLuint> m_oglTextures;
		  std::unordered_map<std::string, ID3D11ShaderResourceView*> m_d3dTextures; 
	  
	};
}
#endif