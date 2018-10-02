#ifndef _SOFT351SUN_H
#define _SOFT351SUN_H

#include <DirectXMath.h>
using namespace DirectX;

class Soft351Sun
{
public:
	// --- Constructor and deconstruction
	Soft351Sun();
	~Soft351Sun();

	// --- Public methods:
	// Set the suns position 
	void SetPosition(
		float x,
		float y,
		float z
	);

	// Set the ambient light colour
	void SetAmbient(
		float r,
		float g,
		float b
	);

	// Set the diffuse colour
	void SetDiffuseColour(
		float r,
		float g,
		float b
	);

	// Get the suns position as vector
	XMVECTOR GetPosition();
	// Get the sun ambient colour
	XMFLOAT4 GetAmbientLight();
	// Get the diffuse colour
	XMFLOAT4 GetDiffuseColour();
	
private:
	// --- Private vars:
	// Sun position holder
	XMFLOAT3 m_position;
	// Diffuse colour holder
	XMFLOAT4 m_diffuseColour;
	// Ambient light colour holder
	XMFLOAT4 m_ambientLight;
};



#endif // !_SOFT351SUN_H

