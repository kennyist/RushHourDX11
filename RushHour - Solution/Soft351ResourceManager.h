#ifndef _SOFT351RESOURCEMANAGER_H
#define _SOFT351RESOURCEMANAGER_H

#include "Soft351Mesh.h"
#include "Soft351ShaderAmbient.h"
#include "Soft351SoundFile.h"
#include <vector>

static class Soft351ResourceManager
{
public:
	// --- Constructor and deconstruction 
	Soft351ResourceManager();
	~Soft351ResourceManager();

	// --- Public methods
	// Release all objects before deletion
	void Release();
	// Add new mesh to the manager
	bool AddMesh(
		ID3D11Device* pd3Device,
		LPCTSTR meshFile,				// mesh location
		bool createAdjacentIndicies,
		LPCTSTR meshName,				// Mesh unique name
		Soft351ShaderAmbient* shader			// Shader the mesh will use
	);
	// Get mesh from manager using its unique name
	Soft351Mesh* GetMesh(LPCTSTR name);
	// Load all meshes to the manager
	bool LoadMeshFiles(ID3D11Device* pd3Device);
	// Add shader to the manager
	bool AddShader(
		LPCTSTR shaderName,				// Unique name
		ID3D11Device* renderingDevice
	);
	// Load all shaders to the manager
	bool LoadShaders(ID3D11Device* device);
	// Get shader using unique name
	Soft351ShaderAmbient* GetShader(LPCTSTR name);
	// Add a sound file to the manager
	bool AddSound(
		char* location,		// file location
		long int volume,	// Volume to use in game
		LPCTSTR name		// Unique name
	);
	// Load all sound files
	bool LoadSoundFiles();
	// Get sound file by unique name
	Soft351SoundFile* GetSound(LPCTSTR name);
	// Update lighting to all meshes
	void UpdateLighting(
		XMFLOAT4 ambientLight,
		XMFLOAT4 lightDiffuse,
		XMVECTOR lightDirection
	);
	

private:
	// --- Private variables
	// Mesh manager
	std::vector<Soft351Mesh> m_meshList;
	// Shader Manager
	std::vector<Soft351ShaderAmbient> m_shaderList;
	// Sounds manager
	std::vector<Soft351SoundFile> m_soundList;
};

#endif