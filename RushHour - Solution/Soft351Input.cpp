#include "Soft351Input.h"

// -- Usefull links
// DirectInput8:
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee417799(v%3dvs.85)
// Directinput8Device:
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee417816(v%3dvs.85)
// DIMOUSESTATE:
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee416630(v=vs.85)

// Constructor with setting initial values
Soft351Input::Soft351Input()
{
	m_directInput = nullptr;
	m_keyboard = nullptr;
	m_mouse = nullptr;
}

Soft351Input::~Soft351Input()
{
}

// Setup and initialize directInput
bool Soft351Input::Setup(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result;

	// --- DirectInput
	// Setup directInput interface
	result = DirectInput8Create(
		hinstance, 
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8, 
		(void**)&m_directInput, 
		NULL
	);
	// If failed to create interface, return false
	if (FAILED(result))
	{
		return false;
	}

	// --- Keyboard
	// Create the direct input interface for the keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	// If failed to create interface for the keyboard, return false
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format using predefined format
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	// If failed to set the format, return false
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	// If failed to set cooperative level, return false
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the keyboard
	result = m_keyboard->Acquire();
	// If failed to acquire the keyboard, return false
	if (FAILED(result))
	{
		return false;
	}

	// -- Mouse 
	// Create directInput interface for mouse
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	// If failed to create mouse, return false
	if (FAILED(result)) {
		return false;
	}

	// Set mouse data format using predefined
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	// If failed to set format, return false
	if (FAILED(result)) {
		return false;
	}

	// Set the cooperative level for the mouse
	result = m_mouse->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND |
		DISCL_NONEXCLUSIVE
	);
	// If failed to set cooperative level return false
	if (FAILED(result)) {
		return false;
	}

	// Acquire the mouse
	result = m_mouse->Acquire();
	// If failed to acquire mouse, return false
	if (FAILED(result)) {
		return false;
	}

	return true;
}

// Release objects before deletion
void Soft351Input::Release()
{
	// Release the keyboard
	if (m_keyboard)
	{
		// Unacquire first
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = NULL;
	}

	// Release the mouse
	if (m_mouse) {
		// Unacquire first
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = NULL;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = NULL;
	}

	return;
}


bool Soft351Input::Frame()
{
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	// If failed return false
	if (!result)
	{
		return false;
	}

	// Read the current mouse state
	result = ReadMouse();
	// If failed return false
	if (!result) {
		return false;
	}

	return true;
}

bool Soft351Input::ReadMouse() {
	HRESULT result = S_OK;

	// Set the previous mouse state before updating
	m_oldMouseState = m_mouseState;

	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If mouse focus was lost, try to regain control
		if (
			(result == DIERR_INPUTLOST) || 
			(result == DIERR_NOTACQUIRED)
		)
		{
			// Reacquire the mouse
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Soft351Input::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus was lost, try to regain control
		if (
			(result == DIERR_INPUTLOST) || 
			(result == DIERR_NOTACQUIRED)
		)
		{
			// Reacquire the keyboard
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	// For each key on the keyboard, by ACSI key number
	for (int i = 0; i < 256; i++) {
		// If key is not pressed
		if (!(m_keyboardState[i] & 0x80)) {
			// if previous state was pressed, set keyup state true
			if (m_pressedKeys[i] && !m_keyUp[i]) {
				m_keyUp[i] = true;
			}
			// else set keyup state false
			else {
				m_keyUp[i] = false;
			}
			// pressed state = false
			m_pressedKeys[i] = false;
			// Single press state = false
			m_singlePress[i] = false;
		}
		// If key is pressed, set pressed state true
		else {
			m_pressedKeys[i] = true;
		}
	}

	// Return ok!
	return true;
}

// get key state held down
bool Soft351Input::GetKey(BYTE key) 
{
	return m_pressedKeys[key];
}

// Get key state on press and not held
bool Soft351Input::GetKeyDown(BYTE key) 
{
	// if key is pressed and single press state is false
	if (m_pressedKeys[key] && !m_singlePress[key]) {
		// set single press state true
		m_singlePress[key] = true;
		// return pressed
		return true;
	}


	// return not pressed
	return false;
}

// Get key state on unpress and not held
bool Soft351Input::GetKeyUp(BYTE key) {
	// return keyup state
	return m_keyUp[key];
}

// Set the mouse sensitivity
void Soft351Input::SetSensitivity(float sensitivity) {
	// reverse incoming sensitivity value
	m_sensitvity = 100.0f - sensitivity;
}

// Get mouse acceleration on X axis with dampening
float Soft351Input::MouseAccelerationXDamp() {
	// get mouse X movement as float and divide is by sensitivity
	return m_mouseState.lX / m_sensitvity;
}

// If mouse button has been clicked
bool Soft351Input::MouseClick(int button) {
	// If old mouse state shows button not pressed
	if ((m_oldMouseState.rgbButtons[button] & 0x80) != 1) {
		// if current state shows button is pressed
		if ((m_mouseState.rgbButtons[button] & 0x80) != 0) {
			// return clicked
			return true;
		}
	}
	// return not clicked
	return false;
}