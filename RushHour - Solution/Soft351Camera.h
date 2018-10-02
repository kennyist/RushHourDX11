// Include DirectXmath and its namespace for math elements
#include <DirectXMath.h>
using namespace DirectX;

// Include transform for position and rotation values
#include "Soft351Transform.h";

#ifndef SOFT351CAMERAOBJECT_H
#define SOFT351CAMERAOBJECT_H

class Soft351Camera
{
public:
	// --- Construct and Deconstruction
	Soft351Camera();
	~Soft351Camera();

	// --- Public vars
	// Position and rotation holder
	Soft351Transform transform;

	// --- Public methods
	// Setup the camera and its properties
	void Setup(
		float height,
		float width,
		float nearClip,
		float farClip
	);
	// Generate updated view matrix and projection matrix each frame
	void Render();
	// Get the rendered view matrix
	XMMATRIX GetViewMatrix();
	// Get the rendered projection matrix
	XMMATRIX GetProjectionMatrix();
	// Get the current clear colour values
	float * GetClearColour(float arr[]);
	// Set the clear colour to a new colour
	void SetClearColour(float r, float g, float b, float a);
private:
	// --- Private vars;
	// Store the view matrix
	XMMATRIX m_viewMatrix;
	// Store the projection matrix
	XMMATRIX m_projectionMatrix;
	// Store the clear colour
	float m_clearColour[4];
};

#endif
