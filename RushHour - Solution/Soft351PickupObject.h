#ifndef SOFT351PICKUPOBJECT_H
#define SOFT351PICKUPOBJECT_H

#include "Soft351WorldObject.h"
#include "Soft351Mesh.h"

// Pickup object child of worldobject
// inherits mesh abilities from worldobject
// inherits transform abilities from gameobject
class Soft351PickupObject : public Soft351WorldObject
{
public:
	// --- public enums:
	// Pickup type enum
	enum PickupType
	{
		Jump = 1,
		Sheild = 2,
		Life = 3
	};

	// --- Constructor & deconstruction
	Soft351PickupObject();
	~Soft351PickupObject();

	// --- Public methods
	// Pickups behavior
	void Update(float frameTime);
	// Set the pickups type
	void SetType(PickupType type);
	// Get the pickups type
	PickupType GetType();
private:
	// --- Private vars
	// pickup type storage
	PickupType m_type;
	// rotation speed of the pickup
	float m_rotateSpeed;
};

#endif

