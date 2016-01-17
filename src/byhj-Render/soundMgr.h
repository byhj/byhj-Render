#ifndef SoundMgr_H
#define SoundMgr_H

#include "mgr.h"
#include <memory>
#include <string>

namespace byhj
{
    class SoundMgr : Mgr
	{
	  SoundMgr();
	  ~SoundMgr();
	   
	  static std::shared_ptr<SoundMgr> getInstance();

	  void loadSound(std::string soundName);

	private:

	  
	};
}

#endif
