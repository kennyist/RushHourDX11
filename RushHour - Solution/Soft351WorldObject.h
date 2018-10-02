#ifndef SOFT351WORLDOBJECT_H
#define SOFT351WORLDOBJECT_H

#include "Soft351GameObject.h"
#include "Soft351Mesh.h"

// Softworldobject adds mesh support to gameobject
// inherits transform abilities from gameobject
class Soft351WorldObject : public Soft351GameObject
{
public:
	// --- Constructor and deconstruction
	Soft351WorldObject(
	);
	~Soft351WorldObject();

	// --- public methods
	// Update the objects behavior
	virtual void Update(float frameTime);
	// Render the object
	void Render(
		ID3D11Device *pRenderingDevice,
		ID3D11DeviceContext *pImmediateContext
	);

	// --- Public variables
	// objects mesh
	Soft351Mesh* mesh;	
};

#endif
