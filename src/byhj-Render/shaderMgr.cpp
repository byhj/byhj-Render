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

	std::shared_ptr<ShaderMgr> getInstance()
	{
		static std::shared_ptr<ShaderMgr> pInstance = std::make_shared<ShaderMgr>();
		
		return pInstance;
	}
}