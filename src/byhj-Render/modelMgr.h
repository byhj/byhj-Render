#ifndef ModelMgr_H
#define ModelMgr_H

#include "mgr.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace byhj
{
	class ModelMgr : public Mgr
	{
	public:
	  ModelMgr();
	  ~ModelMgr();

	  static std::shared_ptr<ModelMgr> getInstance();


	  void loadObj(std::string modelName);
	private:
	  

	};

}
#endif