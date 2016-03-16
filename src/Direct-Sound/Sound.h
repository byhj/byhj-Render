#ifndef Sound_H
#define Sound_H

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

namespace byhj
{

namespace d3d
{

class Sound
{

public:
	Sound();
	Sound(const Sound &);
	~Sound();

	bool Init(HWND, char *soundFile);
	void Shutdown();
	bool PlayWaveFile();

private:

	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

private:
	bool InitDirectSound(HWND hWnd);
	void ShutdownDirectSound();
	bool LoadWaveFile(char *fileName, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

private:
	IDirectSound8 *pDirectSound;
	IDirectSoundBuffer  *pPrimaryBuffer;
	IDirectSoundBuffer8  *pSecondaryBuffer;
};

}
}

#endif