#ifndef ResourceMgr_H
#define ResourceMgr_H

#include "modelMgr.h"
#include "shaderMgr.h"
#include "textureMgr.h"
#include "soundMgr.h"


#include <memory>

namespace byhj
{

	class ResourceMgr {
	public:
		ResourceMgr();
		~ResourceMgr();

		static std::shared_ptr<ResourceMgr> getInsance();

		std::shared_ptr<ShaderMgr>  getShaderMgr();
		//std::shared_ptr<FontMgr>    getFontMgr();
		std::shared_ptr<ModelMgr>   getModelMgr();
		std::shared_ptr<SoundMgr>   getSoundMgr();
		std::shared_ptr<TextureMgr> getTextureMgr();

	private:
		//std::shared_ptr<ShaderMgr > m_pShaderMgr  = nullptr; 
		//std::shared_ptr<FontMgr   > m_pFontMgr    = nullptr;
		//std::shared_ptr<SoundMgr  > m_pSoundMgr   = nullptr;
		//std::shared_ptr<ModelMgr  > m_pModelMgr   = nullptr;
		//std::shared_ptr<TextureMgr> m_pTextureMgr = nullptr;

	};
}
#endif