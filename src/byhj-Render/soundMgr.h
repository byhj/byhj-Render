#ifndef SoundMgr_H
#define SoundMgr_H

#include "mgr.h"
#include <memory>
#include <string>
#include <iostream>

#include "irrKlang/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

namespace byhj
{
    class SoundMgr : Mgr
	{
	public:
	  SoundMgr();
	  ~SoundMgr();
	   
	  static std::shared_ptr<SoundMgr> getInstance();

	  void loadSound(std::string soundName);
      void playSound(std::string soundNmae);
	  void shutdown();

	private:
		irrklang::ISoundEngine *pSoundEngine = nullptr;
	  
	};
}

#endif
