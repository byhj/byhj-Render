#ifndef SoundMgr_H
#define SoundMgr_H

#include "mgr.h"
#include <memory>
#include <string>
#include <iostream>
#include <unordered_set>

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

	  void load(std::string soundName);
      void play(std::string soundNmae);
	  void play();
	  void shutdown();

	private:
		irrklang::ISoundEngine *pSoundEngine = nullptr;
	    std::unordered_set<std::string> m_SoundFiles; 
	};
}

#endif
