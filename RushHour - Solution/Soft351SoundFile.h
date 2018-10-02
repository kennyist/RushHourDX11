#ifndef _SOFT351SOUNDFILE_H
#define _SOFT351SOUNDFILE_H

#include <DXUT.h>
#include <dsound.h>

// Usefull links:
// Wave header:
// http://soundfile.sapp.org/doc/WaveFormat/

class Soft351SoundFile
{
public:
	// --- Public Structs
	// wave header data
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunk1Id[4];
		unsigned long subChunk1Size;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long byteRate;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunk2Id[4];
		unsigned long data;
	};

	// --- Constructor and deconstruction
	Soft351SoundFile();
	~Soft351SoundFile();

	// --- Public methods
	// Load a new wav file into the object
	bool LoadFile(char* fileName, long int volume = 0);
	// Release all objects before deletion
	void Release();

	// --- Public Variables:
	// wave format 
	WAVEFORMATEX m_format;
	// wave file data
	unsigned char* m_data;
	// wave file header data
	WaveHeaderType m_header;
	// wave volume
	long int m_volume;
	// unique name
	LPCTSTR m_name;
};

#endif // !_SOFT351SOUNDFILE_H

