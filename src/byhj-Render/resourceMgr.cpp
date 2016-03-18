#include "resourceMgr.h"

namespace byhj
{
	ResourceMgr::ResourceMgr()
	{

	}

	ResourceMgr::~ResourceMgr()
	{
	}

	std::shared_ptr<ResourceMgr> getInstance()
	{
		static std::shared_ptr<ResourceMgr> pInstance = std::make_shared<ResourceMgr>();

		return pInstance;
	}

	std::shared_ptr<ShaderMgr> getShaderMgr()
	{
		return ShaderMgr::getInstance();
	}


	std::shared_ptr<ModelMgr> getModelMgr()
	{
		return ModelMgr::getInstance();
	}


	std::shared_ptr<TextureMgr> getTextureMgr()
	{
		return TextureMgr::getInstance();
	}


	// std::shared_ptr<FontMgr> getFontMgr()
	// {
	// 	return FontMgr::getInstance();
	// }
   // 

	std::shared_ptr<SoundMgr> getSoundMgr()
	{
		return SoundMgr::getInstance();
	}
}