//**************************************************************************//
// The DirectX9 Thing3D now ported to DirextX10.  This module is based on	//
// Microsoft’s “Simple Sample”, but I have done quite a lot of Googleing.   //
// Apart from using my own Thing3D.											//
//																			//
// This (Googled) code is copyright of Dr Nigel Barlow, lecturer in			//
// computing, University of Plymouth, UK.  email: nigel@soc.plymouth.ac.uk.	//
//																			//
// You may use, modify and distribute this (rather cack-handed in places)	//
// code subject to the following conditions:								//
//																			//
//	1:	You may not use it, or sell it, or use it in any adapted form for	//
//		financial gain, without my written permission.						// 
//																			//
//	2:	You must not remove the copyright messages.							//
//																			//
//	3:	You should correct at least 10% of the typing and speaking errors.  //
//																			//
// Googleing ©Nigel Barlow nigel@soc.plymouth.ac.uk.						//
// "Simple Sample" ©Microsoft Corporation. All rights reserved.				//
//**************************************************************************//

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "SDKmisc.h"

// ------------ My additions
// --- Includes:
#include <D3DX11.h>
#include <DirectXMath.h>
#include "Soft351Camera.h"
#include "Soft351ResourceManager.h"
#include "Soft351PlayerObject.h"
#include "Soft351Input.h"
#include "Soft351World.h"
#include "Soft351GameManager.h"
#include "Soft351Hud.h"
#include "Soft351Sound.h"
#include "Soft351WorldObject.h"
#include "Soft351Sun.h"

// --- NameSpace:
// Using DirectX namespace to include all D3dmath elements
using namespace DirectX;

// --- Library:
// Added to fix problems 
#pragma comment(lib, "legacy_stdio_definitions.lib")

// --- Definitions
//#define DEBUG_VS   // Uncomment this line to debug D3D9 vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug D3D9 pixel shaders 

// --- Global Variables:
// window:
int		g_width			= 1280;			// Wide screen
int		g_height		= 720;			// 720p 
bool	g_windowed		= true;			// Windowed mode?
WCHAR*	g_gameTitle		= L"Rush Hour";	// Window and game title
HWND	g_hwnd;							// Window handle
HINSTANCE g_hInstance;					// Window instance in memory
// DXUT:
D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0; // DirectX 11 
// Background:
Soft351ResourceManager	g_resources;	// Game asset loader and holder
Soft351GameManager*		g_gameManager;	// Game state manager
Soft351Sound*			g_sound;		// DirectSound loader and player
Soft351Input*			g_input;		// DirectInput loader and manager
Soft351World*			g_world;		// Game object manager for world objects (non player/camera/sky)
Soft351Sun				g_sun;			// Sun settings manager
// Game objects:
Soft351Camera*			g_camera;		// Camera object for view and projection matrix handling
Soft351PlayerObject*	g_player;		// Players game object 
Soft351WorldObject*		g_sky;			// Skybox object
// UI:
Soft351Hud*				g_hudManager;	// UI management
CDXUTDialogResourceManager  g_UIresourceManager; // manager for shared resources of dialogs
CDXUTDialog             g_HUD;			// dialog for standard controls


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
	void* pUserContext);
void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);

bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);
HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext);
HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);
void CALLBACK OnD3D11DestroyDevice(void* pUserContext);
void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
	float fElapsedTime, void* pUserContext);

void InitApp();


void HasEscBeenPressed(bool force = false);

