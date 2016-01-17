#include "soundMgr.h"

namespace byhj
{
	SoundMgr::SoundMgr()
	{
		m_dir += "sounds/";
		pSoundEngine = irrklang::createIrrKlangDevice();

		if (pSoundEngine == nullptr) {
		   std::cerr << "Cound not init sound engine" << std::endl;
		}

	}

	SoundMgr::~SoundMgr()
	{

	}

	void SoundMgr::loadSound(std::string soundFile)
	{
	}

	void SoundMgr::playSound(std::string soundFile)
	{
		std::cout << "Play sound: " << soundFile << std::endl;

		//Loop the music
		pSoundEngine->play2D((m_dir + soundFile).c_str(), true);
	}

	void SoundMgr::shutdown()
	{
		pSoundEngine->drop();
	}
}