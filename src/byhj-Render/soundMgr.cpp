#include "soundMgr.h"

namespace byhj
{
	SoundMgr::SoundMgr()
	{
		m_dir += "sounds/";
		pSoundEngine = irrklang::createIrrKlangDevice();

		if (pSoundEngine == nullptr) {
		   std::cerr << "Can not init the sound engine!" << std::endl;
		}

	}

	SoundMgr::~SoundMgr()
	{

	}

	std::shared_ptr<SoundMgr> SoundMgr::getInstance()
	{
		static std::shared_ptr<SoundMgr> pInstance = std::make_shared<SoundMgr>();

		return pInstance;
	}

	void SoundMgr::load(std::string soundFile)
	{
		m_SoundFiles.insert(soundFile);
	}

	void SoundMgr::play(std::string soundFile)
	{
		if (m_SoundFiles.find(soundFile) == m_SoundFiles.end()) {
		     std::cerr << "You have not load the sound file" << soundFile << std::endl;
			 return;
		}

		auto sound = m_dir + soundFile;
		std::cout << "Play sound: " << sound << std::endl;
		pSoundEngine->play2D(sound.c_str(), true);
	}

	//Play all sound whose has load
	void SoundMgr::play()
	{
		for (auto iter = m_SoundFiles.begin(); iter != m_SoundFiles.end(); ++iter) {
			//Loop the music
			auto sound = m_dir + *iter;
			std::cout << "Play sound: " << sound << std::endl;
			pSoundEngine->play2D(sound.c_str(), true);
		}
	}

	void SoundMgr::shutdown()
	{
		pSoundEngine->drop();
	}
}