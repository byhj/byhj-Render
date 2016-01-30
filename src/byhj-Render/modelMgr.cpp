#include "modelMgr.h"

namespace byhj
{
	ModelMgr::ModelMgr()
	{
		m_dir += "objects/";
	}

	ModelMgr::~ModelMgr()
	{

	}

	std::shared_ptr<ModelMgr> ModelMgr::getInstance()
	{
		static std::shared_ptr<ModelMgr>  pInstance = std::make_shared<ModelMgr>();

		if (pInstance != nullptr) {
			return pInstance;
		}
	}

	void ModelMgr::loadObj(std::string modelName)
	{

	}

	

}