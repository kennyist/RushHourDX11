#include "Soft351ShaderUnlit.h"

// Constructor with inheritance
Soft351ShaderUnlit::Soft351ShaderUnlit() 
	: Soft351ShaderAbstract()
{

}

// Load and compile shader files to class/application
bool Soft351ShaderUnlit::LoadShader(
	ID3D11Device *device
) {
	// Attempt to load vertex shader 
	if (!LoadVertexShader(
		device,
		L"unlit.vs",
		"UnlitShader"
	))
	{
		// if loading failed, return false
		return false;
	}

	// Attempt to load pixel shader
	if (!LoadPixelShader(
		device,
		L"unlit.ps",
		"UnlitShader"
	)) {
		// if loading failed, return false
		return false;
	}

	return true;
}

// Release objects
void Soft351ShaderUnlit::ReleaseImplementation() {

}