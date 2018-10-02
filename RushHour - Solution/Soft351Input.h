#ifndef SOFT351INPUT_H
#define SOFT351INPUT_H

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

class Soft351Input
{
public:
	// --- Consts
	// directInput version
	const DWORD dinput_ver = 0x0800;

	// --- Constructor and deconstructions
	Soft351Input();
	~Soft351Input();

	// --- Public methods
	// Setup and initalize directInput
	bool Setup(HINSTANCE, HWND);
	// Release all objects before deletion
	void Release();
	// Update mouse and keyboard states each frame
	bool Frame();
	// get key state held down
	bool GetKey(BYTE key);
	// Get key state on press and not held
	bool GetKeyDown(BYTE key);
	// Get key state on unpress and not held
	bool GetKeyUp(BYTE key);
	// Get mouse acceleration on X axis with dampening
	float MouseAccelerationXDamp();
	// If mouse button has been clicked
	bool MouseClick(int button);
	// Set the mouse sensitivity
	void SetSensitivity(float sensitivy);

private:
	// --- Private methods
	// Read the keyboard state
	bool ReadKeyboard();
	// Read the mouse state
	bool ReadMouse();

	// --- Private variables
	// Direct Input interface
	IDirectInput8* m_directInput;
	// Direct input device keyboard
	IDirectInputDevice8* m_keyboard;
	// Direct input device mouse
	IDirectInputDevice8* m_mouse;
	// Mouse state holder
	DIMOUSESTATE m_mouseState;
	// Previous mouse state 
	DIMOUSESTATE m_oldMouseState;
	// Current keyboard key state, by ACSII key number
	unsigned char m_keyboardState[256];
	// Array of current key states as bool
	bool m_pressedKeys[256];
	// Array of keys that have has single pressed activated
	bool m_singlePress[256];
	// Array of keys that are available for keyUP state
	bool m_keyUp[256];
	// Mouse sensitivity
	float m_sensitvity;
};

#endif // !SOFT351INPUT_H