//**************************************************************************//
// A Windows program always kicks off in WinMain.							//
// Initializes everything and goes into a message processing				//
// loop.																	//
//																			//
// This version uses DXUT, and is much more complicated than previous		//
// versions you have seen.  This allows you to sync the frame rate to your  //
// monitor's vertical sync event.											//
//**************************************************************************//
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int        nCmdShow)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//**************************************************************************//
	// Set DXUT callbacks.														//
	//**************************************************************************//
	DXUTSetCallbackMsgProc(MsgProc);
	DXUTSetCallbackFrameMove(OnFrameMove);
	DXUTSetCallbackDeviceChanging(ModifyDeviceSettings);
	// D3D11 Callbacks
	DXUTSetCallbackD3D11DeviceAcceptable(IsD3D11DeviceAcceptable);
	DXUTSetCallbackD3D11DeviceCreated(OnD3D11CreateDevice);
	DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain);
	DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain);
	DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DestroyDevice);
	DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender);

	// --- Initialize app
	InitApp();

	// --- Initialize and setup DXUT
	DXUTInit(true, true, NULL); 
	DXUTSetCursorSettings(true, true);
	// --- Create window and device with DXUT
	DXUTCreateWindow(g_gameTitle, hInstance, 0, 0);
	DXUTCreateDevice(g_featureLevel, g_windowed, g_width, g_height);
	// --- DirectInput
	g_input = new Soft351Input;					// Create object
	if (!g_input->Setup(hInstance, DXUTGetHWND())) {	// Setup directInput
		MessageBox(g_hwnd, L"DirectInput failed to initialize, Game controls unavailable.", L"WinMain - dInput", MB_OK);
	}
	// Initialize states
	g_input->Frame();

	// --- gVars
	// Set global vars
	g_hwnd = DXUTGetHWND();
	g_hInstance = hInstance;

	// --- Enter into the DXUT render loop
	DXUTMainLoop();
	// --- Return DXUT exit code
	return DXUTGetExitCode();
}

//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{

	// --- UI
	// Setup DXUT ui hud
	g_HUD.Init(&g_UIresourceManager);
	// Set callback for ui actions
	g_HUD.SetCallback(OnGUIEvent);
	// Setup UI manager and pass DXUT hud into the manager
	g_hudManager = new Soft351Hud(
		g_HUD,
		g_width,
		g_height,
		g_gameTitle
	);

	// Setup the main menu
	g_hudManager->DrawMainMenu();
}


