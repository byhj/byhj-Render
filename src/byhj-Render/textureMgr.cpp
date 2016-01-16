#include "textureMgr.h"

namespace byhj
{
	TextureMgr::TextureMgr()
	{
	}

	TextureMgr::~TextureMgr()
	{
	}

	std::shared_ptr<TextureMgr> TextureMgr::getInstance()
	{
		static std::shared_ptr<TextureMgr> pInsance = std::make_shared<TextureMgr>();
		if (pInsance != NULL)
		   return pInsance;
	}

	void TextureMgr::loadOGLTexture(std::string name)
	{
	}

	void TextureMgr::loadD3DTexture(std::string name)
	{
	}

	GLuint TextureMgr::getOGLTextureByName(std::string name)
	{
	}

	ID3D11ShaderResourceView* TextureMgr::getD3DTextureByName(std::string name)
	{

	}

}