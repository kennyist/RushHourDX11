#include "Soft351Camera.h"

// --- Usefull links:
// XMMATRIX:
// https://msdn.microsoft.com/en-us/library/Ee419959(v=VS.85).aspx
// Load float:
// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.loading.xmloadfloat(v=vs.85).aspx
// Convert to radians:
// https://docs.microsoft.com/en-us/windows/desktop/api/directxmath/nf-directxmath-xmconverttoradians
// Transform coords:
// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.transformation.xmvector3transformcoord(v=vs.85).aspx
// Look at:
// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.matrix.xmmatrixlookatlh(v=vs.85).aspx

// Constructor and set up default clear colour values
Soft351Camera::Soft351Camera()
{
	transform.Setup();
	m_clearColour[0] = 0.176f;
	m_clearColour[1] = 0.196f;
	m_clearColour[2] = 0.667f;
	m_clearColour[3] = 0.0f;
}


Soft351Camera::~Soft351Camera()
{
}

// Setup the camera and initial values
void Soft351Camera::Setup(
	float height,		// Screen height
	float width,		// screen width
	float nearClip,		// Near clip plane
	float farClip		// far clip plane
)
{
	// Generate projection matrix (Only happens once)
	m_projectionMatrix = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,						// Field of view
		(float)width / (float)height,	// Aspect ratio
		nearClip,						// Near clipping plane
		farClip							// Far clipping plane
	);					

	Render();	// Fill view and projection matrix values
}

// Generate updated view matrix and projection matrix each frame
void Soft351Camera::Render() {
	// Get up facing vector
	XMVECTOR upVec = XMLoadFloat3(&transform.up);
	// Get the current position of the camera
	XMVECTOR position = XMLoadFloat3(&transform.GetPosition());
	// Generate look at float
	XMFLOAT3 lookAt = XMFLOAT3(
		0.0f,
		0.0f,
		1.0f
	);
	// Convert the look at float to a vector
	XMVECTOR lookAtVec = XMLoadFloat3(&lookAt);
	// Convert current rotation into a matrix
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(transform.GetRotation().x),
		XMConvertToRadians(transform.GetRotation().y),
		XMConvertToRadians(transform.GetRotation().z)
	);
	// Transform lookAtVec by rotation matrix into a vector
	lookAtVec = XMVector3TransformCoord(lookAtVec, rotationMatrix);
	// Repeat for upVector
	upVec = XMVector3TransformCoord(upVec, rotationMatrix);
	// Add position and look at vector together
	lookAtVec = XMVectorAdd(position, lookAtVec);
	// Mix all three vectors into a single look at matrix
	m_viewMatrix = XMMatrixLookAtLH(position, lookAtVec, upVec);
}

// Return the rendered view matrix value
XMMATRIX Soft351Camera::GetViewMatrix() {
	return m_viewMatrix;
}

// Return the rendered projection matrix value
XMMATRIX Soft351Camera::GetProjectionMatrix() {
	return m_projectionMatrix;
}

// Return the current clear colour values
float* Soft351Camera::GetClearColour(float arr[]) {
	for (int i = 0; i < 4; ++i) {
		arr[i] = m_clearColour[i];
	}
	return arr;
}

// Update the clear colour values
void Soft351Camera::SetClearColour(float r, float g, float b, float a) {
	m_clearColour[0] = r;
	m_clearColour[1] = g;
	m_clearColour[2] = b;
	m_clearColour[3] = a;
}