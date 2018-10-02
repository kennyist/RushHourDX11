#include "Soft351Sun.h"

// Constructor and set default values
Soft351Sun::Soft351Sun()
{
	m_position = XMFLOAT3();
	m_ambientLight = XMFLOAT4();
	m_diffuseColour = XMFLOAT4();
}

// Deconstruction
Soft351Sun::~Soft351Sun()
{
}

// Set the suns positions
void Soft351Sun::SetPosition(
	float x,
	float y,
	float z
) {
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

// Set the suns ambient light colour
void Soft351Sun::SetAmbient(
	float r,
	float g,
	float b
)
{
	m_ambientLight.x = r;
	m_ambientLight.y = g;
	m_ambientLight.z = b;
	m_ambientLight.w = 1.0f;
}

// Set the suns diffuse colour
void Soft351Sun::SetDiffuseColour(
	float r,
	float g,
	float b
)
{
	m_diffuseColour.x = r;
	m_diffuseColour.y = g;
	m_diffuseColour.z = b;
	m_diffuseColour.w = 0.1f;
}


// Get the suns position
XMVECTOR Soft351Sun::GetPosition() {
	return XMVectorSet(
		m_position.x,
		m_position.y,
		m_position.z,
		1.0f
	);
}

// Get the suns ambient light colour
XMFLOAT4 Soft351Sun::GetAmbientLight() {
	return m_ambientLight;
}

// Get the suns diffuse colour
XMFLOAT4 Soft351Sun::GetDiffuseColour() {
	return m_diffuseColour;
}