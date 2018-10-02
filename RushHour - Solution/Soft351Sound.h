#ifndef _SOFT351SOUND_H
#define _SOFT351SOUND_H

#pragma comment(lib, "dsound.lib")

#include <dsound.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include "Soft351SoundFile.h"

class Soft351Sound
{
public:
	// --- Constructor and deconstruction
	Soft351Sound();
	~Soft351Sound();

	// --- Public methods
	// Setup and initialize directSound
	bool Setup(HWND hwnd);
	// Release objects before deletion
	void Release();
	// Play a sound effect on the sound effect buffer
	bool PlayEffect(Soft351SoundFile* file);
	// Play looping music on music buffer
	bool PlayMusic(Soft351SoundFile* file);
	// Play car loop on car buffer
	bool PlayCarLoop(Soft351SoundFile* file);
	// Stop music
	void StopMusic();
	// Stop car loop
	void StopCarLoop();

private:
	// --- Private methods
	// Load a wave file from Soft351SoundFile to buffer
	bool LoadWavToBuffer(
		Soft351SoundFile* file,		// sound file
		IDirectSoundBuffer** buffer	// buffer to load to
	);
	// Set the volume for the buffer
	void SetVolume(
		long int val,				// volume 0-100
		IDirectSoundBuffer* buffer	// buffer to set volume too
	);
	// Play a buffer
	void PlayBuffer(
		IDirectSoundBuffer* buffer,	// buffer to play
		bool loop = false			// loop buffer?
	);

	// --- Private Variables:
	// Direct sound interface
	IDirectSound8* m_dSound;
	// global primary buffer
	IDirectSoundBuffer* m_globalBuffer;
	// Car loop buffer
	IDirectSoundBuffer* m_carLoopBuffer;
	// Sound effect buffer
	IDirectSoundBuffer* m_effectBuffer;
	// Music buffer
	IDirectSoundBuffer* m_musicBuffer;

	// Release helpers
	// Used due to release errors if no sound has been played on buffer
	bool _effectPlayed = false;
	bool _carEffectPlayed = false;
};

#endif