//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
	return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	HRESULT hr;

	ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();	

	// --- UI:
	// Setup resource manager
	V_RETURN(g_UIresourceManager.OnD3D11CreateDevice(pd3dDevice, pd3dImmediateContext));

	// --- Assets:
	// Load all shaders to memory
	if (!g_resources.LoadShaders(pd3dDevice)) {
		MessageBox(g_hwnd, L"Failed to load shaders", L"Resource Manager", MB_OK);
		return S_FALSE;
	}
	// Load all meshes to memory
	if (!g_resources.LoadMeshFiles(pd3dDevice)) {
		MessageBox(g_hwnd, L"Failed to load meshes", L"Resource Manager", MB_OK);
		return S_FALSE;
	}
	// Load all sounds to memory
	if (!g_resources.LoadSoundFiles()) {
		MessageBox(g_hwnd, L"Failed to load sounds", L"Resource Manager", MB_OK);
		return S_FALSE;
	}
	
	// --- DirectSound
	// Setup directSound manager
	// Music from https://www.dl-sounds.com/royalty-free
	g_sound = new Soft351Sound;
	g_sound->Setup(DXUTGetHWND());
	// Start menu music
	g_sound->PlayMusic(g_resources.GetSound(L"music-menu"));

	// --- Game management:
	// Setup game manager and set state to menu
	g_gameManager = new Soft351GameManager;
	g_gameManager->SetState(Soft351GameManager::GameState::MainMenu);

	// --- Game Camera:
	// Setup Game camera for view matrix and projection matrix
	g_camera = new Soft351Camera();
	g_camera->Setup(
		g_height,	// window width
		g_width,	// window height
		0.01f,		// near clip
		1000.0f		// far clip
	);
	// Set the cameras default position
	g_camera->transform.SetPosition(15.0f, 5.0f, -4.0f);
	// Set the cameras default rotation
	g_camera->transform.SetRotation(30.0f, 0.0f, 0.0f);
	
	// --- World Management:
	// Setup the sun
	g_sun.SetPosition(90, 120, 0);
	g_sun.SetAmbient(0.15f, 0.15f, 0.15f);
	g_sun.SetDiffuseColour(0.1f, 0.1f, 0.1f);
	// Setup sun settings for all objects
	g_resources.UpdateLighting(
		g_sun.GetAmbientLight(),
		g_sun.GetDiffuseColour(),
		g_sun.GetPosition()
	);

	// Setup the world manager
	g_world = new Soft351World(
		pd3dDevice,
		pd3dImmediateContext,
		g_resources	// asset manager
	);
	
	// --- Game objects:
	// Create the player
	g_player = new Soft351PlayerObject();
	// Give the player a mesh from the asset manager
	g_player->mesh = g_resources.GetMesh(L"player");
	// give the shield pickup a mesh from the asset manager
	g_player->meshShield = g_resources.GetMesh(L"shield");
	// Object has alpha based texture
	g_player->meshShield->alphaTexture = true;

	// Create the skybox
	g_sky = new Soft351WorldObject;
	// Give the skybox a mesh from the asset manager
	g_sky->mesh = g_resources.GetMesh(L"sky");
	// Set the scale of the skybox (Bigger the better)
	g_sky->transform.SetScale(50, 50, 50);
	// Disable ambient light
	g_sky->mesh->m_useAmbientLight = false;

	// Everything went fine, return ok!
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	HRESULT hr;

	// Update UI resource management
	V_RETURN(g_UIresourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));

	// Update UI manager
	g_hudManager->Update(g_width, g_height);

	// Update window settings
	g_width = pBackBufferSurfaceDesc->Width;
	g_height = pBackBufferSurfaceDesc->Height;

	// Update camera aspect ratio
	/*float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_camera->Setup(
		g_width,
		g_height,
		0.1f,
		1000.0f
	);	*/

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
	float fElapsedTime, void* pUserContext)
{
	// --- Camera Rendering
	// Setup clear colour holder
	float ClearColor[4];
	// Get the clear colour from camera manager
	g_camera->GetClearColour(ClearColor);
	// Render camera (setup view matrix/projection matrix for new positioning)
	g_camera->Render();

	// Update DXUT target view
	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	// Clear the depth stencil
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
	ID3D11DepthStencilState* dstate;
	
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);
	
	// --- Game objects:
	// If game state is currently playing or end menu, not main menu
	if (
		g_gameManager->GetState() == Soft351GameManager::GameState::Playing 
		||
		g_gameManager->GetState() == Soft351GameManager::GameState::EndScreen
	) {

		// player set projection and view matrix updates
		g_player->SetProjectionMatrix(g_camera->GetProjectionMatrix());
		g_player->SetViewMatrix(g_camera->GetViewMatrix());
		// Render the player!
		g_player->Render(pd3dDevice, pd3dImmediateContext);
		// if player shield is active, render that too
		if (g_player->SheildActive()) g_player->RenderSheild(pd3dDevice, pd3dImmediateContext);
		// Render all world objects
		g_world->Render(
			g_camera->GetViewMatrix(),		// pass new view matrix
			g_camera->GetProjectionMatrix()	// pass new projection matrix
		);
	}
	// Keep skybox during menu  screens
	// Render the skybox
	g_sky->SetProjectionMatrix(g_camera->GetProjectionMatrix());
	g_sky->SetViewMatrix(g_camera->GetViewMatrix());
	g_sky->Render(pd3dDevice, pd3dImmediateContext);
	
	DXUT_BeginPerfEvent(DXUT_PERFEVENTCOLOR, L"HUD / Stats");
	g_HUD.OnRender(fElapsedTime);
	DXUT_EndPerfEvent();
}



