#include "Soft351Sound.h"

// usefull links:
// - DirectSoundBuffer
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/mt708923(v%3dvs.85)
// - DirectSound8 interface:
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee418035%28v%3dvs.85%29
// - WAVEFORMATEX:
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd390970(v=vs.85).aspx
// - DSCBUFFERDESC:
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee416820(v=vs.85)

// Constructor
Soft351Sound::Soft351Sound()
{
	m_globalBuffer = 0;
	m_dSound = 0;
	m_carLoopBuffer = 0;
	m_effectBuffer = 0;
	m_musicBuffer = 0;
}

// Deconstruction
Soft351Sound::~Soft351Sound()
{
}

// Release objects before deletion
void Soft351Sound::Release() {
	if (m_effectBuffer && _effectPlayed)
	{
		m_effectBuffer->Release();
		m_effectBuffer = NULL;
	}

	if (m_carLoopBuffer && _carEffectPlayed)
	{
		m_carLoopBuffer->Release();
		m_carLoopBuffer = NULL;
	}

	if (m_musicBuffer)
	{
		m_musicBuffer->Release();
		m_musicBuffer = NULL;
	}

	if (m_globalBuffer)
	{
		m_globalBuffer->Release();
		m_globalBuffer = NULL;
	}

	if (m_dSound)
	{
		m_dSound->Release();
		m_dSound = NULL;
	}

	return;
}

// Setup and initialize directSound
bool Soft351Sound::Setup(
	HWND hwnd
)
{
	// Setup method variables
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// create directSound device
	result = DirectSoundCreate8(NULL, &m_dSound, NULL);
	// if failed return false, could not create device
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level
	result = m_dSound->SetCooperativeLevel(
		hwnd, 
		DSSCL_PRIORITY
	);
	// If failed return false, could not set cooperative level
	if (FAILED(result))
	{
		return false;
	}

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	// Set as primary buffer and enable volume control
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	// Rest set to null as not needed
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// create the global buffer
	result = m_dSound->CreateSoundBuffer(&bufferDesc, &m_globalBuffer, NULL);
	// if failed return false, could not create global buffer
	if (FAILED(result))
	{
		return false;
	}

	// Setup wave format with default values
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = m_globalBuffer->SetFormat(&waveFormat);
	// if failed return false, could not set format
	if (FAILED(result))
	{
		return false;
	}

	// Everything worked, return true
	return true;
}

// Play a sound effect on the sound effect buffer
bool Soft351Sound::PlayEffect(
	Soft351SoundFile* file
)
{
	if (!file) return false;
	// Load the wave file to buffer
	LoadWavToBuffer(file, &m_effectBuffer);
	// set the volume from sound file
	SetVolume(file->m_volume, m_effectBuffer);
	// play the effect
	PlayBuffer(m_effectBuffer);

	// set release helper to true
	_effectPlayed = true;

	return true;
}

// Play looping music on music buffer
bool Soft351Sound::PlayMusic(
	Soft351SoundFile* file
)
{
	if (!file) return false;
	// Load the wave file to buffer
	LoadWavToBuffer(file, &m_musicBuffer);
	// set the volume from sound file
	SetVolume(file->m_volume, m_musicBuffer);
	// play the effect
	PlayBuffer(m_musicBuffer, true);
	return true;
}

// Play car loop on car buffer
bool Soft351Sound::PlayCarLoop(
	Soft351SoundFile* file
)
{
	if (!file) return false;
	// Load the wave file to buffer
	LoadWavToBuffer(file, &m_carLoopBuffer);
	// set the volume from sound file
	SetVolume(file->m_volume, m_carLoopBuffer);
	// play the effect
	PlayBuffer(m_carLoopBuffer, true);

	// set release helper to true
	_carEffectPlayed = true;

	return true;
}

// Stop music playing
void Soft351Sound::StopMusic() {
	m_musicBuffer->Stop();
}

// Stop car effect playing
void Soft351Sound::StopCarLoop() {
	m_carLoopBuffer->Stop();
}

// Load a wave file from Soft351SoundFile to buffer
bool Soft351Sound::LoadWavToBuffer(
	Soft351SoundFile* file,
	IDirectSoundBuffer** buffer
)
{
	// Create initial variables
	HRESULT result = S_OK;
	DSBUFFERDESC bufferDesc;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	// Set the buffer description for load buffer
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	// Enable volume control
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME; 
	// Set buffer size from file wave header
	bufferDesc.dwBufferBytes = file->m_header.data;
	bufferDesc.dwReserved = 0;
	// Set format from wave file format
	bufferDesc.lpwfxFormat = &file->m_format;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Load the buffer with description from file
	m_dSound->CreateSoundBuffer(&bufferDesc, &*buffer, NULL);
	// lock inputted buffer so data can write to it
	result = (*buffer)->Lock(0, file->m_header.data, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	// If failed return false, could not lock buffer
	if (FAILED(result))	return false;
	// Copy the wave data from file to the buffer
	memcpy(bufferPtr, file->m_data, file->m_header.data);
	// Unlock the inputted buffer
	result = (*buffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	// if failed return false, could not unlock buffer
	if (FAILED(result))	return false;

	// Cleanup
	bufferPtr = NULL;

	// Everything went ok!
	return true;
}

// Set the volume for the buffer
void Soft351Sound::SetVolume(
	long int val,
	IDirectSoundBuffer* buffer
) {
	// Convert volume to -4000, 0 range for buffer volume level
	if (val < 0) val = 0;
	if (val > 100) val = 100;
	val = 4000 - (val * 40);
	// Not a perfect way to set volume but works for this program
	buffer->SetVolume(-val);
}

// Play a buffer
void Soft351Sound::PlayBuffer(
	IDirectSoundBuffer* buffer,
	bool loop
)
{
	// Set the position to 0
	buffer->SetCurrentPosition(0);
	// play the effect with looping if selected
	buffer->Play(0, 0, (loop) ? DSBPLAY_LOOPING : 0);
}