#include "Soft351SoundFile.h"

// Usefull links:
// Wave header:
// http://soundfile.sapp.org/doc/WaveFormat/
// WAVEFORMATEX:
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd390970(v=vs.85).aspx
// Fopen:
// http://www.cplusplus.com/reference/cstdio/fopen/
// fRead:
// http://www.cplusplus.com/reference/cstdio/fread/
// fSeek:
// http://www.cplusplus.com/reference/cstdio/fseek/

// Constructor
Soft351SoundFile::Soft351SoundFile()
{
	m_data = 0;
}

// Deconstructor
Soft351SoundFile::~Soft351SoundFile()
{
}

// Release objects before deletion
void Soft351SoundFile::Release() {
	if (m_data)
		delete m_data;
}

// Load a wave file into object
bool Soft351SoundFile::LoadFile(
	char* fileLocation,	// wave file location
	long int volume		// default game volume
) {
	// Setup method variables
	int error;
	FILE* p_file;
	unsigned int elements;
	HRESULT result;

	// set object volume
	m_volume = volume;

	// Open the wave file to FILE pointer
	error = fopen_s(&p_file, fileLocation, "rb");
	// if failed to open file, return false
	if (error != 0)
	{
		return false;
	}

	// Read the wave file header data to m_header
	elements = fread(&m_header, sizeof(m_header), 1, p_file);
	// If failed return false, failed to read header
	if (elements != 1)
	{
		return false;
	}

	// Check if file ID is RIFF
	if (
		(m_header.chunkId[0] != 'R') ||
		(m_header.chunkId[1] != 'I') ||
		(m_header.chunkId[2] != 'F') ||
		(m_header.chunkId[3] != 'F')
		)
	{
		// if failed return false, file ID is not RIFF
		return false;
	}

	// Check if the file is in wave format
	if (
		(m_header.format[0] != 'W') ||
		(m_header.format[1] != 'A') ||
		(m_header.format[2] != 'V') ||
		(m_header.format[3] != 'E')
		)
	{
		// if failed return false, file format is not WAVE
		return false;
	}

	// See if data chunk exists
	if (
		(m_header.dataChunk2Id[0] != 'd') ||
		(m_header.dataChunk2Id[1] != 'a') ||
		(m_header.dataChunk2Id[2] != 't') ||
		(m_header.dataChunk2Id[3] != 'a')
		)
	{
		// if failed return false, data chunk not found
		return false;
	}

	// Set the wave format from header data
	m_format.wFormatTag = m_header.audioFormat;
	m_format.nSamplesPerSec = m_header.sampleRate;
	m_format.wBitsPerSample = m_header.bitsPerSample;
	m_format.nChannels = m_header.numChannels;
	m_format.nBlockAlign = (m_format.wBitsPerSample / 8) * m_format.nChannels;
	m_format.nAvgBytesPerSec = m_format.nSamplesPerSec * m_format.nBlockAlign;
	m_format.cbSize = 0;

	// Seek the beginning of data in data chunk
	fseek(p_file, sizeof(WaveHeaderType), SEEK_SET);

	// Initialize the data holder
	m_data = new unsigned char[m_header.data];
	// if failed return false, failed to create data holder
	if (!m_data)
	{
		return false;
	}

	// Copy the file data to m_data holder
	elements = fread(m_data, 1, m_header.data, p_file);
	// if failed return false, failed to copy data
	if (elements != m_header.data)
	{
		return false;
	}

	// Close the file
	error = fclose(p_file);
	// if failed return false, failed to close file
	if (error != 0)
	{
		return false;
	}

	// Cleanup
	p_file = NULL;

	// Everything worked ok!
	return true;
}