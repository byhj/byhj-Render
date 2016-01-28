#include "shaderMgr.h"

namespace byhj
{

	ShaderMgr::ShaderMgr()
	{
		m_dir += "shaders/";
	}

	ShaderMgr::~ShaderMgr()
	{

	}

	std::shared_ptr<ShaderMgr> ShaderMgr::getInstance()
	{
		static auto pInstance = std::make_shared<ShaderMgr>();
		
		return pInstance;
	}
}