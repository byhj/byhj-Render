#ifndef SOUND_H
#define SOUND_H

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include "WAVFileReader.h"
#include <windows.h>
#include <stdio.h>

namespace byhj
{

class Sound
{
public:
	void Init();
	void Play();
	void Shutdown();

private:

	HRESULT PlayWave(_In_ IXAudio2* pXaudio2, _In_z_ LPCWSTR szFilename);
	HRESULT FindMediaFileCch(_Out_writes_(cchDest) WCHAR* strDestPath, _In_ int cchDest, _In_z_ LPCWSTR strFilename);
	
	IXAudio2 *m_pXAudio2 = nullptr;
	IXAudio2MasteringVoice* m_pMasteringVoice = nullptr;

};


}
#endif