//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
{
	g_UIresourceManager.OnD3D11ReleasingSwapChain();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice(void* pUserContext)
{
	MessageBox(0, L"TEST END", L"", MB_OK);
	if (g_input) {
		g_input->Release();
		delete g_input;
	}

	if (g_camera) {
		delete g_camera;
	}

	if (g_gameManager) {
		delete g_gameManager;
	}

	if (g_sound) {
		g_sound->Release();
		delete g_sound;
	}

	if (g_world) {
		delete g_world;
	}

	if (g_hudManager) {
		delete g_hudManager;
	}

	g_resources.Release();

	g_UIresourceManager.OnD3D11DestroyDevice();
	DXUTGetGlobalResourceCache().OnDestroyDevice();
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D11 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	if (pDeviceSettings->ver == DXUT_D3D9_DEVICE)
	{
		IDirect3D9* pD3D = DXUTGetD3D9Object();
		D3DCAPS9 Caps;
		pD3D->GetDeviceCaps(pDeviceSettings->d3d9.AdapterOrdinal, pDeviceSettings->d3d9.DeviceType, &Caps);

		// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
		// then switch to SWVP.
		if ((Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
			Caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		{
			pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		// Debugging vertex shaders requires either REF or software vertex processing 
		// and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
		if (pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF)
		{
			pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
			pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
			pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
#endif
#ifdef DEBUG_PS
		pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif
	}

	// For the first device created if its a REF device, optionally display a warning dialog box
	static bool s_bFirstTime = true;
	if (s_bFirstTime)
	{
		s_bFirstTime = false;
		if ((DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF) ||
			(DXUT_D3D11_DEVICE == pDeviceSettings->ver &&
				pDeviceSettings->d3d11.DriverType == D3D_DRIVER_TYPE_REFERENCE))
		{
			DXUTDisplaySwitchingToREFWarning(pDeviceSettings->ver);
		}

	}

	return true;
}


//**************************************************************************//
// Handle updates to the scene.  This is called regardless of which D3D		//
// API is used (we are only using Dx11).									//
//**************************************************************************//
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	// --- Input:
	// Allow directInput to update keyboard and mouse states
	g_input->Frame();

	// --- Game objects:
	// If game state is currently playing, not menus
	if (g_gameManager->GetState() == Soft351GameManager::GameState::Playing) {

		// --- UI:
		// Update all text in the playing UI
		g_hudManager->UpdateGameUI(
			g_player->GetLives(),	// Update life count
			g_player->GetJumps(),	// Update jump count
			g_world->GetScore()	// Update player score
		);
		// Update shield UI
		g_hudManager->UpdateShieldUI(
			g_player->SheildActive(),
			g_player->ShieldTimeRemaining()
		);
		// Do the players behaviour and pass input to the object
		g_player->Update(*g_input, fElapsedTime);
		// DO the behaviour for all world objects via World manager
		g_world->Update(fElapsedTime);

		// --- Player sound
		// if player has pressed jump play the sound effect
		if (g_player->jumpPressed) {
			// Reset jump press to false to stop looping
			g_player->jumpPressed = false;
			// Get the sound effect from the asset manager
			g_sound->PlayEffect(g_resources.GetSound(L"effect-pickup-jump"));
		}

		// --- Player Collision
		// Get the transform of the closet car from the world manager
		Soft351Transform* transform = g_world->GetNearestCar(g_player->transform.GetPosition());
		// Check if player has collided with the car
		if (g_player->Collision(transform)) {
			// If player has shield active disable shield and play sound effect
			if (g_player->SheildActive()) {
				// Also remove the car in question!
				g_world->DeleteCollidedCar(g_player->transform.GetPosition());
				g_player->DissableSheild();
				// Get the sound effect from the asset manager
				g_sound->PlayEffect(g_resources.GetSound(L"effect-pickup-shield-hit"));
			}
			// If the player has lives remove one and play sound effect
			else if (g_player->GetLives() > 0)
			{
				// remove the car in question, repeat code here due to next else NOT needing this functionality
				g_world->DeleteCollidedCar(g_player->transform.GetPosition());
				g_player->RemoveLife();
				// Get the sound effect from the asset manager
				g_sound->PlayEffect(g_resources.GetSound(L"effect-car-crash-life"));
			}
			// If all else fails, the player crashes and moves to the game over screen
			else {
				// Set the game state to end screen
				g_gameManager->SetState(Soft351GameManager::GameState::EndScreen);
				// Stop car loop sound
				g_sound->StopCarLoop();
				// Player car crash effect, sound fetched from asset manager
				g_sound->PlayEffect(g_resources.GetSound(L"effect-car-crash"));
				// Draw the end game menu
				g_hudManager->DrawEndGame(g_world->GetScore());
			}
		}

		// --- Player pickup collision
		// Has the player collided with a pickup, return value greater than 0 if yes
		if (g_world->CollidedPickup(g_player->transform.GetPosition()) > 0) {
			// Get the pickup code from the pickup collision method
			int pickup = g_world->CollidedPickup(
				g_player->transform.GetPosition(),
				true // Delete the pickup this time!
			);

			// If pickup is shield play the shield sound effect
			if (pickup == 2) g_sound->PlayEffect(g_resources.GetSound(L"effect-pickup-shield"));
			// Else play the usual pickup sound effect
			else g_sound->PlayEffect(g_resources.GetSound(L"effect-pickup-item"));
			// Add the pickup to player via pickup code (Enum switched INT)
			g_player->AddPickup(pickup);
		}

		// --- Camera:
		// Get the players current position
		XMFLOAT3 pos = g_player->transform.GetPosition();
		pos.y += 5.0f; // Add camera offset height
		pos.z -= 4.0f; // Add camera offset behind
					   // Lerp the camera to the new position for movement smoothing
		g_camera->transform.MoveLerp(pos, 0.02f);
	}

	// --- Skybox:
	// Gently rotate the sky
	XMFLOAT3 rot = g_sky->transform.GetRotation();
	g_sky->transform.SetRotation(
		rot.x,
		rot.y + (5.0f * fElapsedTime),
		rot.z
	);

	// Check inputs for escape presses
	HasEscBeenPressed();
}

// Test to see if escape has been pressed and go to main menu or quit the game
void HasEscBeenPressed(bool force){
	// If player presses escape key
	if (g_input->GetKeyDown(DIK_ESCAPE) || force) {
		// If in menu, shut the program
		if (g_gameManager->GetState() == Soft351GameManager::GameState::MainMenu) {
			DXUTShutdown();
		}
		// else return to the main menu
		else
		{
			// Draw main menu (auto clears current UI)
			g_hudManager->DrawMainMenu();
			// Set the game state to menu
			g_gameManager->SetState(Soft351GameManager::GameState::MainMenu);
			// Clear the world
			g_world->ResetWorld();
			// Stop car sound effect
			g_sound->StopCarLoop();
			// Stop game music
			g_sound->StopMusic();
			// Start menu music
			g_sound->PlayMusic(g_resources.GetSound(L"music-menu"));
		}
	}
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
	void* pUserContext)
{
	if(!g_hwnd)
		g_hwnd = hWnd;

	// Pass messages to dialog resource manager calls so GUI state is updated correctly
	*pbNoFurtherProcessing = g_UIresourceManager.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;


	return 0;
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	// If control pressed, play sound effect
	if (nControlID > 0) {
		g_sound->PlayEffect(g_resources.GetSound(L"ui-click"));
	}
	
	// If start button was pressed
	if (nControlID == g_hudManager->StartButtonID()) {
		// Update the worlds difficulty to selected
		g_world->SetDifficulty(g_hudManager->GetDifficulty());
		// Setup the world objects with number of inputted lanes
		g_world->SetupWorld(
			g_hudManager->GetNumberLanes()
		);
		// Setup the players control to the inputted control type
		g_player->SetupControl(
			g_hudManager->GetNumberLanes(),
			g_hudManager->GetControlType()
		);
		// Set the sensitivity of the mouse to the inputed value
		g_input->SetSensitivity(g_hudManager->GetSensitivity());
		// Reset the players items and stats
		g_player->ResetPlayer();
		// Draw the game HUD (auto clears UI)
		g_hudManager->DrawGameMenu(g_hudManager->GetControlType());
		// Stop menu Music
		g_sound->StopMusic();
		// Set game state to playing
		g_gameManager->SetState(Soft351GameManager::GameState::Playing);
		// Play the game music
		g_sound->PlayMusic(g_resources.GetSound(L"music-game"));
		// Play the cars sound loop
		g_sound->PlayCarLoop(g_resources.GetSound(L"effect-car-loop"));
	}

	// If quit button has been pressed
	if (
		nControlID == g_hudManager->QuitButtonID() ||
		nControlID == g_hudManager->ReturnToMenuButtonID()
	) {
		HasEscBeenPressed(true);
	}
}



