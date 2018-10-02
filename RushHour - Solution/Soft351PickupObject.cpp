#include "DXUT.h"
#include "Soft351PickupObject.h"

// Construct pickup object and gameobject parent
// initialize values
Soft351PickupObject::Soft351PickupObject()
	: Soft351WorldObject()
{
	m_type = PickupType::Jump;
	m_rotateSpeed = 50.0f;
}

// Deconstruction
Soft351PickupObject::~Soft351PickupObject()
{
}

// return the pickup type
Soft351PickupObject::PickupType Soft351PickupObject::GetType() {
	return m_type;
}

// Set the pickup type
void Soft351PickupObject::SetType(Soft351PickupObject::PickupType type) {
	m_type = type;
}

// Update the pickup behavior each frame
void Soft351PickupObject::Update(float fTime) {
	// Get the current rotation
	XMFLOAT3 rot = transform.GetRotation();
	// Add rotation locked by frameTime
	rot.y += m_rotateSpeed * fTime;
	// Set the rotation
	transform.SetRotation(
		rot.x,
		rot.y,
		rot.z
	);
}
