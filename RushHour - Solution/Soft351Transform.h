#include <DirectXMath.h>
using namespace DirectX;

#ifndef SOFT351TRANSFORM_H
#define SOFT351TRANSFORM_H

// Position and Rotation holder, based on Unitys system
class Soft351Transform
{
public:
	// --- Consts
	// Which way is up
	const XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	// Which was is left
	const XMFLOAT3 left = XMFLOAT3(-1.0f, 0.0f, 0.0f);

	// --- Constructor and deconstruction
	Soft351Transform();
	~Soft351Transform();

	// --- Public Vars
	// Setup default values
	void Setup();
	// (Unused at the moment) Set parent
	void SetParent(Soft351Transform*);
	// Set the position of this object
	void SetPosition(float, float, float);
	// Set the rotation of this object
	void SetRotation(float, float, float);
	// Set the scale of this object
	void SetScale(float, float, float);
	// Move relative to current position
	void MoveRelative(float, float, float);
	// Rotation Relative to current rotation
	void RotateRelative(float, float, float);
	// Scale object relative to current Scale
	void ScaleRelative(float x, float y, float z);
	// Move to position with Lerp smoothing
	void MoveLerp(XMFLOAT3 target, float speed);
	// Get current position
	XMFLOAT3 GetPosition();
	// Get current Rotation
	XMFLOAT3 GetRotation();
	// Get Current scale
	XMFLOAT3 GetScale();
	// Get the world matrix of this object
	XMMATRIX GetWorldMatrix();

private:
	// --- Private Vars
	// positioning
	float x, y, z;
	// Rotation
	float rx, ry, rz;
	// scaling
	float sx, sy, sz;
	// Parent transform of this object (unused currently)
	Soft351Transform* parent;
};

#endif
