#include "DXUT.h"
#include "Soft351ResourceManager.h"

// -- Usefull links
// Vector:
// http://www.cplusplus.com/reference/vector/vector/

// Constructor
Soft351ResourceManager::Soft351ResourceManager()
{
}
// Deconstruction
Soft351ResourceManager::~Soft351ResourceManager()
{
}

// Release all objects before deletion
void Soft351ResourceManager::Release() {
	// Loop through and release all shaders and clear list
	for (int i = 0; i < m_shaderList.size(); i++) {
		m_shaderList[i].Release();
	}
	m_shaderList.clear();
	// Loop through and release all meshes and clear list
	for (int i = 0; i < m_meshList.size(); i++) {
		m_meshList[i].Release();
	}
	m_meshList.clear();
	// Loop through and release all sounds and clear list
	for (int i = 0; i < m_soundList.size(); i++) {
		m_soundList[i].Release();
	}
	m_soundList.clear();
}

// --- MESHES
// Get mesh by unique name
Soft351Mesh* Soft351ResourceManager::GetMesh(LPCTSTR name)
{
	// Loop through list and check the name, if match return the object
	for (int i = 0; i < m_meshList.size(); i++) {
		
		if (wcscmp(m_meshList[i].GetName(), name) == 0) {
			return &m_meshList[i];
		}
	}

	// return null
	return 0;
}

// Load all meshes into the manager
bool Soft351ResourceManager::LoadMeshFiles(ID3D11Device* pd3dDevice) {

	// --- Player

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Cars\\player.sdkmesh",
		true,
		L"player",
		GetShader(L"default")
	)) {
		return false;
	}

	// --- Lanes

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Lanes\\lane_l.sdkmesh",
		true,
		L"lane_l",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Lanes\\lane_m.sdkmesh",
		true,
		L"lane_m",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Lanes\\lane_r.sdkmesh",
		true,
		L"lane_r",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Lanes\\pavement.sdkmesh",
		true,
		L"pavement",
		GetShader(L"default")
	)) {
		return false;
	}

	// --- Cars

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Cars\\car01.sdkmesh",
		true,
		L"car01",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Cars\\car02.sdkmesh",
		true,
		L"car02",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Cars\\car03.sdkmesh",
		true,
		L"car03",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Cars\\car04.sdkmesh",
		true,
		L"car04",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Cars\\car05.sdkmesh",
		true,
		L"car05",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Cars\\car06.sdkmesh",
		true,
		L"car06",
		GetShader(L"default")
	)) {
		return false;
	}

	// --- Sky

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Sky\\sky.sdkmesh",
		true,
		L"sky",
		GetShader(L"default")
	)) {
		return false;
	}

	// --- Pickups

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Pickups\\sheild.sdkmesh",
		true,
		L"shield",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Pickups\\sheildpickup.sdkmesh",
		true,
		L"shieldPickup",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Pickups\\lifepickup.sdkmesh",
		true,
		L"lifePickup",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Pickups\\jumppickup.sdkmesh",
		true,
		L"jumpPickup",
		GetShader(L"default")
	)) {
		return false;
	}

	// --- Buildings

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Buildings\\building01.sdkmesh",
		true,
		L"building01",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Buildings\\building02.sdkmesh",
		true,
		L"building02",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Buildings\\building03.sdkmesh",
		true,
		L"building03",
		GetShader(L"default")
	)) {
		return false;
	}

	if (!AddMesh(
		pd3dDevice,
		L".\\Media\\Models\\Buildings\\building04.sdkmesh",
		true,
		L"building04",
		GetShader(L"default")
	)) {
		return false;
	}

	return true;
}

// Add a single mesh to the manager
bool Soft351ResourceManager::AddMesh(
	ID3D11Device* pd3Device,
	LPCTSTR meshFile,				// mesh file location	
	bool createAdjacentIndicies,
	LPCTSTR meshName,				// Unique name
	Soft351ShaderAmbient* shader			// shader for the mesh to use
)
{
	bool result;
	Soft351Mesh mesh;

	// attempt to load the mesh file
	result = mesh.Setup(
		pd3Device,
		meshFile,
		createAdjacentIndicies,
		meshName,
		shader
	);
	// if failed to load return false
	if (!result) {
		return false;
	}
	
	// add object to the mesh list
	m_meshList.push_back(mesh);

	return true;
}


