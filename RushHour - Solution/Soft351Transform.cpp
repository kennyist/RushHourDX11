#include "Soft351Transform.h"

// Constructor
Soft351Transform::Soft351Transform()
{
}

// Deconstruction
Soft351Transform::~Soft351Transform()
{
}

// Setup initial values
void Soft351Transform::Setup() {
	// Zero position
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;

	// Zero Rotation
	rx = 0.0f;
	ry = 0.0f;
	rz = 0.0f;

	// Zero Scale
	sx = 1.0f;
	sy = 1.0f;
	sz = 1.0f;
}

// Set to new position
void Soft351Transform::SetPosition(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

// Set to new rotation
void Soft351Transform::SetRotation(float x, float y, float z) {
	this->rx = x;
	this->ry = y;
	this->rz = z;
}

// Set to new scale
void Soft351Transform::SetScale(float x, float y, float z) {
	this->sx = x;
	this->sy = y;
	this->sz = z;
}

// Move object relative to current position
void Soft351Transform::MoveRelative(float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;
}

// Rotate object relative to current Position
void Soft351Transform::RotateRelative(float x, float y, float z) {
	this->rx += x;
	this->ry += y;
	this->rz += z;
}

// Scale object relative to current Scale
void Soft351Transform::ScaleRelative(float x, float y, float z) {
	this->sx += x;
	this->sy += y;
	this->sz += z;
}

// Set the parent of this transform
void Soft351Transform::SetParent(Soft351Transform *transform) {
	this->parent = parent;
}

// Get the current position
XMFLOAT3 Soft351Transform::GetPosition() {
	return XMFLOAT3(
		x,
		y,
		z
	);
}

// Get the current Rotation
XMFLOAT3 Soft351Transform::GetRotation() {
	return XMFLOAT3(
		rx,
		ry,
		rz
	);
}

// Get the current scale
XMFLOAT3 Soft351Transform::GetScale() {
	return XMFLOAT3(
		sx,
		sy,
		sz
	);
}

// Get the world matrix for this object
XMMATRIX Soft351Transform::GetWorldMatrix() {
	// Create rotation matrix from rotation properties
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(rx),
		XMConvertToRadians(ry),
		XMConvertToRadians(rz)
	);

	// Create scale matrix from scale properties
	XMMATRIX scaling = XMMatrixScaling(sx, sy, sz);
	// Create translate matrix from position properties
	XMMATRIX translate = XMMatrixTranslation(x, y, z);

	// combine all into world matrix
	return scaling * rotationMatrix * translate;
}

// Move to position with Lerp smoothing
// Must be called per frame not once
void Soft351Transform::MoveLerp(XMFLOAT3 target, float speed) {
	// Get current position as vector
	XMVECTOR cpos = XMLoadFloat3(&GetPosition());
	// get target position as vector
	XMVECTOR tpos = XMLoadFloat3(&target);
	// Lerp between values using speed
	XMVECTOR lerp = XMVectorLerp(cpos, tpos, speed);

	// Set objects new position
	SetPosition(
		XMVectorGetX(lerp),
		XMVectorGetY(lerp),
		XMVectorGetZ(lerp)
	);
}