// --- SHADERS

// Add a single mesh to the manager
bool Soft351ResourceManager::AddShader(
	LPCTSTR shaderName,				// unique name
	ID3D11Device* renderingDevice
)
{
	HRESULT result = S_OK;
	bool test;

	Soft351ShaderAmbient shader;
	
	shader.LoadShader(renderingDevice);
	shader.m_name = shaderName;

	// Add to shader list
	m_shaderList.push_back(shader);

	return true;
}

// Get shader by unique name
Soft351ShaderAmbient* Soft351ResourceManager::GetShader(LPCTSTR shaderName)
{
	// Loop through list and check the name, if match return the object
	for (int i = 0; i < m_shaderList.size(); i++) {
		if (wcscmp(m_shaderList[i].m_name, shaderName) == 0) {
			return &m_shaderList[i];
		}
	}

	// return null
	return 0;
}

// Load all shaders
bool Soft351ResourceManager::LoadShaders(
	ID3D11Device* renderingDevice
)
{
	if (!AddShader(
		L"default",
		renderingDevice
	)) {
		return false;
	}


	return true;
}

// --- Sounds

// Add a single 
bool Soft351ResourceManager::AddSound(
	char* location,
	long int volume,
	LPCTSTR name
)
{
	Soft351SoundFile sound;

	// attempted to load the sound file
	if (!sound.LoadFile(location, volume)) {
		// if failed return false
		return false;
	}
	// set the unique name
	sound.m_name = name;
	// add to sound list
	m_soundList.push_back(sound);
}

// Load all sound files
bool Soft351ResourceManager::LoadSoundFiles() {

	// -- UI

	if (!AddSound(
		".\\Media\\sound\\ui\\click.wav",
		100,
		L"ui-click"
	)) {
		return false;
	}
	// -- effects
	// pickups
	if (!AddSound(
		".\\Media\\sound\\effects\\itempickup.wav",
		100,
		L"effect-pickup-item"
	)) {
		return false;
	}

	if (!AddSound(
		".\\Media\\sound\\effects\\shieldpickup.wav",
		100,
		L"effect-pickup-shield"
	)) {
		return false;
	}

	if (!AddSound(
		".\\Media\\sound\\effects\\shieldhit.wav",
		100,
		L"effect-pickup-shield-hit"
	)) {
		return false;
	}

	if (!AddSound(
		".\\Media\\sound\\effects\\jump.wav",
		100,
		L"effect-pickup-jump"
	)) {
		return false;
	}

	// car
	if (!AddSound(
		".\\Media\\sound\\effects\\carcrash.wav",
		80,
		L"effect-car-crash"
	)) {
		return false;
	}

	if (!AddSound(
		".\\Media\\sound\\effects\\carlifecrash.wav",
		80,
		L"effect-car-crash-life"
	)) {
		return false;
	}

	if (!AddSound(
		".\\Media\\sound\\effects\\carloop.wav",
		60,
		L"effect-car-loop"
	)) {
		return false;
	}

	// -- music
	if (!AddSound(
		".\\Media\\sound\\music\\menuMusic.wav",
		45,
		L"music-menu"
	)) {
		return false;
	}

	if (!AddSound(
		".\\Media\\sound\\music\\gameMusic.wav",
		45,
		L"music-game"
	)) {
		return false;
	}


	return true;
}

// Get sound by unique name
Soft351SoundFile* Soft351ResourceManager::GetSound(LPCTSTR soundName)
{
	// Loop through list and check the name, if match return the object
	for (int i = 0; i < m_soundList.size(); i++) {
		if (wcscmp(m_soundList[i].m_name, soundName) == 0) {
			return &m_soundList[i];
		}
	}

	// return null
	return 0;
}

// Update lighting to all meshes
void Soft351ResourceManager::UpdateLighting(
	XMFLOAT4 ambientLight,
	XMFLOAT4 lightDiffuse,
	XMVECTOR lightDirection
)
{
	for (int i = 0; i < m_meshList.size(); i++) {
		m_meshList[i].SetAmbientLight(ambientLight);
		m_meshList[i].SetDiffuseLight(lightDiffuse);
		m_meshList[i].SetSunPosition(lightDirection);
	}
}